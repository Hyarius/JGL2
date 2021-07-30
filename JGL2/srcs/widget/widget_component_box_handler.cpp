#include "jgl2.h"

namespace jgl
{
	W_box_component::W_box_component(jgl::Widget* p_owner, jgl::Color p_color, jgl::Color p_border_color) : W_graphical_component(p_owner)
	{
		_color = p_color;
		_border_color = p_border_color;
	}

	void W_box_component::render()
	{
		jgl::draw_rectangle_color(_anchor + _border_size, _area - _border_size * 2, _color, _depth);
		jgl::draw_rectangle_color(_anchor, _area, _border_color, _depth);
	}
}