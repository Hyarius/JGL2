#include "jgl2.h"

enum class Event
{
	A,
	B
};

class MainMenu : public jgl::Widget
{
private:
	jgl::Frame* _backgroundFrame;
	jgl::TextLabel* _gameIconFrame;
	jgl::TextLabel* _usernameLabel;
	jgl::TextEntry* _usernameEntry;
	jgl::TextLabel* _passwordLabel;
	jgl::TextEntry* _passwordEntry;
	jgl::Button* _connectButton;

	jgl::Bool _onUpdate();
	void _onRender();
	void _onGeometryChange();

public:
	MainMenu(jgl::Widget* p_parent);
};

void MainMenu::_onRender()
{

}

void MainMenu::_onGeometryChange()
{
	jgl::Vector2Int backgroundFramePosition = jgl::Vector2Int(10, 10);
	jgl::Vector2Int backgroundFrameSize = jgl::Vector2Int::composeMin(
		jgl::Vector2Int(500, 600),
		jgl::Vector2Int((size().x() - backgroundFramePosition.x() * 2) / 2, (size().y() - backgroundFramePosition.y() * 2) / 2)
	);

	_backgroundFrame->setGeometry(backgroundFramePosition, backgroundFrameSize);

	jgl::Float labelSize = _backgroundFrame->usableSize().y() / 10;
	jgl::Float entrySize = _backgroundFrame->usableSize().y() / 8;
	jgl::Float space = labelSize + entrySize;

	jgl::Vector2Int gameIconFramePosition = jgl::Vector2Int(10, 10);
	jgl::Vector2Int gameIconFrameSize = jgl::Vector2Int(_backgroundFrame->usableSize().x() - gameIconFramePosition.x() * 2, (_backgroundFrame->usableSize().y() - gameIconFramePosition.y() * 2) / 3);

	_gameIconFrame->setGeometry(gameIconFramePosition, gameIconFrameSize);

	jgl::Vector2Int usernameLabelPosition = _gameIconFrame->anchor() + jgl::Vector2Int(0, 10 + _gameIconFrame->size().y());
	jgl::Vector2Int usernameLabelSize = jgl::Vector2Int(_gameIconFrame->size().x() - 20, labelSize);

	_usernameLabel->setGeometry(usernameLabelPosition, usernameLabelSize);

	jgl::Vector2Int usernameEntryPosition = usernameLabelPosition + jgl::Vector2Int(20, labelSize / 4 * 3);
	jgl::Vector2Int usernameEntrySize = jgl::Vector2Int(_gameIconFrame->size().x() - 20, entrySize);

	_usernameEntry->setGeometry(usernameEntryPosition, usernameEntrySize);

	jgl::Vector2Int passwordLabelPosition = usernameLabelPosition + jgl::Vector2Int(0, space);
	jgl::Vector2Int passwordLabelSize = usernameLabelSize;

	_passwordLabel->setGeometry(passwordLabelPosition, passwordLabelSize);

	jgl::Vector2Int passwordEntryPosition = passwordLabelPosition + jgl::Vector2Int(20, labelSize / 4 * 3);
	jgl::Vector2Int passwordEntrySize = usernameEntrySize;

	_passwordEntry->setGeometry(passwordEntryPosition, passwordEntrySize);

	jgl::Vector2Int connectButtonPosition = jgl::Vector2Int((_backgroundFrame->usableSize().x() - 20) / 4, passwordLabelPosition.y() + space);
	jgl::Vector2Int connectButtonSize = jgl::Vector2Int((_backgroundFrame->usableSize().x() - 20) / 2, passwordEntrySize.y());

	_connectButton->setGeometry(connectButtonPosition, connectButtonSize);
}

jgl::Bool MainMenu::_onUpdate()
{
	return (false);
}

MainMenu::MainMenu(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_backgroundFrame = new jgl::Frame(this);
	_backgroundFrame->activate();

	_gameIconFrame = new jgl::TextLabel("ERELIA", _backgroundFrame);
	_gameIconFrame->label().setHorizontalAlignment(jgl::HorizontalAlignment::Centred);
	_gameIconFrame->label().setVerticalAlignment(jgl::VerticalAlignment::Centred);
	_gameIconFrame->activate();

	_usernameLabel = new jgl::TextLabel("Username :", _backgroundFrame);
	_usernameLabel->activate();

	_usernameEntry = new jgl::TextEntry(_backgroundFrame);
	_usernameEntry->setCursorColor(jgl::Color(50, 50, 50));
	_usernameEntry->activate();

	_passwordLabel = new jgl::TextLabel("Password :", _backgroundFrame);
	_passwordLabel->activate();

	_passwordEntry = new jgl::TextEntry(_backgroundFrame);
	_passwordEntry->setCursorColor(jgl::Color(50, 50, 50));
	_passwordEntry->activate();

	_connectButton = new jgl::Button([&]() {
			jgl::Observer<Event>::notify(Event::A);
		}, _backgroundFrame);
	_connectButton->setText("Connect");
	_connectButton->activate();
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

	return (app.run());
}