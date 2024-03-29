#include "Structure/Widget/jgl2_button.h"
#include "Structure/jgl2_graphical_application.h"

namespace jgl
{
	void Button::_onRender()
	{
		jgl::Size_t index = 0;
		if (_pressed == true)
			index = 1;
		_labels[index].render(depth());
		_boxes[index].render(depth());
	}

	void Button::_onGeometryChange()
	{
		for (jgl::Size_t i = 0; i < 2; i++)
		{
			_boxes[i].setGeometry(jgl::Vector2Int(0, 0), size());
			_labels[i].setGeometry(_boxes[0].usableAnchor() + _labels[i].labelOffset(), _boxes[0].usableSize() - _labels[i].labelOffset() * jgl::Vector2Int(2, 2));
			if (_definedTextSize != 0)
				_labels[i].setTextSize(_definedTextSize);
		}

	}

	jgl::Bool Button::_onUpdate()
	{
		if (_pressed == true && isPointed() == false)
			_pressed = false;
		if (jgl::GraphicalApplication::instance()->mouse().getButton(jgl::Mouse::Button::Left) == jgl::InputStatus::Down &&
			isPointed() == true)
		{
			_pressed = true;
			return (true);
		}
		if (jgl::GraphicalApplication::instance()->mouse().getButton(jgl::Mouse::Button::Left) == jgl::InputStatus::Released &&
			isPointed() == true)
		{
			_pressed = false;
			_funct();
			return (true);
		}
		return (false);
	}

	void Button::_onPositionChange()
	{
		for (jgl::Size_t i = 0; i < 2; i++)
		{
			_boxes[i].recalc();
			_labels[i].recalc();
		}
	}

	Button::Button(std::function< void() > p_funct, jgl::Widget* p_parent) : jgl::Widget(p_parent),
		_funct(p_funct)
	{
		Color uppedBackgroundColor = Color(120, 120, 120);
		Color uppedFrontgroundColor = Color(150, 150, 150);
		Color uppedTextColor = Color(255, 255, 255);
		Color uppedTextOutlineColor = Color(0, 0, 0);

		Color clickedBackgroundColor = Color(90, 90, 90);
		Color clickedFrontgroundColor = Color(120, 120, 120);
		Color clickedTextColor = Color(200, 200, 200);
		Color clickedTextOutlineColor = Color(0, 0, 0);

		_boxes[0].setColor(uppedBackgroundColor, uppedFrontgroundColor);
		_boxes[1].setColor(clickedBackgroundColor, clickedFrontgroundColor);

		_labels[0].setColor(uppedTextColor, uppedTextOutlineColor);
		_labels[1].setColor(clickedTextColor, clickedTextOutlineColor);

		for (jgl::Size_t i = 0; i < 2; i++)
		{
			_labels[i].setText("Click me");
			_labels[i].setHorizontalAlignment(jgl::HorizontalAlignment::Centred);
			_labels[i].setVerticalAlignment(jgl::VerticalAlignment::Centred);
		}
	}

	void Button::setDefinedTextSize(const jgl::Size_t& p_definedTextSize)
	{
		_definedTextSize = p_definedTextSize;
	}

	const jgl::Size_t Button::definedTextSize()
	{
		return (_definedTextSize);
	}

	void Button::setFunct(const std::function<void()>& p_funct)
	{
		_funct = p_funct;
	}

	const std::function<void()>& Button::funct()
	{
		return (_funct);
	}

	void Button::setText(std::string p_text)
	{
		for (jgl::Size_t i = 0; i < 2; i++)
			_labels[i].setText(p_text);
	}

	jgl::WidgetComponent::Box* Button::boxes()
	{
		return (_boxes);
	}

	jgl::WidgetComponent::Box& Button::selectedBox()
	{
		return (_boxes[1]);
	}

	jgl::WidgetComponent::Box& Button::unselectedBox()
	{
		return (_boxes[0]);
	}

	jgl::WidgetComponent::TextLabel* Button::labels()
	{
		return (_labels);
	}

	jgl::WidgetComponent::TextLabel& Button::selectedTextLabel()
	{
		return (_labels[1]);
	}

	jgl::WidgetComponent::TextLabel& Button::unselectedTextLabel()
	{
		return (_labels[0]);
	}
}