#pragma once

#include "Structure/jgl2_network.h"
#include "Structure/Widget/jgl2_server_manager.h"
#include "Structure/Widget/jgl2_no_graphics.h"

namespace jgl
{
	namespace Widget
	{
		template <typename TServerMessageEnum>
		class NodeManager : public jgl::Abstract::Widget::NoGraphics
		{
		public:
			using Server = jgl::Network::Server<TServerMessageEnum>;
			using ServerManager = jgl::Widget::ServerManager<TServerMessageEnum>;
			using NodeHandler = jgl::Abstract::Network::NodeHandler<TServerMessageEnum>;
			using Message = jgl::Network::Message<TServerMessageEnum>;
			using Connection = jgl::Network::Connection<TServerMessageEnum>;

		private:
			Server* _inputServer;
			ServerManager* _inputServerManager;
			std::vector<NodeHandler*> _nodeHandlers;

			jgl::Bool _onUpdate()
			{
				for (jgl::Size_t i = 0; i < _nodeHandlers.size(); i++)
				{
					if (_nodeHandlers[i] != nullptr)
					{
						auto& tmpAwnserArray = _nodeHandlers[i]->awnserReady();
						while (tmpAwnserArray.empty() == false)
						{
							Message awnser = tmpAwnserArray.pop_front();

							jgl::Long clientID = awnser.header.emiterID;

							Connection* connection = _inputServer->connection(clientID);
							connection->send(awnser);
						}
					}
				}

				return (false);
			}

		public:
			NodeManager(std::string p_name, jgl::Size_t p_serverPort) : jgl::Abstract::Widget::NoGraphics(p_name)
			{
				_inputServer = new Server(p_serverPort);
				_inputServer->setUnknowMessageActivityFunction([&](jgl::Network::Connection<TServerMessageEnum>* p_connection, jgl::Network::Message<TServerMessageEnum>& p_msg) {

					jgl::Size_t nodeId = p_msg.header.sparedSpace[jgl::Network::Node<TServerMessageEnum>::nodeIDByte];

				p_msg.header.emiterID = p_connection->id();

				_nodeHandlers[nodeId]->emitMessage(p_msg);
					});
				_inputServer->start();

				_inputServerManager = jgl::Abstract::Application::Core::instance()->addRootWidget<ServerManager>("Central node manager");
				_inputServerManager->setServer(_inputServer);
				_inputServerManager->activate();
			}

			void setNode(NodeHandler* p_node)
			{
				if (_nodeHandlers.size() < static_cast<jgl::Size_t>(p_node->id() + 1))
					_nodeHandlers.resize(p_node->id() + 1);
				if (_nodeHandlers[p_node->id()] != nullptr)
				{
					jgl::cerr << "Node id [" << p_node->id() << "] already set" << std::endl;
				}
				else
				{
					_nodeHandlers[p_node->id()] = p_node;
				}
			}
		};
	}
}