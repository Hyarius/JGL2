#pragma once

namespace jgl
{
	namespace Abstract
	{
		class Activity
		{
		protected:

		public:
			Activity()
			{

			}
			virtual ~Activity()
			{

			}
			virtual void execute() = 0;
			virtual void onEnter() = 0;
			virtual void onExit() = 0;
		};
	}

	template <typename TState>
	class StateMachine
	{
	private:
		std::map<TState, Abstract::Activity*> _activities;

		Abstract::Activity* _activeActivity = nullptr;
		std::recursive_mutex _mutex;
		TState _lastState;
		TState _state;

	public:
		StateMachine()
		{

		}
		~StateMachine()
		{

		}
		TState state() const
		{
			return (_state);
		}
		void setState(TState p_state)
		{
			_mutex.lock();
			_lastState = _state;
			_state = p_state;
			_mutex.unlock();
		}

		void addActivity(TState p_state, Abstract::Activity* p_activity)
		{
			_activities[p_state] = p_activity;
		}

		void update()
		{
			if (_activities.count(_state) == 0)
				return;

			_mutex.lock();
			if (_state == _lastState && _activeActivity != nullptr)
			{
				_activeActivity->execute();
			}
			else
			{
				_lastState = _state;

				if (_activeActivity != nullptr)
					_activeActivity->onExit();

				_activeActivity = _activities[_state];

				if (_activeActivity != nullptr)
					_activeActivity->onEnter();
			}
			_mutex.unlock();
		}
	};
}