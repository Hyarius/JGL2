#include "jgl2.h"

namespace jgl
{
	Viewport::Viewport(jgl::Widget* p_owner)
	{
		_owner = p_owner;
		jgl::Uint new_id = 0;
		while (_allocated_id.count(new_id) != 0 && _allocated_id[new_id] == true)
			new_id++;

		_allocated_id[new_id] = true;
		_id = new_id;
	}
	void Viewport::set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area)
	{
		_anchor = p_anchor;
		_area = p_area;
	}
	void Viewport::reset()
	{
		THROW_INFORMATION("Reseting viewport information");
		glViewport(static_cast<jgl::Int>(0), static_cast<jgl::Int>(0), static_cast<jgl::Int>(jgl::Application::active_application()->size().x), static_cast<jgl::Int>(jgl::Application::active_application()->size().y));
	}

	void Viewport::use()
	{
		if (jgl::Application::active_application()->active_viewport() != this)
		{
			if (jgl::Application::active_application()->active_viewport() != nullptr)
			{
				jgl::Application::active_application()->_launch_draw_triangle_color();
				jgl::Application::active_application()->_launch_draw_triangle_texture();
				jgl::Application::active_application()->_launch_draw_triangle_font();
			}
			 
			jgl::Application::active_application()->set_active_viewport(this);

			jgl::Vector2Int tmp = anchor();
			jgl::Vector2Int pos = jgl::Vector2(tmp.x, jgl::Application::active_application()->size().y - _area.y - tmp.y);
			glViewport(static_cast<jgl::Int>(pos.x), static_cast<jgl::Int>(pos.y), static_cast<jgl::Int>(_area.x), static_cast<jgl::Int>(_area.y));
		}
	}
}