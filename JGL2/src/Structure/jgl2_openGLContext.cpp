#include "Structure/jgl2_openGLContext.h"
#include "jgl2_basic_functions.h"


namespace jgl
{
	void GLAPIENTRY
		MessageOpenGLCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		if (severity == GL_DEBUG_TYPE_ERROR || severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR || severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ||
			severity == GL_DEBUG_TYPE_ERROR_ARB || severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB || severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
			fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
	}
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OpenGLContext::_createWindowClass()
	{
		_windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		_windowClass.lpfnWndProc = (WNDPROC)jgl::WndProc;
		_windowClass.cbClsExtra = 0;
		_windowClass.cbWndExtra = 0;
		_windowClass.hInstance = _hInstance;
		_windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		_windowClass.hbrBackground = NULL;
		_windowClass.lpszMenuName = NULL;
		_windowClass.lpszClassName = _convertedTitle;
	}

	void OpenGLContext::_createGhostInstance()
	{
		_hInstance = GetModuleHandle(NULL);
	}

	void OpenGLContext::_setupWindowSize(jgl::Vector2Int p_size)
	{
		_windowSize.left = (long)0;
		_windowSize.right = (long)p_size.x();
		_windowSize.top = (long)0;
		_windowSize.bottom = (long)p_size.y();

		_windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		_windowStyle = WS_OVERLAPPEDWINDOW;

		AdjustWindowRectEx(&_windowSize, _windowStyle, FALSE, _windowExStyle);

		_windowFrame = CreateWindowEx(_windowExStyle, (LPCTSTR)(_convertedTitle), (LPCTSTR)(_convertedTitle), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, _windowSize.right - _windowSize.left, _windowSize.bottom - _windowSize.top, NULL, NULL, _hInstance, NULL);
	}

	void OpenGLContext::_composeOpenGLContext()
	{
		_hdc = GetDC(_windowFrame);

		memset(&_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

		_pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		_pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		_pfd.iPixelType = PFD_TYPE_RGBA;
		_pfd.cColorBits = 24;
		_pfd.cDepthBits = 32;
		_pfd.iLayerType = PFD_MAIN_PLANE;

		jgl::Int nPixelFormat = ChoosePixelFormat(_hdc, &_pfd);
		SetPixelFormat(_hdc, nPixelFormat, &_pfd);

		HGLRC tempOpenGLContext = wglCreateContext(_hdc);
		wglMakeCurrent(_hdc, tempOpenGLContext);

		glewInit();

		jgl::Int attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, _majorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, _minorVersion,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0
		};

		if (wglewIsSupported("WGL_ARB_create_context") == 1)
		{
			_hrc = wglCreateContextAttribsARB(_hdc, NULL, attributes);
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(tempOpenGLContext);
			wglMakeCurrent(_hdc, _hrc);
		}

		jgl::Int glVersion[2] = { -1, -1 };
		glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
		glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

		jgl::cout << "Creating a windows with OpenGL version " << glVersion[0] << "." << glVersion[1] << jgl::endl;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glFrontFace(GL_CCW);
	}

	OpenGLContext::OpenGLContext()
	{
		_convertedTitle = nullptr;
	}
	OpenGLContext::~OpenGLContext()
	{
		if (_convertedTitle != nullptr)
			delete[] _convertedTitle;

		wglMakeCurrent(_hdc, NULL);
		wglDeleteContext(_hrc);
		ReleaseDC(_windowFrame, _hdc);
	}

	void OpenGLContext::initialize(std::string p_title, jgl::Vector2Int p_size, jgl::Int p_major_version, jgl::Int p_minor_version)
	{
		_majorVersion = p_major_version;
		_minorVersion = p_minor_version;

		_convertedTitle = new jgl::WChar[p_title.size() + 1];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, _convertedTitle, p_title.size() + 1, p_title.c_str(), _TRUNCATE);

		_createGhostInstance();
		_createWindowClass();

		RegisterClass(&_windowClass);

		_setupWindowSize(p_size);

		resize(p_size.x(), p_size.y());

		_composeOpenGLContext();

		ShowWindow(_windowFrame, SW_SHOW);
		UpdateWindow(_windowFrame);
		wglSwapIntervalEXT(0);
	}

	void OpenGLContext::setup(jgl::Color background)
	{
		setViewport(0, 0, _size.x(), _size.y());
		glClearColor(background.r, background.g, background.b, background.a);
	}

	void OpenGLContext::resize(jgl::Int w, jgl::Int h)
	{
		_size.x() = w;
		_size.y() = h;
		setViewport(0, 0, _size.x(), _size.y());
	}

	void OpenGLContext::clear()
	{
		setViewport(0, 0, _size.x(), _size.y());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLContext::render()
	{
		SwapBuffers(_hdc);
	}

	void OpenGLContext::setViewport(Int p_x, Int p_y, Int p_w, Int p_h)
	{
		_actualSize = Vector2Int(p_w, p_h);
		glViewport(p_x, _size.y() - p_y - p_h, p_w, p_h);
	}

	const jgl::Vector2Int& OpenGLContext::size() const
	{
		return (_actualSize);
	}
}