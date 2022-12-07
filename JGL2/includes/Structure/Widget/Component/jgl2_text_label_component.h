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
			Bool _initialized = false;
			Shader* _shader = nullptr;
			Buffer* _modelSpaceBuffer = nullptr;
			Buffer* _modelColorBuffer = nullptr;
			Buffer* _modelOutlineColorBuffer = nullptr;
			Buffer* _modelUvBuffer = nullptr;
			Buffer* _indexesBuffer = nullptr;
			Uniform* _textureUniform = nullptr;
			Bool _computed = false;

			Font* _font;
			Font* _selectedFont;
			ULong _selectedTextureID;
			Vector2Int _savedTextSize;
			std::string _text;
			Color _textColor = Color(255, 255, 255);
			Color _outlineColor = Color(0, 0, 0);

			Size_t _textPredefinedSize = 0;
			Size_t _textSize;
			Size_t _textOutlineSize;

			Vector2Int _anchor = Vector2Int(0, 0);
			Vector2Int _size = Vector2Int(0, 0);
			Vector2Int _labelOffset = Vector2Int(0, 0);
			VerticalAlignment _verticalAlignment = VerticalAlignment::Top;
			HorizontalAlignment _horizontalAlignment = HorizontalAlignment::Left;

			Vector2Int _textAnchor = Vector2Int(0, 0);

			Bool _computedTextSize;
			Bool _computedTextOffset;

			void _computeTextSize(Font* p_font);
			void _computeTextOffset(Font* p_font);

			void _initiatizeOpenGL();
			void _computeShaderBuffer(Float p_depth);
			void _castRender();

		public:
			TextLabel();

			Vector2Int render(Float p_depthDelta);

			void setFont(Font* p_font);
			void setText(const std::string& p_text);
			void setTextSize(const Size_t& p_textSize);
			void setTextPredefinedSize(const Size_t& p_textPredefinedSize);
			void setTextOutlineSize(const Size_t& p_textOutlineSize);
			void setColor(const Color& p_textColor, const Color& p_outlineColor);
			void setGeometry(const Vector2Int& p_anchor, const Vector2Int& p_size);
			void setVerticalAlignment(const VerticalAlignment& p_alignement);
			void setHorizontalAlignment(const HorizontalAlignment& p_alignement);
			void setLabelOffset(const Vector2Int& p_labelOffset);

			const std::string& text() const;
			Font* font() const;
			const Size_t& textSize() const;
			const Size_t& textPredefinedSize() const;
			const Color& textColor() const;
			const Color& outlineColor() const;
			const Vector2Int& anchor() const;
			const Vector2Int& size() const;
			const Vector2Int& labelOffset();
		};
	}
}