#pragma once

#include "widget/jgl2_widget.h"
#include "widget/jgl2_widget_component.h"

namespace jgl
{
	class Frame : public jgl::Widget
	{
	private:
		jgl::W_box_component* _box;

		virtual void _on_geometry_change(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth)
		{
			_box->set_geometry(anchor(), area(), depth());
		}

		virtual void _render()
		{
			_box->render();
		}

	public:
		Frame(jgl::Color p_color = jgl::Color(200, 200, 200), jgl::Color p_border_color = jgl::Color(150, 150, 150), jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
		{
			_box = new jgl::W_box_component(this, p_color, p_border_color);
		}
		Frame(jgl::Widget* p_parent) : jgl::Widget(p_parent)
		{
			_box = new jgl::W_box_component(this);
		}

		W_box_component* box() { return (_box); }
	};
}