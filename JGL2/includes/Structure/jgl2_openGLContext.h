#pragma once

#include "jgl2_includes.h"
#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_color.h"

namespace jgl
{
	class OpenGLContext
	{
	private:
		wchar_t* _convertedTitle = nullptr;
		Vector2Int _size;
		Vector2Int _actualSize;
		Int _majorVersion;
		Int _minorVersion;

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
		void _setupWindowSize(Vector2Int p_size);
		void _composeOpenGLContext();

	public:
		OpenGLContext();
		~OpenGLContext();
		void initialize(std::string p_title, Vector2Int p_size, Int p_major_version = 4, Int p_minor_version = 2);

		void setup(Color background);
		void resize(Int w, Int h);
		void clear();
		void render();
		void setViewport(Int p_x, Int p_y, Int p_w, Int p_h);

		const Vector2Int& size() const;
	};
}