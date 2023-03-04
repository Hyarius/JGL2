#include "Structure/Application/Console/jgl2_console_application.h"

namespace jgl::Application
{
	void Console::_setupJobs()
	{
		_addJob([&]() { _nbUpdate++; return (0); });
		_addJob([&]() { _updateWidget(); return (0); });
	}

	Console::Console() : jgl::Abstract::Application::Core()
	{
		EventManager::instance()->subscribe(Event::OnSecondSwap, [&]() {
				_nbUpdatePerSecond = _nbUpdate;
				_nbUpdate = 0;
			});
	}
}