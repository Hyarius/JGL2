#pragma once

#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_color.h"
#include "Structure/jgl2_font.h"

namespace jgl
{
	namespace WidgetComponent
	{
		class TextLabel
		{
		private:
			Font* _font;
			std::string _text;
			Color _textColor;
			Color _outlineColor;

			Size_t _textSize;
			Size_t _textOutlineSize;

			Vector2Int _anchor = Vector2Int(0, 0);
			Vector2Int _size = Vector2Int(0, 0);

			Vector2Int _textAnchor = Vector2Int(0, 0);

			Bool _computed;

			void _computeTextSize(jgl::Font* p_font);

		public:
			TextLabel();

			void render(jgl::Float p_depthDelta);
			void setText(std::string p_text);
			void setTextOutlineSize(jgl::Size_t p_textOutlineSize);
			void setColor(Color p_textColor, Color p_outlineColor);
			void setGeometry(Vector2Int p_anchor, Vector2Int p_size);

			const std::string& text() const;
			const Size_t& textSize() const;
			const Color& textColor() const;
			const Color& outlineColor() const;
			const Vector2Int& anchor() const;
			const Vector2Int& size() const;
		};
	}
}