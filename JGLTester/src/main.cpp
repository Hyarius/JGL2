#include "MainMenu.h"

class ServerManager : public jgl::Widget
{
private:
	jgl::Server<int>* _server;
	jgl::Connection<int>* _connection;

	jgl::Bool _onUpdate();
	void _onRender();
	void _onGeometryChange();

public:
	ServerManager(jgl::Widget* p_parent);
};

void ServerManager::_onRender()
{

}

void ServerManager::_onGeometryChange()
{

}

jgl::Bool ServerManager::_onUpdate()
{
	if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Q) == jgl::InputStatus::Released)
	{
		if (_connection != nullptr)
			_connection->send(jgl::Message<int>(0));
	}

	_server->update();
	return (false);
}

ServerManager::ServerManager(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_server = new jgl::Server<int>(26500);

	_server->addActivity(0, [&](jgl::Connection<int>* p_connection, jgl::Message<int>& p_msg) {
			jgl::cout << "Server received message ID [0]" << jgl::endl;
			p_connection->send(jgl::Message<int>(0));
		});

	_server->addActivity(1, [&](jgl::Connection<int>* p_connection, jgl::Message<int>& p_msg) {
			jgl::cout << "Server received message ID [1]" << jgl::endl;
			p_connection->send(jgl::Message<int>(1));
		});

	_server->addActivity(2, [&](jgl::Connection<int>* p_connection, jgl::Message<int>& p_msg) {
			jgl::cout << "Server received message ID [2]" << jgl::endl;
			p_connection->send(jgl::Message<int>(2));
		});

	_server->setLoginFunction([&](jgl::Connection<int>* p_connection) {
			jgl::cout << "Connection accepted, login function started for client id [" << p_connection->id() << "]" << jgl::endl;
			_connection = p_connection;
			if (_connection != nullptr)
				_connection->send(jgl::Message<int>(0));
		});

	_server->start();
}

class ClientManager : public jgl::Widget
{
private:
	jgl::Client<int>* _client;

	jgl::Bool _onUpdate();
	void _onRender();
	void _onGeometryChange();

public:
	ClientManager(jgl::Widget* p_parent);
};

void ClientManager::_onRender()
{

}

void ClientManager::_onGeometryChange()
{

}

jgl::Bool ClientManager::_onUpdate()
{
	_client->update();
	return (false);
}

ClientManager::ClientManager(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_client = new jgl::Client<int>();

	_client->connect("localhost", 26500);

	_client->addActivity(0, [&](jgl::Message<int>& p_msg) {
		jgl::cout << "Client received message ID [0]" << jgl::endl;
		_client->send(jgl::Message<int>(1));
		});

	_client->addActivity(1, [&](jgl::Message<int>& p_msg) {
		jgl::cout << "Client received message ID [1]" << jgl::endl;
		_client->send(jgl::Message<int>(2));
		});

	_client->addActivity(2, [&](jgl::Message<int>& p_msg) {
		jgl::cout << "Client received message ID [2]" << jgl::endl;
		jgl::cout << "End of section" << jgl::endl;
		});
}

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("JGLPositioner", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.activateMultiThread();
	app.setDefaultFont(new jgl::Font("karma suture.ttf"));

	jgl::Observer<Event>::subscribe(Event::A, []() {jgl::cout << "Event A notified" << jgl::endl; });

	MainMenu* mainMenu = new MainMenu(nullptr);
	mainMenu->setGeometry(jgl::Vector2Int(0, 0), app.size());
	mainMenu->activate();

	ServerManager* serverManager = new ServerManager(nullptr);
	serverManager->setGeometry(jgl::Vector2Int(0, 0), app.size());
	serverManager->activate();

	ClientManager* clientManager = new ClientManager(nullptr);
	clientManager->setGeometry(jgl::Vector2Int(0, 0), app.size());
	clientManager->activate();

	return (app.run());
}