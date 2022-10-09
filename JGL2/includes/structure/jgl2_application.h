#pragma once

#include "jgl2_basic_types.h"
#include "jgl2_thread.h"
#include "jgl2_widget.h"

namespace jgl
{
	class Application
	{
	private:
		jgl::Thread* _update_thread = nullptr;
		jgl::Bool _running = false;
		jgl::ULong _time = 0;
		std::vector<jgl::Abstract::Widget*> _widgets;

		void _updateTime();
		void _runUpdate();
		void _runRender();
		void _run();

		static inline Application* _instance = nullptr;

	public:
		Application();
		void quit();
		int run();
		jgl::ULong time();

		static Application* instance();
	};
}
