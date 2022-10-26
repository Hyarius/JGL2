#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "jgl2_thread.h"
#include "jgl2_widget.h"
#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_openGLContext.h"
#include "Structure/jgl2_polymorphic_container.h"
#include "Structure/jgl2_pool.h"
#include "Structure/jgl2_keyboard.h"
#include "Structure/jgl2_mouse.h"

namespace jgl
{
	class Application
	{
	public:

		friend class jgl::Abstract::Widget;
	private:
		OpenGLContext _context;
		
		static std::map<jgl::UInt, jgl::String> WinMessageToString;
		std::recursive_mutex _message_mutex;
		jgl::Pool< jgl::PolymorphicContainer> _messagePool;
		std::deque< jgl::PolymorphicContainer*> _messagesToTreat;

		jgl::Thread* _update_thread = nullptr;
		jgl::Bool _running = false;
		jgl::ULong _time = 0;
		std::vector<jgl::Abstract::Widget*> _widgets;

		jgl::Keyboard _keyboard;
		jgl::Mouse _mouse;

		void _pullWinMessage();
		void _handleWinMessage();
		void _updateTime();

		void _runUpdate();
		void _runRender();
		void _run();

		static inline Application* _instance = nullptr;

	public:
		Application(jgl::String p_title, jgl::Vector2Int p_size);
		void quit();
		jgl::Int run();
		jgl::ULong time();

		jgl::PolymorphicContainer* obtainWinMessage();
		void releaseWinMessage(jgl::PolymorphicContainer* p_msg);
		void insertWinMessageToTreat(jgl::PolymorphicContainer* p_msg);
		jgl::PolymorphicContainer* getWinMessageToTreat();

		static Application* instance();
	};
}
