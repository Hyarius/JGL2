#pragma once

#include "jgl2_enums.h"
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
			Color _textColor = Color(255, 255, 255);
			Color _outlineColor = Color(0, 0, 0);

			Size_t _textSize;
			Size_t _textOutlineSize;

			Vector2Int _anchor = Vector2Int(0, 0);
			Vector2Int _size = Vector2Int(0, 0);
			VerticalAlignment _verticalAlignment = VerticalAlignment::Top;
			HorizontalAlignment _horizontalAlignment = HorizontalAlignment::Left;

			Vector2Int _textAnchor = Vector2Int(0, 0);

			Bool _computedTextSize;
			Bool _computedTextOffset;

			void _computeTextSize(jgl::Font* p_font);
			void _computeTextOffset(jgl::Font* p_font);

		public:
			TextLabel();

			void render(jgl::Float p_depthDelta);
			void setFont(jgl::Font* p_font);
			void setText(const std::string& p_text);
			void setTextSize(const Size_t& p_textSize);
			void setTextOutlineSize(const jgl::Size_t& p_textOutlineSize);
			void setColor(const Color& p_textColor, const Color& p_outlineColor);
			void setGeometry(const Vector2Int& p_anchor, const Vector2Int& p_size);
			void setVerticalAlignment(const VerticalAlignment& p_alignement);
			void setHorizontalAlignment(const HorizontalAlignment& p_alignement);

			const std::string& text() const;
			jgl::Font* font() const;
			const Size_t& textSize() const;
			const Color& textColor() const;
			const Color& outlineColor() const;
			const Vector2Int& anchor() const;
			const Vector2Int& size() const;
		};
	}
}