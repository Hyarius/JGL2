#include "Structure/Widget/Component/jgl2_text_label_component.h"
#include "jgl2_drawing_functions.h"
#include "Structure/jgl2_application.h"

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
			if (_textPredefinedSize != 0)
			{
				_textSize = _textPredefinedSize;
				return;
			}

			int delta[5] = { 100, 50, 20, 10, 1 };
			_textSize = 2;

			if (_text == "")
			{
				_textSize = _size.y();
				return;
			}

			for (int i = 0; i < 5; i++)
			{
				bool enough = false;
				while (enough == false)
				{
					if (p_font == nullptr)
						throw std::runtime_error("No font give to label component");

					jgl::Vector2Int tmp_size = p_font->calcStringSize(_text, _textSize + delta[i]);
					if (tmp_size.x() >= _size.x() || tmp_size.y() >= _size.y())
						enough = true;
					else
						_textSize += delta[i];
				}
			}

			_computedTextSize = true;
		}

		void TextLabel::_computeTextOffset(jgl::Font* p_font)
		{
			jgl::Vector2Int textSize = p_font->calcStringSize(_text, _textSize);

			switch (_verticalAlignment)
			{
			case jgl::VerticalAlignment::Top:
			{
				_textAnchor.y() = 0;
				break;
			}
			case jgl::VerticalAlignment::Centred:
			{
				_textAnchor.y() = (_size.y() - textSize.y()) / 2;
				break;
			}
			case jgl::VerticalAlignment::Down:
			{
				_textAnchor.y() = _size.y() - textSize.y();
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
				_textAnchor.x() = (_size.x() - textSize.x()) / 2;
				break;
			}
			case jgl::HorizontalAlignment::Right:
			{
				_textAnchor.x() = _size.x() - textSize.x();
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
				_shader = Application::instance()->shader("TextTexture2D");

			if (_modelSpaceBuffer == nullptr)
				_modelSpaceBuffer = _shader->buffer("model_space")->copy();
			if (_modelUvBuffer == nullptr)
				_modelUvBuffer = _shader->buffer("model_uv")->copy();
			if (_modelColorBuffer == nullptr)
				_modelColorBuffer = _shader->buffer("model_color")->copy();
			if (_modelOutlineColorBuffer == nullptr)
				_modelOutlineColorBuffer = _shader->buffer("model_outline_color")->copy();
			if (_indexesBuffer == nullptr)
				_indexesBuffer = _shader->elementBuffer()->copy();
			if (_textureUniform == nullptr)
				_textureUniform = _shader->uniform("textureID")->copy();
		}
		
		void TextLabel::_computeShaderBuffer(Float p_depth)
		{
			_selectedFont = _font;
			if (_selectedFont == nullptr)
			{
				if (jgl::Application::instance()->defaultFont() == nullptr)
					throw std::runtime_error("No default font found in jgl::Application");

				_selectedFont = jgl::Application::instance()->defaultFont();
			}

			if (_computedTextSize == false)
			{
				_computeTextSize(_selectedFont);
			}

			if (_computedTextOffset == false)
			{
				_computeTextOffset(_selectedFont);
			}

			_savedTextSize = _selectedFont->prepareDraw(_text, _anchor + _textAnchor, _textSize, _textColor, _textOutlineSize, _outlineColor, p_depth);

			_selectedFont->exportShaderData(_modelSpaceBuffer, _modelColorBuffer, _modelOutlineColorBuffer, _modelUvBuffer, _indexesBuffer);

			_selectedTextureID = _selectedFont->textureID(_textSize, _textOutlineSize);
		}
		
		void TextLabel::_castRender()
		{
			_shader->activate();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _selectedTextureID);

			_modelSpaceBuffer->activate();
			_modelColorBuffer->activate();
			_modelOutlineColorBuffer->activate();
			_modelUvBuffer->activate();
			_indexesBuffer->activate();

			_textureUniform->send(0);

			_shader->cast(jgl::Shader::Mode::Triangle, _indexesBuffer->size() / sizeof(jgl::UInt));
		}

		jgl::Vector2Int TextLabel::render(jgl::Float p_depth)
		{
			if (_initialized == false)
				_initiatizeOpenGL();

			if (_computed == false || (_font == nullptr && _selectedFont != jgl::Application::instance()->defaultFont()))
				_computeShaderBuffer(p_depth);

			_castRender();

			return (_savedTextSize);
		}

		void TextLabel::setFont(jgl::Font* p_font)
		{
			_font = p_font;
			_computed = false;
		}

		void TextLabel::setText(const std::string& p_text)
		{
			_text = p_text;
			_computedTextSize = false;
			_computedTextOffset = false;
			_computed = false;
		}

		void TextLabel::setTextSize(const Size_t& p_textSize)
		{
			_textSize = p_textSize;
			_computedTextSize = true;
			_computedTextOffset = false;
			_computed = false;
		}

		void TextLabel::setTextPredefinedSize(const Size_t& p_textPredefinedSize)
		{
			_textPredefinedSize = p_textPredefinedSize;
			_computed = false;
		}

		void TextLabel::setTextOutlineSize(const jgl::Size_t& p_textOutlineSize)
		{
			_textOutlineSize = p_textOutlineSize;
			_computed = false;
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
				if (jgl::Application::instance()->defaultFont() == nullptr)
					throw std::runtime_error("No default font found in jgl::Application");

				tmp_font = jgl::Application::instance()->defaultFont();
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
	}
}
