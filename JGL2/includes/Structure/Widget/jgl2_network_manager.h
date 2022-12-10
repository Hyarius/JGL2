#pragma once

#include "Structure/jgl2_network.h"
#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	template <typename TServerMessageEnum>
	class ServerManager : public jgl::Widget
	{
	private:
		jgl::Server< TServerMessageEnum>* _server = nullptr;

		jgl::Bool _onUpdate()
		{
			if (_server != nullptr)
				_server->update();
			return (false);
		}
		void _onRender() {}
		void _onGeometryChange() {}

	public:
		ServerManager(jgl::Widget* p_parent) : jgl::Widget(p_parent)
		{

		}
		void setServer(jgl::Server< TServerMessageEnum>* p_server)
		{
			_server = p_server;
		}
	};

	template <typename TServerMessageEnum>
	class ClientManager : public jgl::Widget
	{
	private:
		jgl::Client< TServerMessageEnum>* _client = nullptr;

		jgl::Bool _onUpdate()
		{
			if (_client != nullptr)
				_client->update();
			return (false);
		}
		void _onRender() {}
		void _onGeometryChange() {}

	public:
		ClientManager(jgl::Widget* p_parent) : jgl::Widget(p_parent)
		{

		}
		void setClient(jgl::Client< TServerMessageEnum>* p_client)
		{
			_client = p_client;
		}
	};
}