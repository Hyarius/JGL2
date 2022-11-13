#include "Structure/Widget/Component/jgl2_text_label_component.h"
#include "jgl2_drawing_functions.h"
#include "Structure/jgl2_application.h"

namespace jgl
{
	namespace WidgetComponent
	{
		TextLabel::TextLabel()
		{
			_computed = false;
		}

		void TextLabel::_computeTextSize(jgl::Font* p_font)
		{
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

			_computed = true;
		}

		void TextLabel::render(jgl::Float p_depth)
		{
			jgl::Font* tmp_font = _font;
			if (tmp_font == nullptr)
			{
				if (jgl::Application::instance()->defaultFont() == nullptr)
					throw std::runtime_error("No default font found in jgl::Application");

				tmp_font = jgl::Application::instance()->defaultFont();
			}

			if (_computed == false)
			{
				_computeTextSize(tmp_font);
			}

			tmp_font->draw(_text, _anchor, _textSize, _textColor, _outlineColor, p_depth);
		}

		void TextLabel::setText(std::string p_text)
		{
			_text = p_text;
			_computed = false;
		}

		void TextLabel::setColor(Color p_textColor, Color p_outlineColor)
		{
			_textColor = p_textColor;
			_outlineColor = p_outlineColor;
		}
		
		void TextLabel::setGeometry(Vector2Int p_anchor, Vector2Int p_size)
		{
			_anchor = p_anchor;
			_size = p_size;
			_textSize = _size.y();
			_computed = false;
		}

		const std::string& TextLabel::text() const
		{
			return (_text);
		}

		const Size_t& TextLabel::textSize() const
		{
			return (_textSize);
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
	}
}
