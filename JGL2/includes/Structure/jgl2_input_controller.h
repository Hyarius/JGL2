#pragma once

#include "jgl2_timer.h"
#include "jgl2_keyboard.h"
#include "jgl2_mouse.h"

namespace jgl
{
	class InputController
	{
	private:
		enum class Type
		{
			Keyboard,
			Mouse
		};
		jgl::Timer _timer;
		Type _type;
		union Data
		{
			jgl::Keyboard::Key key;
			jgl::Mouse::Button button;
		};
		Data _data;
		jgl::InputStatus _expectedStatus;
		std::function<void()> _funct;

	public:
		template <typename TInputType>
		InputController(const TInputType& p_input, jgl::InputStatus p_expectedStatus, std::function<void()> p_funct, jgl::ULong p_inputTimeout = 50) :
			_timer(p_inputTimeout),
			_expectedStatus(p_expectedStatus),
			_funct(p_funct)
		{
			_type = (std::is_same<TInputType, jgl::Keyboard::Key>::value ? Type::Keyboard : Type::Mouse);
			
			_data = static_cast<Data>(p_input);
		}
		void update()
		{
			jgl::Bool needCast = false;

			if (_timer.isRunning() == true)
				return;

			if ((_type == Type::Keyboard && jgl::GraphicalApplication::instance()->keyboard().getKey(_data.key) == _expectedStatus) ||
				(_type == Type::Mouse && jgl::GraphicalApplication::instance()->mouse().getButton(_data.button) == _expectedStatus))
				needCast = true;

			if (needCast == true)
			{
				_timer.start();
				_funct();
			}
		}
	};
}