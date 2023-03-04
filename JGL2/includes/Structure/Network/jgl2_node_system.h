#pragma once

#include "Structure/Network/jgl2_message.h"
#include "Structure/Network/jgl2_connection.h"
#include "Structure/Network/jgl2_client.h"
#include "Structure/Network/jgl2_server.h"
#include "Structure/Widget/jgl2_client_manager.h"

namespace jgl
{
	namespace Network
	{
		template <typename TServerMessageEnum>
		class Node
		{
		public:
			static const jgl::Size_t nodeIDByte = 0;
			static const jgl::Size_t connectionIDByte = 0;

		private:
			virtual jgl::Network::Message<TServerMessageEnum> _treatMessage(jgl::Network::Message<TServerMessageEnum>& p_msg) = 0;

		public:
			jgl::Network::Message<TServerMessageEnum> treatMessage(jgl::Network::Message<TServerMessageEnum>& p_msg)
			{
				jgl::Network::Message<TServerMessageEnum> awnser = _treatMessage(p_msg);

				awnser.copyHeaderData(p_msg);

				return (awnser);
			}
		};
	}

	namespace Abstract
	{
		namespace Network
		{
			template <typename TServerMessageEnum>
			class NodeHandler
			{
			protected:
				jgl::Long _id;
				jgl::LockedQueue<jgl::Network::Message<TServerMessageEnum>> _awnserReady;

			public:
				NodeHandler(jgl::Long p_id) : 
					_id(p_id)
				{

				}

				const jgl::Long& id() const {return (_id); }

				jgl::LockedQueue<jgl::Network::Message<TServerMessageEnum>>& awnserReady() { return (_awnserReady); }
				virtual void emitMessage(jgl::Network::Message<TServerMessageEnum>& p_msg) = 0;
			};
		}
	}

	namespace Network
	{
		template <typename TServerMessageEnum>
		class SinglethreadNodeHandler : public jgl::Abstract::Network::NodeHandler<TServerMessageEnum>
		{
		private:
			jgl::Network::Node<TServerMessageEnum>* _node;

		public:
			SinglethreadNodeHandler(jgl::Long p_id) : jgl::Abstract::Network::NodeHandler<TServerMessageEnum>(p_id),
				_node(nullptr)
			{

			}

			void setNode(jgl::Network::Node<TServerMessageEnum>* p_node)
			{
				_node = p_node;
			}

			void emitMessage(jgl::Network::Message<TServerMessageEnum>& p_msg)
			{
				this->_awnserReady.push_back(_node->treatMessage(p_msg));
			}
		};

		template <typename TServerMessageEnum>
		class MultithreadNodeHandler : public jgl::Abstract::Network::NodeHandler<TServerMessageEnum>
		{
		private:
			jgl::Network::Node<TServerMessageEnum>* _node;
			jgl::WorkerPool _workerPool;

			std::function<void(jgl::Network::Message<TServerMessageEnum>& p_msg)> _emitMessageLambda = nullptr;

		public:
			MultithreadNodeHandler(jgl::Long p_id, std::string p_nodeName, jgl::Size_t p_nbThread = 1) : 
				jgl::Abstract::Network::NodeHandler<TServerMessageEnum>(p_id),
				_node(nullptr),
				_workerPool(p_nodeName, p_nbThread)
			{
				_emitMessageLambda = [&](jgl::Network::Message<TServerMessageEnum>& p_msg) {
					this->_awnserReady.push_back(_node->treatMessage(p_msg));
				};
			}

			void setNode(jgl::Network::Node<TServerMessageEnum>* p_node)
			{
				_node = p_node;
			}

			void emitMessage(jgl::Network::Message<TServerMessageEnum>& p_msg)
			{
				_workerPool.addJob(_emitMessageLambda, p_msg);
			}
		};

		template <typename TServerMessageEnum>
		class OnlineNodeHandler : public jgl::Abstract::Network::NodeHandler<TServerMessageEnum>
		{
		private:
			jgl::Network::Client<TServerMessageEnum>* _nodeClient;
			jgl::Widget::ClientManager<TServerMessageEnum>* _nodeClientManager;

		public:
			OnlineNodeHandler(jgl::Long p_id, std::string p_nodeName, std::string p_address, jgl::Int p_serverPort) : 
				jgl::Abstract::Network::NodeHandler<TServerMessageEnum>(p_id)
			{
				_nodeClient = new jgl::Network::Client<TServerMessageEnum>();
				_nodeClient->connect(p_address, p_serverPort);
				_nodeClient->setUnknowMessageActivityFunction([&](jgl::Network::Message<TServerMessageEnum>& p_msg) {
					this->_awnserReady.push_back(p_msg);
					});

				_nodeClientManager = jgl::Abstract::Application::Core::instance()->addRootWidget<jgl::Widget::ClientManager<TServerMessageEnum>>(p_nodeName + " handler");
				_nodeClientManager->setClient(_nodeClient);
				_nodeClientManager->activate();
			}

			void emitMessage(jgl::Network::Message<TServerMessageEnum>& p_msg)
			{
				p_msg.header.emiterID = _nodeClient->connection()->id();
				p_msg.header.sparedSpace[jgl::Network::Node<TServerMessageEnum>::nodeIDByte] = this->_id;
				p_msg.header.sparedSpace[jgl::Network::Node<TServerMessageEnum>::connectionIDByte] = _nodeClient->connection()->id();
				_nodeClient->send(p_msg);
			}
		};
	}
}