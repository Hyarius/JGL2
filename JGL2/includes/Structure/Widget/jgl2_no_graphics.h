#pragma once

#include "Structure/Widget/jgl2_widget_core.h"

namespace jgl
{
	namespace Abstract
	{
		namespace Widget
		{
			class NoGraphics : public jgl::Abstract::Widget::Core
			{
			private:
				void _onRender()
				{

				}

				void _onGeometryChange()
				{

				}

			public:
				NoGraphics(std::string p_name) : jgl::Abstract::Widget::Core(p_name)
				{

				}
			};
		}
	}
}