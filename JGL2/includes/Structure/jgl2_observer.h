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

		template <typename Func, typename... Args>
		void subscribe(TEventType p_event, Func&& p_func, Args&&... p_args)
		{
			_functs[p_event].push_back(std::bind(std::forward<Func>(p_func), std::forward<Args>(p_args)...));
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