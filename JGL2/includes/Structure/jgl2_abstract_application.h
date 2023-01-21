#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	class AbstractApplication
	{
	protected:
		friend class Widget;
		static inline AbstractApplication* _instance = nullptr;

		std::vector<Widget*> _widgets;

		Bool _running = false;
		ULong _time = 0;

		void _updateTime()
		{
			auto epoch = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch();

			_time = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
		}

	public:
		AbstractApplication()
		{
			if (_instance != nullptr)
				throw std::runtime_error("GraphicalApplication already created");
			_instance = this;
		}

		void quit()
		{
			_running = false;
		}

		virtual Int run() = 0;

		const ULong& time() const
		{
			return (_time);
		}
	};
}