#pragma once

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