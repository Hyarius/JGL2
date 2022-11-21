#include "MainMenu.h"

class ServerManager : public jgl::Widget
{
private:
	jgl::Server<int>* _server;

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
	_server->update();
	return (false);
}

ServerManager::ServerManager(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_server = new jgl::Server<int>(26500);
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
}

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::Application app = jgl::Application("JGLPositioner", jgl::Vector2Int(800, 800), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("karma suture.ttf"));

	jgl::Observer<Event>::subscribe(Event::A, []() {jgl::cout << "Event A notified" << jgl::endl; });

	MainMenu* mainMenu = new MainMenu(nullptr);
	mainMenu->setGeometry(jgl::Vector2Int(0, 0), app.size());
	mainMenu->activate();

	ServerManager* serverManager = new ServerManager(nullptr);
	serverManager->setGeometry(jgl::Vector2Int(0, 0), app.size());
	serverManager->activate();

	return (app.run());
}