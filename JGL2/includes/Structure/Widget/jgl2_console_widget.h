#pragma once

#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	class ConsoleWidget : public jgl::Widget
	{
	private:
		void _onRender()
		{

		}
		
		void _onGeometryChange()
		{

		}

	public:
		ConsoleWidget(jgl::Widget* p_parent) : jgl::Widget(p_parent)
		{

		}
		ConsoleWidget(std::string p_name, jgl::Widget* p_parent) : jgl::Widget(p_name, p_parent)
		{

		}
	};
}