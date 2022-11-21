#pragma once

#include "Structure/jgl2_locked_queue.h"
#include "Structure/Network/jgl2_message.h"
#include "Structure/Network/jgl2_connection.h"

namespace jgl
{
	template <typename TServerMessageEnum>
	class Server
	{
	private:
		using ServerConnection = jgl::Connection<TServerMessageEnum>;

	public:
		using LoginFunct = std::function< void(ServerConnection*) >;
		using LogoutFunct = std::function< void(ServerConnection*) >;
		using ActivityFunct = std::function< void(ServerConnection*, jgl::Message<TServerMessageEnum>&) >;

	protected:
		asio::io_context _asioContext;
		std::thread _threadContext;
		asio::ip::tcp::acceptor _acceptor;

		ServerConnection* _self;
		std::deque<ServerConnection*> _activeConnection;
		std::deque<ServerConnection*> _acceptedConnection;

		LoginFunct _loginFunct = nullptr;
		LogoutFunct _logoutFunct = nullptr;
		std::map<TServerMessageEnum, ActivityFunct> _activityMap;

		jgl::Long _validationKey = 0;
		jgl::Long _majorKeysNumber = 0;
		jgl::Long _mediumKeysNumber = 0;
		jgl::Long _minorKeysNumber = 0;
		jgl::Long _abstractKeysNumber = 0;

		jgl::Bool _isActive;

		jgl::LockedQueue<jgl::InputMessage<TServerMessageEnum>> _input;

		jgl::UInt _idCount = 10000;

		jgl::Long _computeMagicNumber(jgl::Long value)
		{
			return (((_majorKeysNumber << 48) ^ value) + ((_mediumKeysNumber << 32) & value) + ((_minorKeysNumber << 16) | value) + (_abstractKeysNumber));
		}

		void _waitForConnection()
		{
			_acceptor.async_accept(
				[this](std::error_code ec, asio::ip::tcp::socket socket)
				{
					try
					{
						if (!ec)
						{
							asio::ip::tcp::no_delay noDelay(true);
							socket.set_option(noDelay);

							ServerConnection* newConnection = new ServerConnection(ServerConnection::Owner::Server, _asioContext, std::move(socket), &_input);

							_clientConnect(newConnection);

							_activeConnection.push_back(std::move(newConnection));

							if (_activeConnection.size() == 1)
								_self = _activeConnection.front();

							_activeConnection.back()->connectToClient(_idCount++);
						}
					}
					catch (const std::exception& e)
					{
						std::string errorMessage = "Server Exception: " + std::string(e.what());
						throw std::runtime_error(errorMessage.c_str());
					}
					catch (...)
					{
						throw std::runtime_error("Server Exception unknow");
					}

					_waitForConnection();
				});
		}

		void _onMessageReception(ServerConnection* client, jgl::Message<TServerMessageEnum>& msg)
		{
			if (client->state() == ServerConnection::State::Accepted)
			{
				if (_activityMap.count(msg.type()) != 0)
				{
					_activityMap[msg.type()](client, msg);
				}
				else
				{
					jgl::cout << "[SERVER] - Message_received of unknow id(" << static_cast<jgl::Int>(msg.type()) << ")" << jgl::endl;
				}
			}
			else if (client->state() == ServerConnection::State::Unknown)
			{
				_validateClientConnection(client, msg);
			}
		}

		void _clientConnect(ServerConnection* client)
		{
			jgl::Message<TServerMessageEnum> msg;

			msg << _validationKey;

			client->send(msg);
		};

		jgl::Bool _validateClientConnection(ServerConnection* client, jgl::Message<TServerMessageEnum>& msg)
		{
			jgl::Long key;
			jgl::Long clientResult;
			jgl::Long realResult;

			msg >> key;
			msg >> clientResult;

			realResult = _computeMagicNumber(key);
			if (realResult == clientResult)
			{
				client->acceptedByServer();
				if (_loginFunct != nullptr)
					_loginFunct(client);

				msg.clear();
				jgl::Bool response = true;
				msg << response;

				client->send(msg);
				return (true);
			}
			else
			{
				client->refusedByServer();
				msg.clear();
				jgl::Bool response = false;
				msg << response;
				client->send(msg);
				return (false);
			}
		}

