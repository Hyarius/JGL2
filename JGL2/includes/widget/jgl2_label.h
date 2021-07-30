#pragma once

#include "widget/jgl2_widget.h"
#include "widget/jgl2_widget_component.h"

namespace jgl
{
	class Label : public jgl::Widget
	{
	protected:
		jgl::W_box_component* _box;
		jgl::W_label_component* _label;

		virtual void _on_geometry_change(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth)
		{
			_box->set_geometry(anchor(), area(), depth());
			_label->set_geometry(anchor() + _box->border_size() * 2, area() - +_box->border_size() * 4, depth());
		}

		virtual void _render()
		{
			_box->render();
			_label->render();
		}

	public:
		Label(jgl::String p_text, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
		{
			_box = new jgl::W_box_component(this);
			_label = new jgl::W_label_component(this, p_text);
		}
		jgl::W_box_component* box() { return (_box); }
		jgl::W_label_component* label() { return (_label); }
	};
}