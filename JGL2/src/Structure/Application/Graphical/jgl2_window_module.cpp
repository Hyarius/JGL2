#include "Structure/Application/Graphical/jgl2_window_module.h"
#include "Structure/Application/Graphical/jgl2_windows_api_module.h"

namespace jgl::Application::Module
{
	void Window::_convertTitle(std::string p_title)
	{
		_convertedTitle = new wchar_t[p_title.size() + 1];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, _convertedTitle, p_title.size() + 1, p_title.c_str(), _TRUNCATE);
	}

	void Window::_createGhostInstance()
	{
		_hInstance = GetModuleHandle(NULL);
	}

	void Window::_registerWindowClass()
	{
		_windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		_windowClass.lpfnWndProc = (WNDPROC)jgl::Application::Module::WindowsAPI::WindowProc;
		_windowClass.cbClsExtra = 0;
		_windowClass.cbWndExtra = 0;
		_windowClass.hInstance = _hInstance;
		_windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		_windowClass.hbrBackground = NULL;
		_windowClass.lpszMenuName = NULL;
		_windowClass.lpszClassName = _convertedTitle;

		RegisterClass(&_windowClass);
	}

	void Window::_createWindowFrame(jgl::Vector2Int p_size)
	{
		_windowSize.left = (long)0;
		_windowSize.right = (long)p_size.x;
		_windowSize.top = (long)0;
		_windowSize.bottom = (long)p_size.y;

		_windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		_windowStyle = WS_OVERLAPPEDWINDOW;

		AdjustWindowRectEx(&_windowSize, _windowStyle, FALSE, _windowExStyle);

		_windowFrame = CreateWindowEx(_windowExStyle, (LPCTSTR)(_convertedTitle), (LPCTSTR)(_convertedTitle), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, _windowSize.right - _windowSize.left, _windowSize.bottom - _windowSize.top, NULL, NULL, _hInstance, _api_module);
	}

	void Window::_composeOpenGLContext()
	{
		_hdc = GetDC(_windowFrame);

		memset(&_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

		_pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		_pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		_pfd.iPixelType = PFD_TYPE_RGBA;
		_pfd.cColorBits = 24;
		_pfd.cDepthBits = 32;
		_pfd.cStencilBits = 32;
		_pfd.iLayerType = PFD_MAIN_PLANE;

		int nPixelFormat = ChoosePixelFormat(_hdc, &_pfd);
		SetPixelFormat(_hdc, nPixelFormat, &_pfd);

		HGLRC tempOpenGLContext = wglCreateContext(_hdc);
		wglMakeCurrent(_hdc, tempOpenGLContext);

		glewInit();

		int attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, C_MAJOR_VERSION,
			WGL_CONTEXT_MINOR_VERSION_ARB, C_MINOR_VERSION,
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

		int glVersion[2] = { -1, -1 };
		glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
		glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0f);
		glDepthFunc(GL_LESS);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);

		glEnable(GL_SCISSOR_TEST);
	}

	void Window::_activateWindow()
	{
		ShowWindow(_windowFrame, SW_SHOW);
		UpdateWindow(_windowFrame);
		wglSwapIntervalEXT(0);
	}

	void Window::_treatMessage(jgl::PolymorphicContainer* p_message)
	{
		jgl::UInt messageId;
		*p_message >> messageId;

		switch (messageId)
		{
			case WM_SIZE:
				
				jgl::UInt width;
				jgl::UInt height;

				*p_message >> width;
				*p_message >> height;

				jgl::Vector2 ratio = jgl::Vector2(
					jgl::Float(width) / jgl::Float(size().x),
					jgl::Float(height) / jgl::Float(size().y)
				);

				resize(width, height);
				jgl::Abstract::Application::Core::instance()->_widgets.setRootWidgetGeometry(ratio);
			break;
		}
	}

	Window::Window(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat) :
		MessageConsumer(p_messageToTreat)
	{

	}

	void Window::createWindow(std::string p_title, jgl::Vector2Int p_size, jgl::Color p_backgroundColor)
	{
		_backgroundColor = p_backgroundColor;

		_convertTitle(p_title);

		_createGhostInstance();

		_registerWindowClass();

		_createWindowFrame(p_size);

		_composeOpenGLContext();

		resize(p_size.x, p_size.y);
		reset();

		_activateWindow();
	}

	void Window::reset()
	{
		setViewport(_size);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
		clear();
	}

	void Window::resize(int w, int h)
	{
		_size.x = w;
		_size.y = h;
		reset();
	}

	void Window::clear()
	{
		_origin = 0;
		glViewport(0, 0, _size.x, _size.y);
		glScissor(0, 0, _size.x, _size.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Window::render()
	{
		SwapBuffers(_hdc);
	}

	void Window::setOrigin(jgl::Vector2Int p_origin)
	{
		_origin = p_origin;
	}
	void Window::setViewport(jgl::Vector2Int p_viewportSize)
	{
		_viewportSize = p_viewportSize;
	}
	void Window::setScissorViewport(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
	{
		glScissor(p_anchor.x, _size.y - p_anchor.y - p_size.y, p_size.x, p_size.y);
	}
}