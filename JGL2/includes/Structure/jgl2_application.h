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

namespace jgl
{
	class Application
	{
	public:
		friend class Viewport;
		friend class Widget;
		friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		OpenGLContext _context;
		
		std::recursive_mutex _messageMutex;
		static std::map<UInt, std::string> WinMessageToString;
		Pool< PolymorphicContainer> _messagePool;
		jgl::LockedQueue< PolymorphicContainer*> _messagesToTreat;

		Thread* _updateThread = nullptr;
		Bool _running = false;
		Bool _multithreaded = false;
		Bool _isCursorVisible = true;
		HCURSOR _defaultCursor;
		ULong _time = 0;
		std::vector<Widget*> _widgets;
		jgl::Float _maxDepth = 1000000.0f;

		jgl::ULong _nbUpdateFrame = 0;
		jgl::ULong _nbRenderFrame = 0;

		Font* _defaultFont;

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
		void _create2DTextTextureShader();

		void _setViewport(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size);

		static inline Application* _instance = nullptr;

		PolymorphicContainer* _obtainWinMessage();
		void _releaseWinMessage(PolymorphicContainer* p_msg);
		void _insertWinMessageToTreat(PolymorphicContainer* p_msg);
		PolymorphicContainer* _getWinMessageToTreat();

	public:
		Application(std::string p_title, Vector2Int p_size, jgl::Color p_backgroundColor);
		void quit();
		Int run();

		jgl::ULong nbUpdateFrame() { return (_nbUpdateFrame); }
		jgl::ULong nbRenderFrame() { return (_nbRenderFrame); }

		void hideCursor();
		void revealCursor();
		void setCursorVisibleStatus(jgl::Bool p_state);
		jgl::Bool isCursorVisible();

		void setDefaultFont(Font* p_defaultFont);
		Font* defaultFont() const;

		void addShader(std::string p_shaderName, Shader* p_shader);
		Shader* shader(std::string p_shaderName);

		const Keyboard& keyboard() const { return (_keyboard); }
		const Mouse& mouse() const { return (_mouse); }

		void activateMultiThread();
		void deactivateMultiThread();

		void setMaxDepth(jgl::Float p_maxDepth);
		void resize(jgl::Vector2Int p_size);

		const ULong& time() const;
		const Vector2Int& size() const;
		const Float maxDepth() const;

		Vector2 convertScreenToOpenGL(Vector2Int p_screenPos);
		Vector2Int convertOpenGLToScreen(Vector2 p_openGLPos);

		static Application* instance();
	};
}
