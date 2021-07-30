#include "jgl2.h"

namespace jgl
{
	Widget::Widget(jgl::Widget* p_parent)
	{
		_name = "Unamed widget";
		
		set_parent(p_parent);

		_viewport = new jgl::Viewport(this);
		_viewport->set_geometry(0, 0);
	}
	
	bool Widget::_fixed_update()
	{
		return (false);
	}

	bool Widget::_update()
	{
		return (false);
	}
	
	void Widget::set_parent(jgl::Widget* p_parent)
	{
		if (_parent != nullptr)
			_parent->_remove_children(this);

		_parent = (p_parent == nullptr ? (jgl::Application::active_application() == nullptr ? nullptr : jgl::Application::active_application()->_central_widget) : p_parent);
		_depth = (_parent == nullptr ? 1 : _parent->_depth + 1);
		THROW_INFORMATION("Depth set to " + jgl::itoa(_depth));
		if (_parent != nullptr)
			_parent->_add_children(this);
	}
	
	void Widget::_render_children()
	{
		if (_activate == true)
		{
			_render();

			for (jgl::Size_t i = 0; i < _childrens.size(); i++)
			{
				_viewport->use();
				_childrens[i]->_render_children();
			}
		}
	}
	jgl::Bool Widget::_update_children()
	{
		if (_activate == true && _frozen == false)
		{
			for (jgl::Size_t i = 0; i < _childrens.size(); i++)
			{
				_viewport->use();
				if (_childrens[i]->_update_children() == true)
					return (true);
			}

			if (_update() == true)
				return (true);
		}
		return (false);
	}
	jgl::Bool Widget::_fixed_update_children()
	{
		if (_activate == true && _frozen == false)
		{
			for (jgl::Size_t i = 0; i < _childrens.size(); i++)
			{
				_viewport->use();
				if (_childrens[i]->_fixed_update_children() == true)
					return (true);
			}

			if (_fixed_update() == true)
				return (true);
		}
		return (false);
	}
	void Widget::_resize_children(jgl::Vector2 ratio)
	{
		for (jgl::Size_t i = 0; i < _childrens.size(); i++)
		{
			_childrens[i]->_resize_children(ratio);
		}
		_resize(ratio);
	}
}