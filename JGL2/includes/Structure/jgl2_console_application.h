#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "jgl2_thread.h"
#include "Structure/Widget/jgl2_widget.h"
#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_openGLContext.h"
#include "Structure/jgl2_polymorphic_container.h"
#include "Structure/jgl2_pool.h"
#include "Structure/jgl2_keyboard.h"
#include "Structure/jgl2_mouse.h"
#include "Structure/jgl2_shader.h"
#include "Structure/jgl2_viewport.h"
#include "jgl2_basic_functions.h"
#include "Structure/jgl2_locked_queue.h"

#include "Structure/jgl2_abstract_application.h"

namespace jgl
{
	class ConsoleApplication : public AbstractApplication
	{
	private:

	public:
		ConsoleApplication();

		Int run();
	};
}