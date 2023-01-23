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
		using DefaultActivityFunctionPointer = std::function< void() >;
		class DefaultActivity : public Abstract::Activity
		{
		protected:
			DefaultActivityFunctionPointer _onEnterFunct;
			DefaultActivityFunctionPointer _executeFunct;
			DefaultActivityFunctionPointer _onExitFunct;

		public:
			DefaultActivity(DefaultActivityFunctionPointer p_onEnterFunct, DefaultActivityFunctionPointer p_executeFunct, DefaultActivityFunctionPointer p_onExitFunct) :
				_onEnterFunct(p_onEnterFunct),
				_executeFunct(p_executeFunct),
				_onExitFunct(p_onExitFunct)
			{

			}
			void execute()
			{
				if (_executeFunct != nullptr)
					_executeFunct();
			}
			void onEnter()
			{
				if (_onEnterFunct != nullptr)
					_onEnterFunct();
			}
			void onExit()
			{
				if (_onExitFunct != nullptr)
					_onExitFunct();
			}
		};

		std::map<TState, Abstract::Activity*> _activities;
		std::vector<DefaultActivity*> _activitiesToDelete;

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
			for (jgl::Size_t i = 0; i < _activitiesToDelete.size(); i++)
			{
				delete _activitiesToDelete[i];
			}
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
		void addActivity(TState p_state, DefaultActivityFunctionPointer p_executeFunction)
		{
			DefaultActivity* newActivity = new DefaultActivity(nullptr, p_executeFunction, nullptr);

			_activitiesToDelete.push_back(newActivity);
			addActivity(p_state, newActivity);
		}
		void addActivity(TState p_state, DefaultActivityFunctionPointer p_onEnterFunction, DefaultActivityFunctionPointer p_executeFunction)
		{
			DefaultActivity* newActivity = new DefaultActivity(p_onEnterFunction, p_executeFunction, nullptr);

			_activitiesToDelete.push_back(newActivity);
			addActivity(p_state, newActivity);
		}
		void addActivity(TState p_state, DefaultActivityFunctionPointer p_onEnterFunction, DefaultActivityFunctionPointer p_executeFunction, DefaultActivityFunctionPointer p_onExitFunction)
		{
			DefaultActivity* newActivity = new DefaultActivity(p_onEnterFunction, p_executeFunction, p_onExitFunction);

			_activitiesToDelete.push_back(newActivity);
			addActivity(p_state, newActivity);
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