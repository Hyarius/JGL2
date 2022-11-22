#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "Structure/jgl2_locked_queue.h"
#include "Structure/Network/jgl2_message.h"
#include "Structure/Network/jgl2_connection.h"
#include "Structure/jgl2_thread.h"

namespace jgl
{
	template <typename TServerMessageEnum>
	class Client
	{
	public:
		using ActivityFunction = std::function< void(jgl::Message<TServerMessageEnum>&) >;
	protected:
		asio::io_context _asioContext;
		jgl::Thread* _threadContext;
		jgl::Connection<TServerMessageEnum>* _connection;

		jgl::ULong _messageTimeoutDelay = 0;

		jgl::Long _majorKeyNumber = 0;
		jgl::Long _mediumKeysNumber = 0;
		jgl::Long _minorKeysNumber = 0;
		jgl::Long _abstractKeysNumber = 0;

		std::map<TServerMessageEnum, ActivityFunction> _activityMap;

		jgl::LockedQueue<jgl::InputMessage<TServerMessageEnum>> _input;

		jgl::Long _computeMagicNumber(jgl::Int value)
		{
			return (((_majorKeyNumber << 48) ^ value) + ((_mediumKeysNumber << 32) & value) + ((_minorKeysNumber << 16) | value) + (_abstractKeysNumber));
		}

		void _unsecuredSend(const jgl::Message<TServerMessageEnum>& msg)
		{
			_connection->send(msg);
		}

		virtual ~Client()
		{
			disconnect();
		}

	public:
		Client() : _connection()
		{

		}

		void addActivity(TServerMessageEnum msg_type, ActivityFunction funct)
		{
			_activityMap[msg_type] = funct;
		}

		void setKeys(jgl::Int major_keys, jgl::Int medium_keys, jgl::Int minor_keys, jgl::Int abstract_keys)
		{
			_majorKeyNumber = major_keys;
			_mediumKeysNumber = medium_keys;
			_minorKeysNumber = minor_keys;
			_abstractKeysNumber = abstract_keys;
		}

		bool connect(std::string host, const jgl::UShort port)
		{
			try
			{
				std::string hostConverted = std::string(host.c_str());
				asio::ip::tcp::resolver resolver(_asioContext);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(hostConverted, std::to_string(port));

				asio::ip::tcp::socket socket(_asioContext);

				socket.open(asio::ip::tcp::v4());

				asio::ip::tcp::no_delay no_delay(true);
				socket.set_option(no_delay);

				_connection = new Connection<TServerMessageEnum>(jgl::Connection<TServerMessageEnum>::Owner::Client, _asioContext, std::move(socket), &_input);

				_connection->connectToServer(endpoints);

				_threadContext = new jgl::Thread("AsioContext", [&]()
					{
						_asioContext.run();
					});
			}
			catch (const std::exception& e)
			{
				std::string errorMessage = "Client Exception: " + std::string(e.what());
				throw std::runtime_error(errorMessage.c_str());
				return false;
			}
			catch (...)
			{
				throw std::runtime_error("Client Exception unknow");
				return false;
			}
			return true;
		}

		bool reconnect(std::string host, const jgl::UShort port)
		{
			try
			{
				std::string hostConverted = std::string(host.c_str());
				asio::ip::tcp::resolver resolver(_asioContext);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(hostConverted, std::to_string(port));

				_connection->connectToServer(endpoints);
				_connection->input()->clear();
			}
			catch (const std::exception& e)
			{
				std::string errorMessage = "Client Exception: " + std::string(e.what());
				std::runtime_error(errorMessage.c_str());
				return false;
			}
			catch (...)
			{
				std::runtime_error("Client Exception unknow");
				return false;
			}
			return true;
		}

		void disconnect()
		{
			if (isConnected())
			{
				_connection->disconnect();
			}

			_asioContext.stop();
			_threadContext->join();
			_input.clear();
		}

		bool isConnected()
		{
			if (_connection)
				return _connection->isConnected();
			else
				return false;
		}

		void update()
		{
			if (_connection == nullptr)
				return;
			if (_input.empty() == false)
			{
				if (_connection->state() == jgl::Connection<TServerMessageEnum>::State::Unknown)
				{
					auto msg = _input.pop_front().msg;

					if (msg.size() == sizeof(jgl::Long))
					{
						jgl::Long key;
						jgl::Long result;

						msg >> key;

						msg.clear();
						result = _computeMagicNumber(key);
						msg << key;
						msg << result;
						_unsecuredSend(msg);
					}
					else if (msg.size() == sizeof(bool))
					{
						bool accepted;

						msg >> accepted;
						if (accepted == true)
						{
							_connection->acceptedByServer();
						}
						else
						{
							_connection->refusedByServer();
						}
					}
					else
					{

					}
				}
				else if (_connection->state() == jgl::Connection<TServerMessageEnum>::State::Accepted)
				{
					auto input = _input.pop_front();
					auto msg = input.msg;

					if (_activityMap.count(msg.type()) != 0)
					{
						if (jgl::Application::instance() == nullptr || _messageTimeoutDelay == 0 ||
							input.time + _messageTimeoutDelay < jgl::Application::instance()->time())
						{
							_activityMap[msg.type()](msg);
						}
					}
					else
					{
						std::string errorMessage = "[CLIENT] - Message_received of unknow id (" + std::to_string(msg.type()) + ")";
						throw std::runtime_error(errorMessage.c_str());
					}
				}
			}
		}

		void send(const jgl::Message<TServerMessageEnum>& msg)
		{
			if (_connection->state() == jgl::Connection<TServerMessageEnum>::State::Accepted)
				_connection->send(msg);
		}

		jgl::LockedQueue<jgl::InputMessage<TServerMessageEnum>>& input()
		{
			return (_input);
		}

		jgl::Connection<TServerMessageEnum>* connection() const
		{
			return (_connection);
		}
	};
}