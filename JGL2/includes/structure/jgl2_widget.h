#pragma once

#include "Structure/jgl2_string.h"

namespace jgl
{
	namespace Abstract
	{
		class Widget
		{
		private:
			jgl::String _widgetName;
			Widget* _parent = nullptr;

			void _addChildren(Widget* p_parent);
			void _removeChildren(Widget* p_parent);

			virtual jgl::Bool _onUpdate() = 0;
			virtual void _onRender() = 0;

		public:
			Widget(jgl::String p_widgetName, Widget* p_parent);
			Widget(Widget* p_parent);

			void setParent(Widget* p_parent);

			jgl::Bool update();
			void render();
		};
	}
}