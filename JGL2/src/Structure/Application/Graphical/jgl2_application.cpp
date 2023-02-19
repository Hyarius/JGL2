#include "Structure/Application/Graphical/jgl2_application.h"
#include "jgl2_basic_functions.h"
#include "structure/jgl2_vector2.h"

namespace jgl
{
	void Application::_setupJobs()
	{
		_addJob([&]() { _windowManager.clear(); return (0); });
		_addJob([&]() { _APIManager.pullWinMessage(); return (0); });
		_addJob([&]() { _renderWidget(); return (0); });
		_addJob([&]() { _windowManager.render(); return (0); });

		_addJob("UpdateThread", [&]() { _windowManager.update(); return (0); });
		_addJob("UpdateThread", [&]() { _systemManager.update(); return (0); });
		_addJob("UpdateThread", [&]() { _mouseManager.update(); return (0); });
		_addJob("UpdateThread", [&]() { _keyboardManager.update(); return (0); });
		_addJob("UpdateThread", [&]() { _updateWidget(); return (0); });
	}

	Application::Application(std::string p_title, jgl::Vector2Int p_size, jgl::Color p_backgroundColor) : jgl::ApplicationCore(),
		_APIManager(),
		_windowManager(_APIManager.windowMessagesToTreat()),
		_mouseManager(_APIManager.mouseMessagesToTreat()),
		_keyboardManager(_APIManager.keyboardMessagesToTreat()),
		_systemManager(_APIManager.systemMessagesToTreat()),
		_defaultFont(nullptr)
	{
		_windowManager.connectToAPI(&_APIManager);
		_windowManager.createWindow(p_title, p_size, p_backgroundColor);

		_create2DColorShader();
		_create2DTextureShader();
		_create2DTextTextureShader();
	}
}