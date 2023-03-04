#pragma once

#include "structure/Application/Core/jgl2_application_core.h"

namespace jgl
{
	namespace Application
	{
		class Console : public Abstract::Application::Core
		{
		private:
			void _setupJobs();
			jgl::Long _nbUpdate;
			jgl::Long _nbUpdatePerSecond;

		public:
			Console();
			static Console* instance() { return (static_cast<Console*>(_instance)); }
			const jgl::Long& nbUpdatePerSecond() {return (_nbUpdatePerSecond); }
		};
	}
}