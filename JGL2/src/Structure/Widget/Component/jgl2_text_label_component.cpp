#include "Structure/Widget/Component/jgl2_text_label_component.h"
#include "jgl2_drawing_functions.h"
#include "Structure/jgl2_graphical_application.h"

namespace jgl
{
	namespace WidgetComponent
	{
		TextLabel::TextLabel()
		{
			_computedTextSize = false;
			_computedTextOffset = false;
		}

		void TextLabel::_computeTextSize(jgl::Font* p_font)
		{
			jgl::Size_t oldTextSize = _textSize;
			if (_textPredefinedSize != 0)
			{
				_textSize = _textPredefinedSize;
				if (oldTextSize != _textSize)
					_selectedTextureID = 0;
				return;
			}

			_textSize = p_font->computeTextSize(_text, _size);

			if (oldTextSize != _textSize)
				_selectedTextureID = 0;
			_computedTextSize = true;
		}

		void TextLabel::_computeTextOffset(jgl::Font* p_font)
		{
			_textRenderedSize = p_font->calcStringSize(_text, _textSize);

			switch (_verticalAlignment)
			{
			case jgl::VerticalAlignment::Top:
			{
				_textAnchor.y() = 0;
				break;
			}
			case jgl::VerticalAlignment::Centred:
			{
				_textAnchor.y() = (_size.y() - _textRenderedSize.y()) / 2;
				break;
			}
			case jgl::VerticalAlignment::Down:
			{
				_textAnchor.y() = _size.y() - _textRenderedSize.y();
				break;
			}
			default:
			{
				_textAnchor.y() = 0;
				break;
			}
			}


			switch (_horizontalAlignment)
			{
			case jgl::HorizontalAlignment::Left:
			{
				_textAnchor.x() = 0;
				break;
			}
			case jgl::HorizontalAlignment::Centred:
			{
				_textAnchor.x() = (_size.x() - _textRenderedSize.x()) / 2;
				break;
			}
			case jgl::HorizontalAlignment::Right:
			{
				_textAnchor.x() = _size.x() - _textRenderedSize.x();
				break;
			}
			default:
			{
				_textAnchor.x() = 0;
				break;
			}
			}

			_computedTextOffset = true;
		}

		void TextLabel::_initiatizeOpenGL()
		{
			if (_shader == nullptr)
				_shader = GraphicalApplication::instance()->shader("TextTexture2D");

			if (_modelSpaceBuffer == nullptr)
				_modelSpaceBuffer = _shader->buffer("model_space")->copy();
			if (_modelUvsBuffer == nullptr)
				_modelUvsBuffer = _shader->buffer("model_uvs")->copy();
			if (_indexesBuffer == nullptr)
				_indexesBuffer = _shader->elementBuffer()->copy();
			if (_textureUniform == nullptr)
				_textureUniform = _shader->uniform("textureID")->copy();
			if (_textColorUniform == nullptr)
				_textColorUniform = _shader->uniform("textColor")->copy();
			if (_outlineColorUniform == nullptr)
				_outlineColorUniform = _shader->uniform("outlineColor")->copy();
		}

		void TextLabel::_computeShaderBuffer(Float p_depth)
		{
			Font* tmp_font = _selectedFont;

			_selectedFont = _font;
			if (_selectedFont == nullptr)
			{
				if (jgl::GraphicalApplication::instance()->defaultFont() == nullptr)
					throw std::runtime_error("No default font found in jgl::GraphicalApplication");

				_selectedFont = jgl::GraphicalApplication::instance()->defaultFont();
			}
			if (_selectedFont != tmp_font)
				_selectedTextureID = 0;

			if (_computedTextSize == false)
			{
				_computeTextSize(_selectedFont);
			}

			if (_computedTextOffset == false)
			{
				_computeTextOffset(_selectedFont);
			}

			_savedTextSize = _selectedFont->prepareDraw(_text, _anchor + _textAnchor, _textSize, _textOutlineSize, p_depth);

			_selectedFont->exportShaderData(_modelSpaceBuffer, _modelUvsBuffer, _indexesBuffer);

			if (_selectedTextureID == 0)
			{
				_selectedTextureID = _selectedFont->textureID(_textSize, _textOutlineSize);
			}

			_computed = true;
		}
		
		void TextLabel::_castRender()
		{
			_shader->activate();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _selectedTextureID);

