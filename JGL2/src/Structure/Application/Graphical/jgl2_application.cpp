#include "Structure/Application/Graphical/jgl2_application.h"
#include "jgl2_basic_functions.h"
#include "structure/jgl2_vector2.h"

namespace jgl::Application
{
	void Graphical::_setupJobs()
	{
		_addJob([&]() {
				_nbRender++;
				jgl::Size_t actualSecond = (time() / 1000);
				if (actualSecond != _lastSecond)
				{
					_updatePerSecond = _nbUpdate;
					_renderPerSecond = _nbRender;
					_nbUpdate = 0;
					_nbRender = 0;
					for (jgl::Size_t i = 0; i < _functToInvoke.size(); i++)
					{
						_functToInvoke[i]();
					}
					_lastSecond = actualSecond;
				}
				return (0);
			});
		_addJob([&]() { _window.reset(); return (0); });
		_addJob([&]() { _API.pullWinMessage(); return (0); });
		_addJob([&]() { _renderWidget(); return (0); });
		_addJob([&]() { _window.render(); return (0); });

		_addJob("UpdateThread", [&]() { 
				_nbUpdate++;
				return (0);
			});
		_addJob("UpdateThread", [&]() { _window.update(); return (0); });
		_addJob("UpdateThread", [&]() { _system.update(); return (0); });
		_addJob("UpdateThread", [&]() { _mouse.update(); return (0); });
		_addJob("UpdateThread", [&]() { _keyboard.update(); return (0); });
		_addJob("UpdateThread", [&]() { _updateWidget(); return (0); });
	}

	Graphical::Graphical(std::string p_title, jgl::Vector2Int p_size, jgl::Color p_backgroundColor) : jgl::Abstract::Application::Core(),
		_API(),
		_window(_API.windowMessagesToTreat()),
		_mouse(_API.mouseMessagesToTreat()),
		_keyboard(_API.keyboardMessagesToTreat()),
		_system(_API.systemMessagesToTreat()),
		_defaultFont(nullptr)
	{
		_window.connectToAPI(&_API);
		_window.createWindow(p_title, p_size, p_backgroundColor);

		_create2DColorShader();
		_create2DTextureShader();
		_create2DTextTextureShader();
	}
}