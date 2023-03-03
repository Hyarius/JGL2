#pragma once

#include "Structure/Network/jgl2_message.h"
#include "Structure/jgl2_locked_queue.h"

namespace jgl
{
	namespace Network
	{
		template <typename TServerMessageEnum>
		class Connection
		{
		private:
			using ConnectionMessage = jgl::Network::Message<TServerMessageEnum>;
		public:
			enum class Owner
			{
				Error = -1,
				Server,
				Client
			};

			enum class State
			{
				Unknown = 0,
				Refused = -1,
				Accepted = 1
			};

		protected:
			Connection::State _state;
			asio::ip::tcp::socket _socket;
			asio::io_context& _context;
			jgl::LockedQueue<ConnectionMessage> _output;
			jgl::LockedQueue<jgl::Network::InputMessage<TServerMessageEnum>>* _input;
			ConnectionMessage _tmp_reading_message;
			ConnectionMessage _tmp_writing_message;
			Connection::Owner _owner = Owner::Error;
			jgl::Long _id;

		private:
			void _writeHeader()
			{
				_tmp_writing_message = _output.pop_front();

				asio::async_write(_socket, asio::buffer(&_tmp_writing_message.header, sizeof(ConnectionMessage::Header)),
					[this](std::error_code ec, size_t length)
					{
						if (!ec)
						{
							if (_tmp_writing_message.content.size() > 0)
							{
								_writeContent();
							}
							else
							{
								if (!_output.empty())
									_writeHeader();
							}
						}
						else
						{
							_socket.close();
						}
					});
			}

			void _writeContent()
			{
				asio::async_write(_socket, asio::buffer(_tmp_writing_message.content.data(), _tmp_writing_message.content.size()),
					[this](std::error_code ec, size_t length)
					{
						if (!ec)
						{
							if (!_output.empty())
								_writeHeader();
						}
						else
						{
							_socket.close();
						}
					});
			}

			void _readHeader()
			{
				_tmp_reading_message.clear();

				asio::async_read(_socket, asio::buffer(&_tmp_reading_message.header, sizeof(ConnectionMessage::Header)),
					[this](std::error_code ec, size_t length)
					{
						if (!ec)
						{
							if (_tmp_reading_message.header.size > 0)
							{
								_tmp_reading_message.content.resize(_tmp_reading_message.header.size);
								_readContent();
							}
							else
							{
								_addMessageToQueue();
							}
						}
						else
						{
							jgl::cout << "[" << _id << "] Read Header Fail.\n";
							_socket.close();
						}
					});
			}

			void _readContent()
			{
				asio::async_read(_socket, asio::buffer(_tmp_reading_message.content.data(), _tmp_reading_message.content.size()),
					[this](std::error_code ec, size_t length)
					{
						if (!ec)
						{
							_addMessageToQueue();
						}
						else
						{
							jgl::cout << "[" << _id << "] Read Body Fail.\n";
							_socket.close();
						}
					});
			}

			void _addMessageToQueue()
			{
				if (_owner == Owner::Server)
					_input->push_back({ this, _tmp_reading_message });
				else
					_input->push_back({ nullptr, _tmp_reading_message });
				_tmp_reading_message.header.sparedSpace[3] = 42;
				_readHeader();
			}

		public:
			Connection(Connection::Owner p_parent, asio::io_context& p_context, asio::ip::tcp::socket p_socket, jgl::LockedQueue<jgl::Network::InputMessage<TServerMessageEnum>>* p_input)
				: _context(p_context), _socket(std::move(p_socket)), _input(p_input), _tmp_reading_message({})
			{
				_state = Connection::State::Unknown;
				_id = 0;
				_owner = p_parent;
			}

			State state() { return (_state); }

			void acceptedByServer()
			{
				_state = Connection::State::Accepted;
			}

			void refusedByServer()
			{
				_state = Connection::State::Refused;
			}

			virtual ~Connection()
			{}

			jgl::LockedQueue<jgl::Network::InputMessage<TServerMessageEnum>>* input()
			{
				return (_input);
			}

			jgl::UInt id() const
			{
				return (_id);
			}

			void connectToClient(jgl::Long u_id = 0)
			{
				if (_owner == Connection::Owner::Server)
				{
					if (_socket.is_open())
					{
						_id = u_id;
						_readHeader();
					}
				}
			}

			void connectToServer(const asio::ip::tcp::resolver::results_type& p_endpoints)
			{
				if (_owner == Connection::Owner::Client)
				{
					asio::async_connect(_socket, p_endpoints,
						[this](std::error_code ec, asio::ip::tcp::endpoint p_endpoint)
						{
							asio::ip::tcp::no_delay no_delay(true);
							_socket.set_option(no_delay);

							if (!ec)
								_readHeader();
						});
				}
			}

			void disconnect()
			{
				if (isConnected())
				{
					asio::post(_context, [this]() { _socket.close(); });
				}
			}

			bool isConnected() const
			{
				return (_socket.is_open());
			}

			void send(const ConnectionMessage& p_msg)
			{
				if (isConnected() == false)
					return;

				if (p_msg.header.emiterID == 0)
					p_msg.header.emiterID = _id;

				asio::post(_context,
					[this, p_msg]()
					{
						bool state = _output.empty();
						_output.push_back(p_msg);
						if (state == true)
							_writeHeader();
					});
			}
		};
	}
}