			_modelSpaceBuffer->activate();
			_modelUvsBuffer->activate();
			_indexesBuffer->activate();

			_textureUniform->send(0);
			_textColorUniform->send(_textColor);
			_outlineColorUniform->send(_outlineColor);

			_shader->cast(jgl::Shader::Mode::Triangle, _indexesBuffer->size() / sizeof(jgl::UInt));
		}

		jgl::Vector2Int TextLabel::render(jgl::Float p_depth)
		{
			if (_initialized == false)
				_initiatizeOpenGL();

			if (_computed == false || (_font == nullptr && _selectedFont != jgl::GraphicalApplication::instance()->defaultFont()))
				_computeShaderBuffer(p_depth + 0.01f);

			_castRender();

			return (_savedTextSize);
		}

		void TextLabel::recalc()
		{
			_computed = false;
		}

		void TextLabel::setFont(jgl::Font* p_font)
		{
			_font = p_font;
			_computed = false;
			_selectedFont = nullptr;
		}

		void TextLabel::setText(const std::string& p_text)
		{
			_text = p_text;
			_computedTextSize = false;
			_computedTextOffset = false;
			_computed = false;
			recalc();
		}

		void TextLabel::setTextSize(const Size_t& p_textSize)
		{
			_textSize = p_textSize;
			_computedTextSize = true;
			_computedTextOffset = false;
			_computed = false;
			_selectedTextureID = 0;
		}

		void TextLabel::setTextPredefinedSize(const Size_t& p_textPredefinedSize)
		{
			_textPredefinedSize = p_textPredefinedSize;
			_textSize = _textPredefinedSize;
			_computed = false;
			_selectedTextureID = 0;
		}

		void TextLabel::setTextOutlineSize(const jgl::Size_t& p_textOutlineSize)
		{
			_textOutlineSize = p_textOutlineSize;
			_computed = false;
			_selectedTextureID = 0;
		}

		void TextLabel::setColor(const Color& p_textColor, const Color& p_outlineColor)
		{
			_textColor = p_textColor;
			_outlineColor = p_outlineColor;
			_computed = false;
		}
		
		void TextLabel::setGeometry(const Vector2Int& p_anchor, const Vector2Int& p_size)
		{
			_anchor = p_anchor;
			_size = p_size;
			_textSize = _size.y();
			_computedTextSize = false;
			_computedTextOffset = false;
			_computed = false;
			_selectedTextureID = 0;
		}

		void TextLabel::setVerticalAlignment(const VerticalAlignment& p_alignement)
		{
			_verticalAlignment = p_alignement;
			_computedTextOffset = false;
			_computed = false;
		}
		
		void TextLabel::setHorizontalAlignment(const HorizontalAlignment& p_alignement)
		{
			_horizontalAlignment = p_alignement;
			_computedTextOffset = false;
			_computed = false;
		}

		void TextLabel::setLabelOffset(const jgl::Vector2Int& p_labelOffset)
		{
			_labelOffset = p_labelOffset;
			_computed = false;
		}

		jgl::Font* TextLabel::font() const
		{
			jgl::Font* tmp_font = _font;
			if (tmp_font == nullptr)
			{
				if (jgl::GraphicalApplication::instance()->defaultFont() == nullptr)
					throw std::runtime_error("No default font found in jgl::GraphicalApplication");

				tmp_font = jgl::GraphicalApplication::instance()->defaultFont();
			}
			return (tmp_font);
		}

		const std::string& TextLabel::text() const
		{
			return (_text);
		}

		const Size_t& TextLabel::textSize() const
		{
			return (_textSize);
		}

		const Size_t& TextLabel::textPredefinedSize() const
		{
			return (_textPredefinedSize);
		}

		const Color& TextLabel::textColor() const
		{
			return (_textColor);
		}

		const Color& TextLabel::outlineColor() const
		{
			return (_outlineColor);
		}

		const Vector2Int& TextLabel::anchor() const
		{
			return (_anchor);
		}

		const Vector2Int& TextLabel::size() const
		{
			return (_size);
		}

		const jgl::Vector2Int& TextLabel::labelOffset()
		{
			return (_labelOffset);
		}

		Vector2Int TextLabel::currentTextSize() const
		{
			return (font()->calcStringSize(_text, _textSize));
		}
	}
}
