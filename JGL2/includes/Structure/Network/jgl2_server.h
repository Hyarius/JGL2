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

		jgl::Long _computeMagicNumber(jgl::Long p_value)
		{
			return (((_majorKeysNumber << 48) ^ p_value) + ((_mediumKeysNumber << 32) & p_value) + ((_minorKeysNumber << 16) | p_value) + (_abstractKeysNumber));
		}

		void _waitForConnection()
		{
			_acceptor.async_accept(
				[this](std::error_code p_ec, asio::ip::tcp::socket p_socket)
				{
					try
					{
						if (!p_ec)
						{
							DEBUG_TRACE("New connection received");
							DEBUG_TRACE("Setting up sicket option option");
							asio::ip::tcp::no_delay noDelay(true);
							p_socket.set_option(noDelay);

							DEBUG_TRACE("Creating new connection for the new socket");
							ServerConnection* newConnection = new ServerConnection(ServerConnection::Owner::Server, _asioContext, std::move(p_socket), &_input);

							DEBUG_TRACE("Starting connection validation process");
							_clientConnect(newConnection);

							DEBUG_TRACE("Adding new connection to the pull");
							_activeConnection.push_back(std::move(newConnection));

							if (_activeConnection.size() == 1)
								_self = _activeConnection.front();

							DEBUG_TRACE("Connecting the new connection to destined client");

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

		void _onMessageReception(ServerConnection* p_client, jgl::Message<TServerMessageEnum>& p_msg)
		{
			if (p_client->state() == ServerConnection::State::Accepted)
			{
				DEBUG_TRACE("Reception of a new message : Connection status Accepted");
				if (_activityMap.count(p_msg.type()) != 0)
				{
					_activityMap[p_msg.type()](p_client, p_msg);
				}
				else
				{
					jgl::cout << "[SERVER] - Message_received of unknow id(" << static_cast<jgl::Int>(p_msg.type()) << ")" << jgl::endl;
				}
			}
			else if (p_client->state() == ServerConnection::State::Unknown)
			{
				DEBUG_TRACE("Reception of a new message : Connection status Accepted");
				_validateClientConnection(p_client, p_msg);
			}
		}

		void _clientConnect(ServerConnection* p_client)
		{
			DEBUG_TRACE("Send the validation key to client [" + std::to_string(p_client->id()) + "]");
			jgl::Message<TServerMessageEnum> msg;

			msg << _validationKey;

			p_client->send(msg);
		};

		jgl::Bool _validateClientConnection(ServerConnection* p_client, jgl::Message<TServerMessageEnum>& p_msg)
		{
			jgl::Long key;
			jgl::Long clientResult;
			jgl::Long realResult;

			p_msg >> key;
			p_msg >> clientResult;
			DEBUG_TRACE("Receptioning client [" + std::to_string(p_client->id()) + "] validation result [" + std::to_string(clientResult) + "] to key [" + std::to_string(key) + "]");

			realResult = _computeMagicNumber(key);
			DEBUG_TRACE("Expected result [" + std::to_string(realResult) + "]");

			if (realResult == clientResult)
			{
				DEBUG_TRACE("Connection accepted, validation magic number are matching");
				p_msg.clear();
				jgl::Bool response = true;
				p_msg << response;

				p_client->send(p_msg);
				p_client->acceptedByServer();
				if (_loginFunct != nullptr)
				{
					_loginFunct(p_client);
				}
				return (true);
			}
			else
			{
				DEBUG_TRACE("Connection refused, validation magic number are different");
				p_client->refusedByServer();
				p_msg.clear();
				jgl::Bool response = false;
				p_msg << response;
				p_client->send(p_msg);
				return (false);
			}
		}

		void _clientDisconnect(ServerConnection* p_client)
		{
			DEBUG_TRACE("Calling client disconnection process");
			if (_logoutFunct != nullptr)
				_logoutFunct(p_client);
		};

		virtual ~Server()
		{
			DEBUG_TRACE("Server destructor");
			stop();
		}

	public:
		Server(uint16_t p_port) :
			_asioContext(),
			_acceptor(_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), p_port))
		{
			DEBUG_TRACE("Server constructor");
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

		void addActivity(TServerMessageEnum p_msg_type, ActivityFunct p_funct)
		{
			_activityMap[p_msg_type] = p_funct;
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

		void setKeys(jgl::Int p_major_version, jgl::Int p_medium_version, jgl::Int p_minor_version, jgl::Int p_abstract_version)
		{
			_majorKeysNumber = p_major_version;
			_mediumKeysNumber = p_medium_version;
			_minorKeysNumber = p_minor_version;
			_abstractKeysNumber = p_abstract_version;
		}

		void start()
		{
			try
			{
				DEBUG_TRACE("Server starting process");
				_waitForConnection();

				DEBUG_TRACE("Starting context thread");
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
			DEBUG_TRACE("Server stopping process");
			_asioContext.stop();
			DEBUG_TRACE("Joining context thread");
			if (_threadContext.joinable())
			{
				_threadContext.join();
			}

			DEBUG_TRACE("Closing acceptor");
			_acceptor.close();

			_isActive = false;
		}

		jgl::Bool is_active()
		{
			return (_isActive);
		}

		void sendTo(ServerConnection* p_client, const jgl::Message<TServerMessageEnum>& p_msg)
		{
			if (p_client != nullptr && p_client->isConnected())
			{
				p_client->send(p_msg);
			}
			else
			{
				_clientDisconnect(p_client);
				_activeConnection.erase(
					std::remove(_activeConnection.begin(), _activeConnection.end(), p_client), _activeConnection.end());
				_acceptedConnection.erase(
					std::remove(_acceptedConnection.begin(), _acceptedConnection.end(), p_client), _acceptedConnection.end());
			}
		}

		void sendToAll(const jgl::Message<TServerMessageEnum>& p_msg, ServerConnection* p_ignore = nullptr)
		{
			jgl::Bool error = false;

			for (auto& client : _activeConnection)
			{
				if (client != nullptr && client != p_ignore)
				{
					sendTo(client, p_msg);
				}
			}

			if (error == true)
			{
				_activeConnection.erase(std::remove(_activeConnection.begin(), _activeConnection.end(), nullptr), _activeConnection.end());
				_acceptedConnection.erase(std::remove(_acceptedConnection.begin(), _acceptedConnection.end(), nullptr), _acceptedConnection.end());
			}
		}

		void sendToArray(const jgl::Message<TServerMessageEnum>& p_msg, const std::vector<ServerConnection*>& p_list)
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