#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

namespace jgl
{
	namespace Abstract
	{
		class Widget
		{
		private:
			std::string _widgetName = "Un-named";
			jgl::Bool _activated = false;

			Widget* _parent = nullptr;
			std::vector<Widget*> _childrens;

			void _addChildren(Widget* p_widget);
			void _removeChildren(Widget* p_widget);

			virtual jgl::Bool _onUpdate() = 0;
			virtual void _onRender() = 0;

		public:
			Widget(std::string p_widgetName, Widget* p_parent);
			Widget(Widget* p_parent);

			void setParent(Widget* p_parent);

			void activate();
			void deactivate();
			void setActivate(jgl::Bool p_state);
			jgl::Bool isActivated();

			jgl::Bool update();
			void render();
		};
	}
}