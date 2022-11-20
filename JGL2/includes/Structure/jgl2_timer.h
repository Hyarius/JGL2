#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

namespace jgl
{
	class Timer
	{
	public:
		struct State
		{
			enum {
				Stopped,
				Timeout,
				Running
			} value;

			State(decltype(value) p_value = decltype(value)::Stopped) : value(p_value) {}
			
			operator bool() const {
				return value == State::Running;
			}
		};
	private:
		State _state;
		jgl::ULong _startingTime;
		jgl::ULong _timerDuration;

	public:
		Timer(jgl::ULong p_timer_period);

		void setDuration(jgl::ULong p_timer_period);
		jgl::ULong elapsedTime();
		jgl::ULong duration() { return (_timerDuration); }
		jgl::Float percent();
		void start();
		void stop();
		State timeout();

	};
}