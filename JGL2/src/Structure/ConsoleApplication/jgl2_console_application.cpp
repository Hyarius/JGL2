#include "Structure/jgl2_console_application.h"

namespace jgl
{
	ConsoleApplication::ConsoleApplication() : AbstractApplication()
	{

	}

	Int ConsoleApplication::run()
	{
		_updateTime();

		jgl::ULong actualNbFrame = 0;
		jgl::ULong nextTime = _time;

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