#pragma once

#include "widget/jgl2_widget.h"
#include "structure/jgl2_string.h"
#include "structure/jgl2_vector.h"
#include "structure/jgl2_font.h"

#include "widget/jgl2_widget_component.h"

#include "opengl/jgl2_application.h"

namespace jgl
{
	class Text_entry : public jgl::Widget
	{
	protected:
		jgl::String _text;

		jgl::Bool _is_selected = false;
		jgl::Size_t _cursor_abs_pos = 0;
		jgl::Size_t _low_cursor_pos = 0;
		jgl::Size_t _high_cursor_pos = 0;
		jgl::Vector2Int _cursor_pos = 0;

		jgl::W_box_component* _box = nullptr;
		jgl::W_label_component* _label = nullptr;

		void _on_geometry_change(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth)
		{
			_box->set_geometry(anchor(), area(), depth());
			_label->set_geometry(anchor() + _box->border_size() * 2, area() - _box->border_size() * 4, depth());
			_calc_optimal_text();
			_cursor_pos = _calc_cursor_pos();
		}

		bool _fixed_update()
		{
			if (jgl::Application::active_application()->mouse().get_button(jgl::Mouse_button::Left) == jgl::Input_status::Down)
			{
				if (_box->is_pointed() == true)
				{
					select();
					return (true);
				}
				else
				{
					unselect();
				}
			}
			if (_is_selected == true)
			{
				static jgl::Ulong next_input = jgl::Application::active_application()->time();
				static const jgl::Ulong delta_time = 100;
				jgl::Ulong actual_time = jgl::Application::active_application()->time();

				if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Left_arrow) == jgl::Input_status::Down && next_input <= actual_time)
				{
					if (_cursor_abs_pos > 0)
					{
						_cursor_abs_pos--;
						if (_cursor_abs_pos < _low_cursor_pos)
						{
							_low_cursor_pos--;
							_update_text();
						}
						_cursor_pos = _calc_cursor_pos();
						next_input = actual_time + delta_time;
					}
				}
				if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Right_arrow) == jgl::Input_status::Down && next_input <= actual_time)
				{
					if (_cursor_abs_pos < _text.size())
					{
						_cursor_abs_pos++;
						if (_cursor_abs_pos > _high_cursor_pos)
							_calc_optimal_text();
						_cursor_pos = _calc_cursor_pos();
						next_input = actual_time + delta_time;
					}
				}
				if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Backspace) == jgl::Input_status::Down && _cursor_abs_pos != 0 && next_input <= actual_time)
				{
					_cursor_abs_pos--;
					_text.erase(_cursor_abs_pos);
					if (_cursor_abs_pos == _low_cursor_pos)
						_low_cursor_pos--;
					//_calc_optimal_text();
					_update_text();
					_cursor_pos = _calc_cursor_pos();
					next_input = actual_time + delta_time / 2;
				}
				if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Delete) == jgl::Input_status::Down && _cursor_abs_pos < _text.size() && next_input <= actual_time)
				{
					_text.erase(_cursor_abs_pos);
					_high_cursor_pos--;
					_update_text();
					_cursor_pos = _calc_cursor_pos();
					next_input = actual_time + delta_time / 2;
				}
			}
			return (false);
		}

		bool _update()
		{
			if (_is_selected == true)
			{
				if (jgl::Application::active_application()->keyboard().get_text_entry() != '\0')
				{
					if (jgl::Application::active_application()->keyboard().get_text_entry().is_printable() == true)
					{
						_text.insert(jgl::Application::active_application()->keyboard().get_text_entry(), _cursor_abs_pos);
						_cursor_abs_pos++;
						if (_cursor_abs_pos > _high_cursor_pos)
							_calc_optimal_text(); 
						else
							_update_text();
						_cursor_pos = _calc_cursor_pos();
						return (true);
					}
				}
			}
			return (false);
		}

		void _calc_optimal_text()
		{
			jgl::Font* font = _label->font();

			jgl::Vector2Int size = 0;
			jgl::String to_draw = "";

			if (_cursor_abs_pos != _text.size())
				to_draw.push_back(_text[_cursor_abs_pos]);

			_low_cursor_pos = _cursor_abs_pos;
			for (jgl::Size_t i = _cursor_abs_pos; i > 0 && size.x < _label->area().x; i--)
			{
				size += font->calc_char_offset(_text[i - 1], _label->area().y);
				if (size.x < _label->area().x)
				{
					to_draw.push_front(_text[i - 1]);
					_low_cursor_pos--;
				}
			}

			_high_cursor_pos = _cursor_abs_pos;
			for (jgl::Size_t i = _cursor_abs_pos + 1; i < _text.size() && size.x < _label->area().x; i++)
			{
				size += font->calc_char_offset(_text[i], _label->area().y);
				if (size.x < _label->area().x)
				{
					to_draw.push_back(_text[i]);
					_high_cursor_pos++;
				}
			}

			_label->set_text(to_draw);
		}

		void _update_text()
		{
			jgl::Font* font = _label->font();

			jgl::Vector2Int size = 0;
			jgl::String to_draw = "";

			for (jgl::Size_t i = _low_cursor_pos; i < _cursor_abs_pos; i++)
			{
				size += font->calc_char_offset(_text[i], _label->area().y);
				to_draw.push_back(_text[i]);
			}

			_high_cursor_pos = _cursor_abs_pos;
			for (jgl::Size_t i = _cursor_abs_pos; i < _text.size() && size.x < _label->area().x; i++)
			{
				size += font->calc_char_offset(_text[i], _label->area().y);
				if (size.x < _label->area().x)
				{
					to_draw.push_back(_text[i]);
					_high_cursor_pos++;
				}
			}

			if (to_draw != "")
				_label->set_text(to_draw);
			else
				_label->set_text_alone(to_draw);
		}

		jgl::Vector2Int _calc_cursor_pos()
		{
			jgl::Vector2Int result = 0;
			jgl::Vector2Int tmp = 0;

			for (jgl::Size_t i = _low_cursor_pos; i < _cursor_abs_pos; i++)
			{
				result.x += _label->font()->calc_char_offset(_text[i], _label->text_size()).x;
			}
			return (result);
		}

		void _render()
		{
			_box->render();
			_label->render();
			if (_is_selected == true)
			{
				if (((jgl::Application::active_application()->time() / 500) % 2) == 0)
					jgl::draw_rectangle_color(_label->text_pos() + _box->border_size() * 2 + _cursor_pos, jgl::Vector2Int(3, _label->area().y), jgl::Color(0, 0, 0), depth() + 1, _viewport);
			}
		}

	public:
		Text_entry(jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
		{
			_text = "";
			_box = new jgl::W_box_component(this);
			_label = new jgl::W_label_component(this, "");
			_label->set_vertical_align(jgl::Vertital_alignment::Down);
		}

		jgl::W_box_component* box() { return (_box); }
		jgl::W_label_component* label() { return (_label); }

		void select()
		{
			_is_selected = true;
		}

		void unselect()
		{
			_is_selected = false;
		}
	};
}