#include "Structure/Widget/jgl2_text_entry.h"
#include "Structure/jgl2_application.h"
#include "jgl2_drawing_functions.h"

namespace jgl
{

	void TextEntry::_onRender()
	{
		if (_selected == true && (jgl::Application::instance()->time() % 1000) > 500)
		{
			jgl::drawRectangleColor(_cursorColor, _label.anchor() + jgl::Vector2Int(0, _box.borderSize().y()) + _cursorPosition, _cursorSize, depth());
		}
		jgl::Vector2Int labelTextSize = _label.render(depth());

		if (labelTextSize.x() > _label.size().x())
		{
			_moveHigherCursor();
			_label.setText(_computeTextToRender());
		}

		_box.render(depth());
	}

	void TextEntry::_onGeometryChange()
	{
		_box.setGeometry(jgl::Vector2Int(0, 0), size());
		_label.setGeometry(_box.usableAnchor() + _label.labelOffset(), _box.usableSize() - _label.labelOffset() * jgl::Vector2Int(2, 2));
		_label.setTextPredefinedSize(_label.size().y());

		_cursorSize = jgl::Vector2Int(_label.size().y() / 7, _label.size().y() - _box.borderSize().y());
	}

	std::string TextEntry::_computeTextToRender()
	{
		std::string tmp_text;

		if (_masked == true)
		{
			tmp_text = std::string(_higherCursor - _lowerCursor, '*');
		}
		else
		{
			tmp_text = _entry.substr(_lowerCursor, _higherCursor - _lowerCursor);
		}

		return (tmp_text);
	}

	void TextEntry::_addCharInEntry(jgl::Char p_char)
	{
		_entry.insert(_entry.begin() + _cursor, p_char);
		_moveCursor(1);
		_computeHigherCursor();
		_label.setText(_computeTextToRender());
	}

	void TextEntry::_deleteCharInEntry(jgl::Int p_cursorPositionDelta)
	{
		if (_cursor == 0 && p_cursorPositionDelta < 0)
			throw std::runtime_error("Error while deleting char in textEntry");

		_moveCursor(p_cursorPositionDelta);
		_entry.erase(_entry.begin() + _cursor);
		_computeHigherCursor();
		_label.setText(_computeTextToRender());
	}

	void TextEntry::_moveHigherCursor()
	{
		_lowerCursor = _cursor;
		_higherCursor = _cursor;

		while (true)
		{
			std::string tmp_text = _computeTextToRender();
			jgl::Vector2Int tmp_textSize = _label.font()->calcStringSize(tmp_text, _label.textPredefinedSize());

			if (tmp_textSize.x() >= _label.size().x())
			{
				_lowerCursor++;
				break;
			}
			if (_lowerCursor <= 0)
			{
				break;
			}

			_lowerCursor--;
		}
	}

	void TextEntry::_moveLowerCursor()
	{
		_lowerCursor = _cursor;
		_computeHigherCursor();
	}

	void TextEntry::_computeHigherCursor()
	{
		_higherCursor = _cursor;

		while (true)
		{
			std::string tmp_text = _computeTextToRender();
			jgl::Vector2Int tmp_textSize = _label.font()->calcStringSize(tmp_text, _label.textPredefinedSize());

			if (tmp_textSize.x() >= _label.size().x())
			{
				_higherCursor--;
				break;
			}
			if (_higherCursor >= _entry.size())
			{
				break;
			}

			_higherCursor++;
		}
	}

	void TextEntry::_computeCursorPosition()
	{
		std::string tmp_text;
		
		if (_masked == true)
		{
			tmp_text = std::string(_cursor - _lowerCursor, '*');
		}
		else
		{
			tmp_text = _entry.substr(_lowerCursor, _cursor - _lowerCursor);
		}
		jgl::Vector2Int tmp_textSize = _label.font()->calcStringSize(tmp_text, _label.textPredefinedSize());
		_cursorPosition = jgl::Vector2Int(tmp_textSize.x(), 0);
	}

	void TextEntry::_moveCursor(jgl::Int p_cursorPositionDelta)
	{
		if ((_cursor == 0 && p_cursorPositionDelta < 0) ||
			(_cursor >= _entry.size() && p_cursorPositionDelta > 0))
			return;

		_cursor += p_cursorPositionDelta;

		if (_cursor > _higherCursor)
		{
			_moveHigherCursor();
		}
		else if (_cursor < _lowerCursor)
		{
			_moveLowerCursor();
		}

		_computeCursorPosition();
	}

	jgl::Bool TextEntry::_onUpdate()
	{
		if (jgl::Application::instance()->mouse().getButton(jgl::Mouse::Button::Left) == jgl::InputStatus::Released)
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
				_addCharInEntry(jgl::Application::instance()->keyboard().getEntry());
			}
			else if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Delete) == jgl::InputStatus::Released)
			{
				_deleteCharInEntry(0);
			}
			else if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::Backspace) == jgl::InputStatus::Released)
			{
				_deleteCharInEntry(-1);
			}
			else if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::LeftArrow) == jgl::InputStatus::Released)
			{
				_moveCursor(-1);
				_label.setText(_computeTextToRender());
			}
			else if (jgl::Application::instance()->keyboard().getKey(jgl::Keyboard::RightArrow) == jgl::InputStatus::Released)
			{
				_moveCursor(1);
				_label.setText(_computeTextToRender());
			}
		}

		return (false);
	}

	void TextEntry::_onPositionChange()
	{
		_box.recalc();
		_label.recalc();
	}

	TextEntry::TextEntry(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{

	}

	void TextEntry::setCursorColor(jgl::Color p_color)
	{
		_cursorColor = p_color;
	}
	
	const jgl::Color& TextEntry::cursorColor() const
	{
		return (_cursorColor);
	}

	void TextEntry::maskText()
	{
		_masked = true;
	}
	
	void TextEntry::unmaskText()
	{
		_masked = false;
	}

	void TextEntry::setMaskedText(jgl::Bool p_status)
	{
		_masked = p_status;
	}
	
	const jgl::Bool& TextEntry::maskedText() const
	{
		return (_masked);
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

	jgl::WidgetComponent::Box& TextEntry::box()
	{
		return (_box);
	}

	jgl::WidgetComponent::TextLabel& TextEntry::label()
	{
		return (_label);
	}

	void TextEntry::setText(const std::string& p_text)
	{
		_entry = p_text;
		_higherCursor = _cursor;
		_lowerCursor = 0;
		_label.setText(_entry);
	}

	const std::string& TextEntry::text() const
	{
		return (_entry);
	}
}