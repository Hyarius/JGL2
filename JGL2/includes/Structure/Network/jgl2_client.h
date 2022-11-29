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
		using LoginSucessfulActivity = std::function< void() >;
		using LoginFailedActivity = std::function< void() >;
		using DisconnectionActivity = std::function< void() >;
	protected:
		asio::io_context _asioContext;
		jgl::Thread* _threadContext;
		jgl::Connection<TServerMessageEnum>* _connection;

		jgl::ULong _messageTimeoutDelay = 0;

		jgl::Long _majorKeyNumber = 0;
		jgl::Long _mediumKeysNumber = 0;
		jgl::Long _minorKeysNumber = 0;
		jgl::Long _abstractKeysNumber = 0;

		LoginSucessfulActivity _onSuccessfulLogin = nullptr;
		LoginFailedActivity _onFailedLogin = nullptr;
		DisconnectionActivity _onDisconnection = nullptr;

		std::map<TServerMessageEnum, ActivityFunction> _activityMap;

		jgl::LockedQueue<jgl::InputMessage<TServerMessageEnum>> _input;

		jgl::Long _computeMagicNumber(jgl::Int p_value)
		{
			return (((_majorKeyNumber << 48) ^ p_value) + ((_mediumKeysNumber << 32) & p_value) + ((_minorKeysNumber << 16) | p_value) + (_abstractKeysNumber));
		}

		void _unsecuredSend(const jgl::Message<TServerMessageEnum>& p_msg)
		{
			_connection->send(p_msg);
		}

		virtual ~Client()
		{
			disconnect();
		}

	public:
		Client() : _connection()
		{

		}

		void setSuccessfulLoginActivity(LoginSucessfulActivity p_funct)
		{
			_onSuccessfulLogin = p_funct;
		}

		void setFailedLoginActivity(LoginFailedActivity p_funct)
		{
			_onFailedLogin = p_funct;
		}

		void setDisconnectionActivity(DisconnectionActivity p_funct)
		{
			_onDisconnection = p_funct;
		}

		void addActivity(TServerMessageEnum p_msg_type, ActivityFunction p_funct)
		{
			_activityMap[p_msg_type] = p_funct;
		}

		void setKeys(jgl::Int p_major_keys, jgl::Int p_medium_keys, jgl::Int p_minor_keys, jgl::Int p_abstract_keys)
		{
			_majorKeyNumber = p_major_keys;
			_mediumKeysNumber = p_medium_keys;
			_minorKeysNumber = p_minor_keys;
			_abstractKeysNumber = p_abstract_keys;
		}

		bool connect(std::string p_host, const jgl::UShort p_port)
		{
			try
			{
				asio::ip::tcp::resolver resolver(_asioContext);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(p_host, std::to_string(p_port));

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

		bool reconnect(std::string p_host, const jgl::UShort p_port)
		{
			try
			{
				asio::ip::tcp::resolver resolver(_asioContext);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(p_host, std::to_string(p_port));

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

			if (_onDisconnection != nullptr)
			{
				_onDisconnection();
			}
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
							if (_onSuccessfulLogin != nullptr)
								_onSuccessfulLogin();
						}
						else
						{
							_connection->refusedByServer();
							if (_onFailedLogin != nullptr)
								_onFailedLogin();
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
						std::string errorMessage = "[CLIENT] - Message_received of unknow id (" + std::to_string(static_cast<jgl::Int>(msg.type())) + ")";
						throw std::runtime_error(errorMessage.c_str());
					}
				}
			}
		}

		void send(const jgl::Message<TServerMessageEnum>& p_msg)
		{
			if (_connection->state() == jgl::Connection<TServerMessageEnum>::State::Accepted)
				_connection->send(p_msg);
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