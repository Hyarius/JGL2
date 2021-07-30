#pragma once

#include "opengl/jgl2_viewport.h"

namespace jgl
{
	class Widget
	{
	protected:
		friend class Application;

		jgl::String _name = "Unamed widget";
		jgl::Bool _activate = false;
		jgl::Bool _frozen = false;
		jgl::Float _depth = 1;
		jgl::Viewport* _viewport = nullptr;

		jgl::Widget* _parent = nullptr;
		jgl::Array<jgl::Widget*> _childrens = {};

		virtual void _on_geometry_change(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth) = 0;
		virtual void _render() = 0;
		virtual bool _update();
		virtual bool _fixed_update();


		void _render_children();
		jgl::Bool _update_children();
		jgl::Bool _fixed_update_children();
		void _resize_children(jgl::Vector2 ratio);

		void _resize(jgl::Vector2 ratio)
		{
			jgl::Vector2 tmp_anchor = anchor();
			jgl::Vector2 tmp_area = area();
			set_geometry(tmp_anchor * ratio, tmp_area * ratio, depth());
		}

		void _add_children(jgl::Widget* p_children)
		{
			_childrens.push_back(p_children);
		}

		void _remove_children(jgl::Widget* p_children)
		{
			_childrens.erase(_childrens.find(p_children));
		}

	public:
		Widget(jgl::Widget* p_parent = nullptr);

		jgl::Bool is_frozen() { return (_frozen); }
		void froze() { _frozen = true; }
		void unfroze() { _frozen = false; }

		jgl::Bool is_active() { return (_activate); }
		void activate() { _activate = true; }
		void desactivate() { _activate = false; }
		
		jgl::Viewport* viewport() { return (_viewport); }

		jgl::Vector2Int cumuled_anchor() { if (_parent == nullptr)return (0); return (_viewport->anchor() + _parent->anchor()); }
		jgl::Vector2Int anchor() { return (_viewport->anchor()); }
		jgl::Vector2Int area() { return (_viewport->area()); }

		void set_depth(jgl::Float p_depth)
		{
			jgl::Float old_depth = _depth;
			_depth = p_depth;
			for (jgl::Size_t i = 0; i < _childrens.size(); i++)
			{
				if (_childrens[i]->depth() == old_depth + 1)
					_childrens[i]->set_depth(_depth + 1);
			}
			_on_geometry_change(anchor(), area(), p_depth);
		}
		jgl::Float depth() { return (_depth); }

		void set_name(jgl::String p_name) { _name = p_name; }
		const jgl::String& name() { return (_name); }

		void set_parent(jgl::Widget* p_parent);
		void set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth = -1)
		{
			if (p_anchor != _viewport->anchor() || p_area != _viewport->area())
			{
				THROW_INFORMATION("Setting [" + _name + "] geometry to : " + p_anchor.text() + " / " + p_area.text());
				_viewport->set_geometry(p_anchor, p_area);
				if (p_depth != -1)
				{
					set_depth(p_depth);
				}
				else
					_on_geometry_change(p_anchor, p_area, p_depth);
			}
		}
	};
}