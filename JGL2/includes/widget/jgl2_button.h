#pragma once

#include "widget/jgl2_widget.h"
#include "widget/jgl2_widget_component.h"
#include "structure/jgl2_data_contener.h"
#include "opengl/jgl2_application.h"

namespace jgl
{
	class Button : public jgl::Widget
	{
	protected:
		using Button_funct = void(*)(jgl::Data_contener);

		Button_funct _funct = nullptr;
		jgl::Data_contener _param = jgl::Data_contener();

		jgl::Bool _is_selected;
		jgl::W_box_component* _box = nullptr;
		jgl::W_label_component* _label = nullptr;

		void _on_geometry_change(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth)
		{
			_box->set_geometry(anchor(), area(), depth());
			_label->set_geometry(anchor() + _box->border_size() * 2, area() - _box->border_size() * 4, depth());
		}

		void _select()
		{
			_is_selected = true;
			_box->set_color(jgl::Color(150, 150, 150));
			_box->set_border_color(jgl::Color(100, 100, 100));
		}

		void _unselect()
		{
			_is_selected = false;
			_box->set_color(jgl::Color(200, 200, 200));
			_box->set_border_color(jgl::Color(150, 150, 150));
		}

		void _render()
		{
			_box->render();
			_label->render();
		}
		virtual bool _update()
		{
			if (_box->is_pointed() == true && jgl::Application::active_application()->mouse().get_button(jgl::Mouse_button::Left) == jgl::Input_status::Release)
			{
				if (_funct != nullptr)
					_funct(_param);
				return (true);
			}

			if (_is_selected == false && _box->is_pointed() == true && jgl::Application::active_application()->mouse().get_button(jgl::Mouse_button::Left) == jgl::Input_status::Down)
			{
				_select();
			}
			else if (_is_selected == true && (jgl::Application::active_application()->mouse().get_button(jgl::Mouse_button::Left) != jgl::Input_status::Down || _box->is_pointed() == false))
			{
				_unselect();
			}
			return (false);
		}

	public:
		Button(jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
		{
			_box = new jgl::W_box_component(this);
			_label = new jgl::W_label_component(this, "Click me");
		}
		void set_funct(Button_funct p_funct)
		{
			_funct = p_funct;
		}
		void set_param(jgl::Data_contener p_param)
		{
			_param = p_param;
		}
		void set_behavior(Button_funct p_funct, jgl::Data_contener p_param)
		{
			set_funct(p_funct);
			set_param(p_param);
		}
	};
}