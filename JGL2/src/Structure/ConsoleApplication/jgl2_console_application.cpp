#include "Structure/jgl2_console_application.h"

namespace jgl
{
	ConsoleApplication::ConsoleApplication() : AbstractApplication()
	{

	}

	Int ConsoleApplication::run()
	{
		_running = true;
		while (_running == true)
		{
			_updateTime();

			for (jgl::Size_t i = 0; i < _widgets.size(); i++)
			{
				_widgets[i]->update();
			}
		}
		return (0);
	}
}