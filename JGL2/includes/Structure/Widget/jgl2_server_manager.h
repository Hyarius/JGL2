#pragma once

#include "Structure/jgl2_network.h"
#include "Structure/Widget/jgl2_no_graphics.h"

namespace jgl
{
	namespace Widget
	{
		template <typename TServerMessageEnum>
		class ServerManager : public jgl::Abstract::Widget::NoGraphics
		{
		private:
			jgl::Network::Server< TServerMessageEnum>* _server = nullptr;

			jgl::Bool _onUpdate()
			{
				if (_server != nullptr)
					_server->update();
				return (false);
			}

		public:
			ServerManager(std::string p_name) : jgl::Abstract::Widget::NoGraphics(p_name)
			{

			}
			void setServer(jgl::Network::Server< TServerMessageEnum>* p_server)
			{
				_server = p_server;
			}
			jgl::Network::Server< TServerMessageEnum>* server() {
				return (_server);
			}
		};
	}
}