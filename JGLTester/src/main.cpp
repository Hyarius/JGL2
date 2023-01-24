#include "jgl2.h"
class ConnectionMenu : public jgl::Widget
{
private:
	jgl::Frame* _backgroundFrame;
	jgl::TextEntry* _usernameEntry;
	jgl::TextEntry* _serverAddress;
	jgl::Button* _connectButton;
	jgl::Button* _hostButton;

	jgl::Bool _onUpdate()
	{
		return (false);
	}

	void _onRender()
	{

	}

	void _onGeometryChange()
	{
		_backgroundFrame->setGeometry(jgl::Vector2Int(30, 30), jgl::Vector2Int(500, 220));

		_usernameEntry->setGeometry(jgl::Vector2Int(20, 20), jgl::Vector2Int(300, 50));

		_serverAddress->setGeometry(jgl::Vector2Int(20, 80), jgl::Vector2Int(300, 50));

		_connectButton->setGeometry(jgl::Vector2Int(20, 150), jgl::Vector2Int(145, 50));
		_hostButton->setGeometry(jgl::Vector2Int(195, 150), jgl::Vector2Int(145, 50));
	}

public:
	ConnectionMenu(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		_backgroundFrame = new jgl::Frame(this);
		_backgroundFrame->setName("BackgroundFrame");
		_backgroundFrame->activate();

		_usernameEntry = new jgl::TextEntry(_backgroundFrame);
		_usernameEntry->setName("UsernameEntry");
		_usernameEntry->setText("Username");
		_usernameEntry->activate();

		_serverAddress = new jgl::TextEntry(_backgroundFrame);
		_serverAddress->setName("ServerAddressEntry");
		_serverAddress->setText("ServerAddress");
		_serverAddress->activate();

		_connectButton = new jgl::Button([&]() {}, _backgroundFrame);
		_connectButton->setName("ConnectButton");
		_connectButton->setText("Join");
		_connectButton->activate();

		_hostButton = new jgl::Button([&]() {}, _backgroundFrame);
		_hostButton->setName("HostButton");
		_hostButton->setText("Host");
		_hostButton->activate();
	}
};

int main(int argc, char** argv)
{
	jgl::cout.setEncoding("fr-FR");
	jgl::GraphicalApplication app = jgl::GraphicalApplication("CardGame", jgl::Vector2Int(900, 600), jgl::Color(60, 60, 60));
	app.setDefaultFont(new jgl::Font("Sono-Regular.ttf"));

	ConnectionMenu mainApp = ConnectionMenu(nullptr);
	mainApp.setGeometry(jgl::Vector2Int(0, 0), app.size());
	mainApp.activate();

	return (app.run());
}