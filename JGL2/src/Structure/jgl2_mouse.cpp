#include "Structure/jgl2_mouse.h"

namespace jgl
{
	Mouse::Mouse()
	{
        _wheel = 0;
        for (jgl::Size_t i = 0; i < C_NB_BUTTON; i++)
            _buttons[i] = jgl::InputStatus::Up;
	}

	void Mouse::_updateState()
	{
        _wheel = 0;
        for (jgl::Size_t i = 0; i < C_NB_BUTTON; i++)
        {
            if (_buttons[i] == jgl::InputStatus::Pressed)
            {
                _buttons[i] = jgl::InputStatus::Down;
            }
            else if (_buttons[i] == jgl::InputStatus::Released)
            {
                _buttons[i] = jgl::InputStatus::Up;
            }
        }
	}

    jgl::Float Mouse::wheel() const
    {
        return (_wheel);
    }

    jgl::Vector2Int Mouse::pos() const
    {
        return (_pos);
    }
    
    jgl::InputStatus Mouse::getButton(Mouse::Button p_button) const
    {
        return (_buttons[static_cast<jgl::Size_t>(p_button)]);
    }

    std::string to_string(const Mouse::Button& p_button)
    {
        switch (p_button)
        {
        case (Mouse::Button::Center):
            return ("Center");
        case (Mouse::Button::Right):
            return ("Right");
        case (Mouse::Button::Left):
            return ("Left");
        }
        return ("Unknow");
    }
}