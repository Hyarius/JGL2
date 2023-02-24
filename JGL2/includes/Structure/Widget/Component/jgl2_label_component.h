#pragma once

#include "Structure/jgl2_color.h"
#include "Structure/jgl2_vector2.h"
#include "Structure/jgl2_shader.h"
#include "Structure/jgl2_font.h"

#include "Structure/jgl2_monitored_value.h"

#include "Structure/Application/Graphical/jgl2_application.h"

namespace jgl
{
	namespace Widget
	{
		namespace Component
		{
			class Label
			{
			public:
				struct DefaultValues
				{
					jgl::Size_t textSize = 40;
					jgl::Size_t outlineSize = 5;
					jgl::Color textColor = jgl::Color(255, 255, 255);
					jgl::Color outlineColor = jgl::Color(0, 0, 0);
					jgl::HorizontalAlignment horizontalAlignment = jgl::HorizontalAlignment::Centred;
					jgl::VerticalAlignment verticalAlignment = jgl::VerticalAlignment::Centred;
				};
				static inline DefaultValues defaultValues;

			private:
				std::string _text;

				jgl::MonitoredValue<jgl::Font*> _font;
				jgl::MonitoredValue<jgl::Size_t> _textSize;
				jgl::MonitoredValue<jgl::Size_t> _outlineSize;
				jgl::MonitoredValue<jgl::Color> _textColor;
				jgl::MonitoredValue<jgl::Color> _outlineColor;
				jgl::MonitoredValue<jgl::HorizontalAlignment> _horizontalAlignment;
				jgl::MonitoredValue<jgl::VerticalAlignment> _verticalAlignment;

				jgl::Vector2Int _textAnchorOffset;

				jgl::Vector2Int _anchor;
				jgl::Vector2Int _size;
				jgl::Float _depth;

				jgl::Vector2Int _textRenderedSize;

				static inline jgl::Shader* _shader = nullptr;
				jgl::Buffer* _verticesBuffer = nullptr;
				jgl::Buffer* _uvsBuffer = nullptr;
				jgl::Buffer* _depthBuffer = nullptr;
				jgl::Buffer* _elementBuffer = nullptr;

				jgl::Uniform* _textureUniform = nullptr;
				jgl::Uniform* _textColorUniform = nullptr;
				jgl::Uniform* _outlineColorUniform = nullptr;
				jgl::Uniform* _outlineSizeUniform = nullptr;

				GLuint _fontTextureID = 0;

				jgl::Bool _shaderInitialized = false;
				jgl::Bool _verticesBaked = false;
				jgl::Bool _depthBaked = false;

				void _computeTextAnchor();

				void _initializeShaderDatas();

				void _computeVerticesData();
				void _computeDepthData();

			public:
				Label();

				void render();

				void setDefaultValues(DefaultValues& p_defaultValues);

				void reset();
				void useValue();

				void computeTextSize();

				void setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size);
				void setDepth(jgl::Float p_depth);

				const jgl::Vector2Int& anchor() const { return (_anchor); }
				const jgl::Vector2Int& size() const { return (_size); }
				const jgl::Float& depth() const { return (_depth); }

				void setText(std::string p_text);
				void setTextSize(jgl::Size_t p_textSize);
				void setOutlineSize(jgl::Size_t p_outlineSize);
				void setTextColor(jgl::Color p_textColor);
				void setOutlineColor(jgl::Color p_outlineColor);
				void setHorizontalAlignment(jgl::HorizontalAlignment p_alignment);
				void setVerticalAlignment(jgl::VerticalAlignment p_alignment);

				const std::string& text() const { return (_text); }
				jgl::Font* const& font() const { return (_font.value()); }
				const jgl::Size_t& textSize() const { return (_textSize); }
				const jgl::Size_t& outlineSize() const { return (_outlineSize); }

				const jgl::Color& textColor() const { return (_textColor.value()); }
				const jgl::Color& outlineColor() const { return (_outlineColor.value()); }
			};
		}
	}
}