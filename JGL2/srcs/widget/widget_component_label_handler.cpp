#include "jgl2.h"

namespace jgl
{
	void W_label_component::_calc_text_size(jgl::Font* tmp_font)
	{
		int delta[5] = { 100, 50, 20, 10, 1 };
		_text_size = 2;

		if (_text == "")
		{
			_text_size = area().y;
			return;
		}

		for (int i = 0; i < 5;i++)
		{
			bool enough = false;
			while (enough == false)
			{
				if (tmp_font == nullptr)
					THROW_EXCEPTION(jgl::Error_level::Error, 1, "No font give to label component");

				jgl::Vector2Int tmp_size = tmp_font->calc_text_size(_text, _text_size + delta[i]);
				if (tmp_size.x >= _area.x || tmp_size.y >= _area.y)
					enough = true;
				else
					_text_size += delta[i];
			}
		}
	}
	void W_label_component::_calc_text_pos_horizontal(jgl::Vector2Int& tmp_size)
	{
		if (_Halign == jgl::Horizontal_alignment::Left)
		{
			_text_pos.x = 0;
		}
		else if (_Halign == jgl::Horizontal_alignment::Centred)
		{
			_text_pos.x = (_area.x - tmp_size.x) / 2;
		}
		else
		{
			_text_pos.x = _area.x - tmp_size.x;
		}
	}
	void W_label_component::_calc_text_pos_vertical(jgl::Vector2Int& tmp_size)
	{
		if (_Valign == jgl::Vertital_alignment::Top)
		{
			_text_pos.y = 0;
		}
		else if (_Valign == jgl::Vertital_alignment::Centred)
		{
			_text_pos.y = (_area.y - tmp_size.y) / 2;
		}
		else
		{
			_text_pos.y = _area.y - tmp_size.y;
		}
	}
	void W_label_component::_calc_text_pos(jgl::Font *tmp_font)
	{
		_text_pos = 0;

		jgl::Vector2Int tmp_size = tmp_font->calc_text_size(_text, _text_size);
		_calc_text_pos_horizontal(tmp_size);
		_calc_text_pos_vertical(tmp_size);
	}
	void W_label_component::_recalc()
	{
		jgl::Font* tmp_font = _font;
		if (tmp_font == nullptr)
			tmp_font = jgl::Application::active_application()->default_font();

		_calc_text_size(tmp_font);
		_calc_text_pos(tmp_font);

		_computed_font = tmp_font;
	}

	W_label_component::W_label_component(jgl::Widget* p_owner, jgl::String p_text) : W_graphical_component(p_owner)
	{
		_text = p_text;
		_Halign = jgl::Horizontal_alignment::Left;
		_Valign = jgl::Vertital_alignment::Top;

		_text_color = jgl::Color(50, 50, 50);
		_text_outline_color = jgl::Color(0, 0, 0);
	}
	void W_label_component::set_text_alone(jgl::String p_text)
	{
		THROW_INFORMATION("Set text alone to " + p_text);
		_text = p_text;
	}
	void W_label_component::set_text(jgl::String p_text)
	{
		THROW_INFORMATION("Set text to " + p_text);
		_text = p_text;
		_recalc();
	}

	void W_label_component::set_geometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth)
	{
		W_graphical_component::set_geometry(p_anchor, p_area, p_depth);
		_recalc();
	}

	void W_label_component::render()
	{
		jgl::Font* tmp_font = _font;
		if (tmp_font == nullptr)
			tmp_font = jgl::Application::active_application()->default_font();

		if (tmp_font != _computed_font)
			_recalc();

		_computed_font->draw_text(_text, _anchor + _text_pos, _text_size, _depth, 1.0f, _text_color, _text_outline_color);
	}
}