		void _clientDisconnect(ServerConnection* client)
		{
			if (_logoutFunct != nullptr)
				_logoutFunct(client);
		};

		virtual ~Server()
		{
			stop();
		}

	public:
		Server(uint16_t port) :
			_asioContext(),
			_acceptor(_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
		{

		}

		Connection<TServerMessageEnum>* self()
		{
			return (_self);
		}

		Connection<TServerMessageEnum>* connection(jgl::Long p_id)
		{
			for (jgl::Size_t i = 0; i < _acceptedConnection.size(); i++)
			{
				if (_acceptedConnection[i]->id() == p_id)
				{
					return (_acceptedConnection[i]);
				}
			}
			return (nullptr);
		}

		void addActivity(TServerMessageEnum msg_type, ActivityFunct funct)
		{
			_activityMap[msg_type] = funct;
		}

		void setLoginFunction(LoginFunct p_loginFunct)
		{
			_loginFunct = p_loginFunct;
		}

		void setLogoutFunction(LogoutFunct p_logoutFunct)
		{
			_logoutFunct = p_logoutFunct;
		}

		void setValidationKey(jgl::Int p_validationKey)
		{
			_validationKey = p_validationKey;
		}

		void setKeys(jgl::Int major_version, jgl::Int medium_version, jgl::Int minor_version, jgl::Int abstract_version)
		{
			_majorKeysNumber = major_version;
			_mediumKeysNumber = medium_version;
			_minorKeysNumber = minor_version;
			_abstractKeysNumber = abstract_version;
		}

		void start()
		{
			try
			{
				_waitForConnection();

				_threadContext = std::thread([this]() { _asioContext.run(); });
			}
			catch (std::exception& e)
			{
				_isActive = false;
				std::string errorMessage = "[SERVER] Exception: " + std::string(e.what());
				throw std::runtime_error(errorMessage.c_str());
			}

			_isActive = true;
		}

		void stop()
		{
			_asioContext.stop();
			if (_threadContext.joinable())
			{
				_threadContext.join();
			}

			_acceptor.close();

			_isActive = false;
		}

		jgl::Bool is_active()
		{
			return (_isActive);
		}

		void sendTo(ServerConnection* client, const jgl::Message<TServerMessageEnum>& msg)
		{
			if (client != nullptr && client->isConnected())
			{
				client->send(msg);
			}
			else
			{
				_clientDisconnect(client);
				_activeConnection.erase(
					std::remove(_activeConnection.begin(), _activeConnection.end(), client), _activeConnection.end());
				_acceptedConnection.erase(
					std::remove(_acceptedConnection.begin(), _acceptedConnection.end(), client), _acceptedConnection.end());
			}
		}

		void sendToAll(const jgl::Message<TServerMessageEnum>& msg, ServerConnection* p_ignore = nullptr)
		{
			jgl::Bool error = false;

			for (auto& client : _activeConnection)
			{
				if (client != nullptr && client != p_ignore)
				{
					sendTo(client, msg);
				}
			}

			if (error == true)
			{
				_activeConnection.erase(std::remove(_activeConnection.begin(), _activeConnection.end(), nullptr), _activeConnection.end());
				_acceptedConnection.erase(std::remove(_acceptedConnection.begin(), _acceptedConnection.end(), nullptr), _acceptedConnection.end());
			}
		}

		void sendToArray(const jgl::Message<TServerMessageEnum>& p_msg, std::vector<ServerConnection*>& p_list)
		{
			for (jgl::Size_t i = 0; i < p_list.size(); i++)
			{
				if (p_list[i] != nullptr)
					sendTo(p_list[i], p_msg);
			}
		}

		void update()
		{
			while (_input.empty() == false)
			{
				auto msg = _input.pop_front();

				_onMessageReception(msg.remote, msg.msg);
			}
			for (auto& client : _activeConnection)
			{
				if (client != nullptr && client->isConnected() == false)
				{
					_clientDisconnect(client);
					delete client;
					client = nullptr;
				}
			}
		}
	};
}