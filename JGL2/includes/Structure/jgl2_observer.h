#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

namespace jgl
{
	template<typename TEventType>
	class Observer
	{
	private:
		std::map < TEventType, std::vector < std::function< void() > > > _functs;

	public:
		Observer()
		{

		}

		void subscribe(TEventType p_event, std::function<void()> p_funct)
		{
			_functs[p_event].push_back(p_funct);
		}

		void notify(TEventType p_event)
		{
			std::vector<std::function<void()>>& tmp_array = _functs[p_event];
			for (jgl::Size_t i = 0; i < tmp_array.size(); i++)
			{
				tmp_array[i]();
			}
		}
	};
}