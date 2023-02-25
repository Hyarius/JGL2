#pragma once

#include "Structure/Widget/jgl2_widget_core.h"

namespace jgl
{
	class ConsoleWidget : public jgl::Abstract::Widget::Core
	{
	private:
		void _onRender()
		{

		}

		void _onGeometryChange()
		{

		}

	public:
		ConsoleWidget(std::string p_name) : jgl::Abstract::Widget::Core(p_name)
		{

		}
	};
}