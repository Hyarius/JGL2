#include "jgl2.h"

namespace jgl
{
	W_graphical_component::W_graphical_component(jgl::Widget* p_owner) : jgl::W_component(p_owner)
	{

	}

	bool W_graphical_component::is_pointed()
	{
		Vector2 pos1, pos2, mouse;

		pos1 = _owner->cumuled_anchor();

		pos2 = pos1 + _area;

		mouse = jgl::Application::active_application()->mouse().pos();

		if (mouse.x < pos1.x || mouse.x > pos2.x || mouse.y < pos1.y || mouse.y > pos2.y)
			return (false);
		return (true);
	}

	void W_graphical_component::set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth)
	{
		_anchor = p_anchor;
		_area = p_area;
		_depth = p_depth;
	}

	void W_graphical_component::set_depth(jgl::Float p_depth)
	{
		_depth = p_depth;
	}
}