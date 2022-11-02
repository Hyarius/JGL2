#pragma once

#include "jgl2_includes.h"
#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_color.h"

namespace jgl
{
	class OpenGLContext
	{
	private:
		jgl::WChar* _convertedTitle = nullptr;
		jgl::Vector2Int _size;
		jgl::Int _majorVersion;
		jgl::Int _minorVersion;

		HWND _windowFrame;
		HINSTANCE _hInstance = NULL;
		RECT _windowSize;
		WNDCLASS _windowClass;
		DWORD _windowStyle;
		DWORD _windowExStyle;

		HGLRC _hrc;
		HDC _hdc;
		PIXELFORMATDESCRIPTOR _pfd;

		void _createWindowClass();
		void _createGhostInstance();
		void _setupWindowSize(jgl::Vector2Int p_size);
		void _composeOpenGLContext();

	public:
		OpenGLContext();
		~OpenGLContext();
		void initialize(std::string p_title, jgl::Vector2Int p_size, jgl::Int p_major_version = 4, jgl::Int p_minor_version = 2);

		void setup(jgl::Color background);
		void resize(jgl::Int w, jgl::Int h);
		void clear();
		void render();

		jgl::Vector2Int size();
	};
}