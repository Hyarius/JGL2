#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "Structure/jgl2_activity.h"

namespace jgl
{
	class ActivityManager
	{
	private:
		std::vector<Abstract::Activity*> _activities;

	public:
		ActivityManager()
		{

		}

		void addActivity(Abstract::Activity* p_activity)
		{
			_activities.push_back(p_activity);
		}

		void update()
		{
			for (jgl::Size_t i = 0; i < _activities.size();)
			{
				_activities[i]->update();
				if (_activities[i]->isCompleted() == true)
				{
					_activities.erase(_activities.begin() + i);
				}
				else
				{
					i++;
				}
			}
		}
	};
}