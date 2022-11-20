#include "Structure/jgl2_timer.h"
#include "Structure/jgl2_application.h"

namespace jgl
{
	Timer::Timer(jgl::ULong p_timer_period)
	{
		_state = State::Stopped;
		_timerDuration = p_timer_period;
	}

	void Timer::setDuration(jgl::ULong p_timer_period)
	{
		_state = State::Stopped;
		_timerDuration = p_timer_period;
	}

	void Timer::start()
	{
		_startingTime = jgl::Application::instance()->time();
		_state = State::Running;
	}

	void Timer::stop()
	{
		_state = State::Stopped;
	}

	Timer::State Timer::timeout()
	{
		if (_state == State::Running && _startingTime + _timerDuration <= jgl::Application::instance()->time())
		{
			_state = State::Timeout;
		}

		return (_state);
	}

	jgl::ULong Timer::elapsedTime()
	{
		return (jgl::Application::instance()->time() - _startingTime);
	}

	jgl::Float Timer::percent()
	{
		return std::clamp(0.0f, static_cast<jgl::Float>(elapsedTime()) / _timerDuration, 100.0f);
	}
}