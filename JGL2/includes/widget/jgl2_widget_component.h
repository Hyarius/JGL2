#pragma once

#include "jgl2_basic_types.h"
#include "structure/jgl2_vector.h"
#include "structure/jgl2_color.h"

namespace jgl
{
	class W_component
	{
	protected:
		jgl::Widget* _owner;

	public:
		W_component(jgl::Widget* p_owner)
		{
			_owner = p_owner;
		}
	};

	class W_graphical_component : public jgl::W_component
	{
	protected:
		jgl::Vector2Int _anchor = 0;
		jgl::Vector2Int _area = 0;
		jgl::Float _depth = 1;

	public:
		W_graphical_component(jgl::Widget* p_owner);

		jgl::Vector2Int anchor() { return (_anchor); }
		jgl::Vector2Int area() { return (_area); }

		bool is_pointed();

		void set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth);

		void set_depth(jgl::Float p_depth);
	};

	class W_box_component : public W_graphical_component
	{
	protected:
		jgl::Color _color = jgl::Color(200, 200, 200);
		jgl::Color _border_color = jgl::Color(150, 150, 150);

		jgl::Size_t _border_size = 5;

	public:
		W_box_component(jgl::Widget* p_owner, jgl::Color p_color = jgl::Color(200, 200, 200), jgl::Color p_border_color = jgl::Color(150, 150, 150));

		jgl::Color color() { return (_color); }
		jgl::Color border_color() { return (_border_color); }
		jgl::Size_t border_size() { return (_border_size); }

		void set_color(jgl::Color p_color) { _color = p_color; }
		void set_border_color(jgl::Color p_border_color) { _border_color = p_border_color; }
		void set_border_size(jgl::Size_t p_border_size) { _border_size = p_border_size; }

		void render();
	};

	class W_label_component : public W_graphical_component
	{
	protected:
		jgl::Font* _font;
		jgl::Font* _computed_font;

		jgl::String _text;

		jgl::Vector2Int _text_pos;
		jgl::Size_t _text_size;
		jgl::Horizontal_alignment _Halign;
		jgl::Vertital_alignment _Valign;

		jgl::Color _text_color;
		jgl::Color _text_outline_color;


		void _calc_text_size(jgl::Font* tmp_font);
		void _calc_text_pos_horizontal(jgl::Vector2Int& tmp_size);
		void _calc_text_pos_vertical(jgl::Vector2Int& tmp_size);
		void _calc_text_pos(jgl::Font* tmp_font);
		void _recalc();

	public:
		W_label_component(jgl::Widget* p_owner, jgl::String p_text);

		const jgl::String& text() { return (_text); }
		const jgl::Size_t& text_size() { return (_text_size); }
		const jgl::Vector2Int& text_pos() { return (_text_pos); }
		jgl::Font* font()
		{
			jgl::Font* tmp_font = _font;
			if (tmp_font == nullptr)
				tmp_font = jgl::Application::active_application()->default_font(); 
			return (tmp_font);
		}

		void set_font(jgl::Font* p_font) { _font = p_font; _recalc(); }
		void set_text(jgl::String p_text);
		void set_text_alone(jgl::String p_text);
		void set_horizontal_align(jgl::Horizontal_alignment p_align) { _Halign = p_align; _recalc(); }
		void set_vertical_align(jgl::Vertital_alignment p_align) { _Valign = p_align; _recalc(); }
		void set_text_color(jgl::Color p_color) { _text_color = p_color; }
		void set_text_outline_color(jgl::Color p_outline_color) { _text_outline_color = p_outline_color; }

		void set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth);

		void render();
	};
}