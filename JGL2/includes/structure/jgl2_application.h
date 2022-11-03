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
#include "Structure/jgl2_shader.h"
#include "jgl2_basic_functions.h"

namespace jgl
{
	class Application
	{
	public:

		friend class Abstract::Widget;
	private:
		OpenGLContext _context;
		
		static std::map<UInt, std::string> WinMessageToString;
		Pool< PolymorphicContainer> _messagePool;
		std::deque< PolymorphicContainer*> _messagesToTreat;

		Thread* _updateThread = nullptr;
		Bool _running = false;
		ULong _time = 0;
		std::vector<Abstract::Widget*> _widgets;

		std::map<std::string, Shader*> _shaders;

		Keyboard _keyboard;
		Mouse _mouse;

		void _pullWinMessage();
		void _handleWinMessage();
		void _updateTime();

		void _runUpdate();
		void _runRender();
		void _runMonoThread();
		void _run();

		void _create2DColorShader();
		void _create2DTextureShader();

		static inline Application* _instance = nullptr;

	public:
		Application(std::string p_title, Vector2Int p_size);
		void quit();
		Int run();

		void addShader(std::string p_shaderName, Shader* p_shader);
		Shader* shader(std::string p_shaderName);

		void resize(jgl::Vector2Int p_size);

		ULong time();
		Vector2Int size();

		PolymorphicContainer* obtainWinMessage();
		void releaseWinMessage(PolymorphicContainer* p_msg);
		void insertWinMessageToTreat(PolymorphicContainer* p_msg);
		PolymorphicContainer* getWinMessageToTreat();

		Vector2 convertScreenToOpenGL(Vector2Int p_screenPos);
		Vector2Int convertOpenGLToScreen(Vector2 p_openGLPos);

		static Application* instance();
	};
}
