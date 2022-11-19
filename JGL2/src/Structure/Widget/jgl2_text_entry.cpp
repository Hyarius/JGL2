#include "Structure/Widget/jgl2_text_entry.h"
#include "Structure/jgl2_application.h"
#include "jgl2_drawing_functions.h"

namespace jgl
{
	std::string TextEntry::_computeRenderableText()
	{
		std::string result = _totalText;

		return (result);
	}

	void TextEntry::_updateText(std::string p_totalText)
	{
		_totalText = p_totalText;
		_label.setText(_computeRenderableText());
	}

	void TextEntry::_onRender()
	{
		_box.render(depth());
		_label.render(depth() + 0.2f);
		if (_selected == true && (jgl::Application::instance()->time() % 1000) > 500)
		{
			jgl::drawRectangleColor(_cursorColor, _cursorPosition, _cursorSize, depth() + 0.4f);
		}
	}

	void TextEntry::_onGeometryChange()
	{
		_box.setGeometry(jgl::Vector2Int(0, 0), size());
		_label.setGeometry(_box.usableAnchor(), _box.usableSize());
		_cursorSize = jgl::Vector2Int((_label.size().y() - _box.borderSize().y() * 2) / 7, _label.size().y() - _box.borderSize().y() * 1.2f);
		_computeCursorPosition();

		if (_definedTextSize != 0)
			_label.setTextSize(_definedTextSize);
	}

	jgl::Bool TextEntry::_onUpdate()
	{
		if (jgl::Application::instance()->mouse().button(jgl::Mouse::Button::Left) == jgl::InputStatus::Released)
		{
			if (isPointed() == true)
			{
				select();
			}
			else
			{
				unselect();
			}
		}

		if (_selected == true)
		{
			if (jgl::Application::instance()->keyboard().getEntry() >= ' ')
			{
				_totalText.insert(_totalText.begin() + _cursor, jgl::Application::instance()->keyboard().getEntry());
				_cursor++;
				_label.setText(_totalText);
				_computeCursorPosition();
			}
			if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Backspace) == jgl::InputStatus::Released &&
				_totalText.size() >= 1)
			{
				_totalText.erase(_totalText.begin() + _cursor - 1);
				_cursor--;
				_label.setText(_totalText);
				_computeCursorPosition();
			}
			if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Delete) == jgl::InputStatus::Released &&
				_totalText.size() >= _cursor)
			{
				_totalText.erase(_totalText.begin() + _cursor);
				_label.setText(_totalText);
				_computeCursorPosition();
			}
			if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::LeftArrow) == jgl::InputStatus::Released &&
				_cursor > 0)
			{
				_cursor--;
				_computeCursorPosition();
			}
			if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::RightArrow) == jgl::InputStatus::Released &&
				_cursor < _totalText.size())
			{
				_cursor++;
				_computeCursorPosition();
			}
		}

		return (false);
	}

	TextEntry::TextEntry(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{

	}

	void TextEntry::_computeCursorPosition()
	{
		jgl::Vector2Int tmp_delta = _label.font()->calcStringSize(_totalText.substr(0, _cursor), _label.textSize());
		_cursorPosition = jgl::Vector2Int(_box.borderSize().x() * 1, _box.borderSize().y() * 2) + jgl::Vector2Int(tmp_delta.x(), 0) + _box.anchor();
	}

	void TextEntry::setDefinedTextSize(const jgl::Size_t& p_definedTextSize)
	{
		_definedTextSize = p_definedTextSize;
	}

	const jgl::Size_t TextEntry::definedTextSize()
	{
		return (_definedTextSize);
	}

	void TextEntry::setCursorColor(const jgl::Color& p_cursorColor)
	{
		_cursorColor = p_cursorColor;
	}
	
	const jgl::Color TextEntry::cursorColor()
	{
		return (_cursorColor);
	}

	void TextEntry::select()
	{
		_selected = true;
	}

	void TextEntry::unselect()
	{
		_selected = false;
	}

	void TextEntry::setSelectStatus(jgl::Bool p_status)
	{
		_selected = p_status;
	}

	jgl::Bool TextEntry::isSelected()
	{
		return (_selected);
	}
}