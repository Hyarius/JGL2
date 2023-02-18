#include "Structure/Application/Graphical/jgl2_mouse_manager.h"
#include "jgl2_basic_functions.h"

namespace jgl
{
	void MouseManager::_treatMessage(jgl::PolymorphicContainer* p_message)
	{
		jgl::UInt messageId;
		*p_message >> messageId;

		switch (messageId)
		{
		case WM_LBUTTONDOWN:
		{
			if (_mouse->_buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Left)] == jgl::InputStatus::Up)
				_mouse->_buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Left)] = jgl::InputStatus::Pressed;
			break;
		}
		case WM_MBUTTONDOWN:
		{
			if (_mouse->_buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Center)] == jgl::InputStatus::Up)
				_mouse->_buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Center)] = jgl::InputStatus::Pressed;
			break;
		}
		case WM_RBUTTONDOWN:
		{
			if (_mouse->_buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Right)] == jgl::InputStatus::Up)
				_mouse->_buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Right)] = jgl::InputStatus::Pressed;
			break;
		}
		case WM_LBUTTONUP:
		{
			_mouse->_buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Left)] = jgl::InputStatus::Released;
			break;
		}
		case WM_MBUTTONUP:
		{
			_mouse->_buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Center)] = jgl::InputStatus::Released;
			break;
		}
		case WM_RBUTTONUP:
		{
			_mouse->_buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Right)] = jgl::InputStatus::Released;
			break;
		}
		case WM_MOUSEWHEEL:
		{
			jgl::Short value;

			*p_message >> value;
			jgl::Float delta = value / 100.0f;

			_mouse->_wheel += delta;
			break;
		}
		case WM_MOUSEMOVE:
		{
			*p_message >> _mouse->_pos.x;
			*p_message >> _mouse->_pos.y;

			break;
		}
		}
	}

	MouseManager::MouseManager(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat) :
		MessageConsumer(p_messageToTreat),
		_mouse(new Mouse())
	{

	}

	void MouseManager::_onUpdate()
	{
		_mouse->_updateState();
	}
}