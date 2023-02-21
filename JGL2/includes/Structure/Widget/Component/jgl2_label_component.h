#pragma once

#include "Structure/jgl2_color.h"
#include "Structure/jgl2_vector2.h"
#include "Structure/jgl2_shader.h"
#include "Structure/jgl2_font.h"

#include "Structure/jgl2_monitored_value.h"

#include "Structure/Application/Graphical/jgl2_application.h"

namespace jgl
{
	struct LabelDefaultValues
	{
	public:
		static jgl::Color textColor;
		static jgl::Color outlineColor;
	};

	class Label
	{
	private:
		jgl::Size_t _textSize;
		jgl::Size_t _outlineSize;
		std::string _text;

		jgl::MonitoredValue<jgl::Font*> _font;
		jgl::MonitoredValue<jgl::Color> _textColor;
		jgl::MonitoredValue<jgl::Color> _outlineColor;

		jgl::Vector2Int _anchor;
		jgl::Vector2Int _size;
		jgl::Float _depth;

	public:
		Label();
		void reset();
		void useValue();
		void setText(std::string p_text);
		void setTextSize(jgl::Size_t p_textSize);
		void setOutlineSize(jgl::Size_t p_outlineSize);
		void setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size);
		void setDepth(jgl::Float p_depth);
		void render();

		const std::string& text() const { return (_text); }
		const jgl::Size_t& textSize() const { return (_textSize); }
		const jgl::Size_t& outlineSize() const { return (_outlineSize); }

		jgl::Font* const& font() const { return (_font.value()); }
		const jgl::Color& textColor() const { return (_textColor.value()); }
		const jgl::Color& outlineColor() const { return (_outlineColor.value()); }

		const jgl::Vector2Int& anchor() const { return (_anchor); }
		const jgl::Vector2Int& size() const { return (_size); }
		const jgl::Float& depth() const { return (_depth); }
	};
}