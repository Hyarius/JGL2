#pragma once

#include "Structure/jgl2_network.h"
#include "Structure/Widget/jgl2_console_widget.h"

namespace jgl
{
	template <typename TServerMessageEnum>
	class ServerManager : public jgl::ConsoleWidget
	{
	private:
		jgl::Server< TServerMessageEnum>* _server = nullptr;

		jgl::Bool _onUpdate()
		{
			if (_server != nullptr)
				_server->update();
			return (false);
		}

	public:
		ServerManager(jgl::Widget* p_parent) : jgl::ConsoleWidget(p_parent)
		{

		}
		void setServer(jgl::Server< TServerMessageEnum>* p_server)
		{
			_server = p_server;
		}
		jgl::Server< TServerMessageEnum>* server() {
			return (_server);
		}
	};

	template <typename TServerMessageEnum>
	class ClientManager : public jgl::ConsoleWidget
	{
	private:
		jgl::Client< TServerMessageEnum>* _client = nullptr;

		jgl::Bool _onUpdate()
		{
			if (_client != nullptr)
				_client->update();
			return (false);
		}

	public:
		ClientManager(jgl::Widget* p_parent) : jgl::ConsoleWidget(p_parent)
		{

		}
		void setClient(jgl::Client< TServerMessageEnum>* p_client)
		{
			_client = p_client;
		}
		jgl::Client< TServerMessageEnum>* client() {
			return (_client);
		}
	};
}