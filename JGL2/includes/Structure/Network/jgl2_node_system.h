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
			using Message = jgl::Network::Message<TServerMessageEnum>;
			static const jgl::Size_t nodeIDByte = 0;

		private:
			virtual Message _treatMessage(Message& p_msg) = 0;

		public:
			Message treatMessage(Message& p_msg)
			{
				Message awnser = _treatMessage(p_msg);

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
			public:
				using Message = jgl::Network::Message<TServerMessageEnum>;
			protected:
				jgl::Long _id;
				jgl::LockedQueue<Message> _awnserReady;

			public:
				NodeHandler(jgl::Long p_id) :
					_id(p_id)
				{

				}

				const jgl::Long& id() const { return (_id); }

				jgl::LockedQueue<Message>& awnserReady() { return (_awnserReady); }
				virtual void emitMessage(Message& p_msg) = 0;
			};
		}
	}

	namespace Network
	{
		template <typename TServerMessageEnum>
		class SinglethreadNodeHandler : public jgl::Abstract::Network::NodeHandler<TServerMessageEnum>
		{
		public:
			using Node = jgl::Network::Node<TServerMessageEnum>;
			using Message = jgl::Network::Message<TServerMessageEnum>;

		private:
			Node* _node;

		public:
			SinglethreadNodeHandler(jgl::Long p_id) : jgl::Abstract::Network::NodeHandler<TServerMessageEnum>(p_id),
				_node(nullptr)
			{

			}

			void setNode(Node* p_node)
			{
				_node = p_node;
			}

			void emitMessage(Message& p_msg)
			{
				this->_awnserReady.push_back(_node->treatMessage(p_msg));
			}
		};

		template <typename TServerMessageEnum>
		class MultithreadNodeHandler : public jgl::Abstract::Network::NodeHandler<TServerMessageEnum>
		{
		public:
			using Node = jgl::Network::Node<TServerMessageEnum>;
			using Message = jgl::Network::Message<TServerMessageEnum>;

		private:
			Node* _node;
			jgl::WorkerPool _workerPool;

			std::function<void(Message& p_msg)> _emitMessageLambda = nullptr;

		public:
			MultithreadNodeHandler(jgl::Long p_id, std::string p_nodeName, jgl::Size_t p_nbThread = 1) :
				jgl::Abstract::Network::NodeHandler<TServerMessageEnum>(p_id),
				_node(nullptr),
				_workerPool(p_nodeName, p_nbThread)
			{
				_emitMessageLambda = [&](Message& p_msg) {
					this->_awnserReady.push_back(_node->treatMessage(p_msg));
				};
			}

			void setNode(Node* p_node)
			{
				_node = p_node;
			}

			void emitMessage(Message& p_msg)
			{
				_workerPool.addJob(_emitMessageLambda, p_msg);
			}
		};

		template <typename TServerMessageEnum>
		class OnlineNodeHandler : public jgl::Abstract::Network::NodeHandler<TServerMessageEnum>
		{
		public:
			using Client = jgl::Network::Client<TServerMessageEnum>;
			using ClientManager = jgl::Widget::ClientManager<TServerMessageEnum>;
			using Message = jgl::Network::Message<TServerMessageEnum>;
			using Node = jgl::Network::Node<TServerMessageEnum>;

		private:
			Client* _nodeClient;
			ClientManager* _nodeClientManager;

		public:
			OnlineNodeHandler(jgl::Long p_id, std::string p_nodeName, std::string p_address, jgl::Int p_serverPort) :
				jgl::Abstract::Network::NodeHandler<TServerMessageEnum>(p_id)
			{
				_nodeClient = new Client();
				_nodeClient->connect(p_address, p_serverPort);
				_nodeClient->setUnknowMessageActivityFunction([&](Message& p_msg) {
					this->_awnserReady.push_back(p_msg);
					});

				_nodeClientManager = jgl::Abstract::Application::Core::instance()->addRootWidget<ClientManager>(p_nodeName + " handler");
				_nodeClientManager->setClient(_nodeClient);
				_nodeClientManager->activate();
			}

			void emitMessage(Message& p_msg)
			{
				p_msg.header.emiterID = _nodeClient->connection()->id();
				p_msg.header.sparedSpace[Node::nodeIDByte] = this->_id;
				_nodeClient->send(p_msg);
			}
		};
	}
}