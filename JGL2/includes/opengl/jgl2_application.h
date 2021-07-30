#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

#include "structure/jgl2_string.h"
#include "structure/jgl2_vector.h"

#include "opengl/jgl2_shader.h"

#include "opengl/jgl2_viewport.h"

#include "structure/jgl2_mouse.h"
#include "structure/jgl2_keyboard.h"

#include "widget/jgl2_widget.h"


namespace jgl
{
	void draw_rectangle_color(jgl::Vector2Int pos, jgl::Vector2Int size, jgl::Color color, jgl::Float depth, jgl::Viewport* viewport);

	class Application
	{
	private:
		friend class Viewport;
		friend class Widget;

		class OpenGL_context
		{
		private:
			jgl::Vector2Int _size;

		protected:
			HGLRC _hrc;
			HDC _hdc;
			HWND _hwnd;
			PIXELFORMATDESCRIPTOR _pfd;

		public:
			OpenGL_context()
			{
				_hrc = nullptr;
				_hdc = nullptr;
				_hwnd = nullptr;
				memset(&_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
			}
			~OpenGL_context(void)
			{
				wglMakeCurrent(_hdc, NULL);
				wglDeleteContext(_hrc);
				ReleaseDC(_hwnd, _hdc);
			}
			jgl::Vector2Int size() { return (_size); }
			void create_opengl_context(HWND hwnd, jgl::Int major_version = 3, jgl::Int minor_version = 2)
			{
				_hwnd = hwnd;
				_hdc = GetDC(_hwnd);
				THROW_INFORMATION("Getting DC completed");

				// Pixelformat descriptor cleaning
				memset(&_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
				THROW_INFORMATION("Clearing pixelformatdescriptor");

				// Pixelformat descriptor definition
				_pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
				_pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
				_pfd.iPixelType = PFD_TYPE_RGBA;
				_pfd.cColorBits = 24;
				_pfd.cDepthBits = 32;
				_pfd.iLayerType = PFD_MAIN_PLANE;
				THROW_INFORMATION("Pixelformatdescriptor defined");

				// Pixelformat selection
				jgl::Int nPixelFormat = ChoosePixelFormat(_hdc, &_pfd);
				if (nPixelFormat == 0)
					THROW_EXCEPTION(jgl::Error_level::Critical, -7, "Can't choose a pixel format while creating a new opengl context");
				THROW_INFORMATION("ChoosePixelFormat defined");

				// Pixelformat definition
				bool bResult = SetPixelFormat(_hdc, nPixelFormat, &_pfd);
				if (!bResult)
					THROW_EXCEPTION(jgl::Error_level::Critical, -7, "Can't set a pixel format while creating a new opengl context");
				THROW_INFORMATION("SetPixelFormat defined");

				// Creating and using a context OpenGL2.1
				HGLRC tempOpenGLContext = wglCreateContext(_hdc);
				wglMakeCurrent(_hdc, tempOpenGLContext);
				THROW_INFORMATION("wglCreateContext completed");

				//Glew initiialization to start update context to opengl3+
				GLenum error = glewInit();
				if (error != GLEW_OK)
					THROW_EXCEPTION(jgl::Error_level::Critical, -7, "Can't initialize glew while creating a new opengl context");
				THROW_INFORMATION("glewInit completed");

				// OpenGL3+ attribut definition
				jgl::Int attributes[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, major_version,
				WGL_CONTEXT_MINOR_VERSION_ARB, minor_version,
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Configuration for opengl3+
				0
				};

				// If we can create such opengl context
				if (wglewIsSupported("WGL_ARB_create_context") == 1) {
					// Creating the new context
					_hrc = wglCreateContextAttribsARB(_hdc, NULL, attributes);

					// Unconnecting temporaty context created line 59
					wglMakeCurrent(NULL, NULL);

					// Deleting temporaty context created line 58
					wglDeleteContext(tempOpenGLContext);

					// Activating new context
					wglMakeCurrent(_hdc, _hrc); // On passe au contexte OpenGL 3.0
				}
				else {
					THROW_EXCEPTION(jgl::Error_level::Critical, -7, "OpenGL asked version not supported");
				}

				jgl::Int glVersion[2] = { -1, -1 }; // Des valeurs par défaut pour la version
				glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // On récupère la version majeure d'OpenGL utilisée
				glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // On récupère la version mineure d'OpenGL utilisée

				if (glVersion[0] != major_version || glVersion[1] != minor_version)
				{
					THROW_EXCEPTION(jgl::Error_level::Critical, -7, "OpenGL asked version not supported");
				}
				else
				{
					THROW_INFORMATION("Using OpenGL: " + jgl::itoa(glVersion[0]) + "." + jgl::itoa(glVersion[1]));
				}
			}
			void setup(jgl::Color background)
			{
				glClearColor(background.r, background.g, background.b, background.a);
			}
			void resize(jgl::Int w, jgl::Int h)
			{
				_size.x = w;
				_size.y = h;
				//THROW_INFORMATION("New resolution : " + _size.text());
			}
			void clear()
			{
				glViewport(0, 0, _size.x, _size.y);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}
			void render(void)
			{
				SwapBuffers(_hdc);
			}
		};

		class Central_widget : public jgl::Widget
		{
		private:

			virtual void _on_geometry_change(jgl::Vector2Int p_anchor, jgl::Vector2Int p_area, jgl::Float p_depth);
			virtual void _render();

		public:
			Central_widget(jgl::Widget* p_parent);
		};

		jgl::String _title = "Default title";
		jgl::Vector2Int _size = jgl::Vector2Int(820, 640);
		jgl::Color _background = jgl::Color(50, 50, 50);
		jgl::Bool _play = true;
		jgl::Bool _masked = false;

		jgl::Ulong _time = 0u;
		jgl::Ulong _tick_delta = 1000 / 60;

		jgl::Mouse _mouse;
		jgl::Keyboard _keyboard;

		jgl::Map<jgl::String, jgl::Shader*> _shader_map;
		jgl::Array<class Image*> _images_array;
		jgl::Array<class Font*> _font_array;

		class jgl::Font* _default_font = nullptr;

		HWND _window_frame;
		HINSTANCE _hInstance;
		OpenGL_context _opengl_context;

		const jgl::Viewport* _active_viewport;
		Central_widget* _central_widget;

		void _create_window(jgl::String title, jgl::Int width, jgl::Int height, jgl::Uint major, jgl::Uint minor);

		static Application* _active_application;

		void _launch_draw_triangle_color();
		void _launch_draw_triangle_texture();
		void _launch_draw_triangle_font();

		void _clear();
		void _render();

		void _update_input();
		void _handle_win_message();

	public:
		/*
			Application constructor
			Need to give a title and a size

			Optional : 
			jgl::Color p_background - Custom color for the background
		*/
		Application(const jgl::String& p_title, const jgl::Vector2Int& p_size, jgl::Color p_background = jgl::Color(50, 50, 50));

		/*
			Application destructor
		*/
		~Application();

		/*
			Returns the FPS max defined by the user
		*/
		jgl::Ulong max_fps() { return (1000 * _tick_delta); }

		/*
			Set the max FPS
		*/
		void set_max_fps(jgl::Ulong nb_fps) { _tick_delta = 1000 / nb_fps; }

		/*
			Returns the base font for the application

			/!\ INITIALIZED TO NULLPTR AT CREATION /!\
		*/
		class jgl::Font* default_font() { return (_default_font); }

		/*
			Define the default font of the application
		*/
		void set_default_font(class jgl::Font* p_font) { _default_font = p_font; }

		/*
			Static method

			Define the active application, on which calculation will be made

			By default, the active application is edited at each new application created
		*/
		static void set_active_application(jgl::Application* p_app)
		{
			_active_application = p_app;
			if (p_app != nullptr)
			{
				THROW_INFORMATION("Main application swap to " + _active_application->title());
			}
			else
				THROW_INFORMATION("Main application set to NULL");
		}

		/*
			Static method

			Returns the active application
		*/
		static Application* active_application() { if (_active_application == nullptr)THROW_EXCEPTION(jgl::Error_level::Critical, 1, "No application started"); return (_active_application); }

		/*
			Resize the window to a desired size exprimed by X en Y, in pixel
		*/
		void resize(jgl::Int x, jgl::Int y);

		/*
			Returns the actual size of the window, in pixel
		*/
		const jgl::Vector2Int size() const { return (_size); }

		/*
			Returns the title of the window
		*/
		const jgl::String& title() { return (_title); }

		/*
			Returns the current tick of the application, relative to the start of the computer
		*/
		const jgl::Ulong& time() { return (_time); }

		/*
			WinProc method

			Do not use it

			Define the mask status of the window
		*/
		void set_masked(jgl::Bool p_masked) { _masked = p_masked; }

		/*
			Returns the mask status of the window :
			- true : The window is visible by the user
			- false : The window isn't visible by the user
		*/
		jgl::Bool masked() { return (_masked); }

		/*
			Return the active viewport
		*/
		const jgl::Viewport* active_viewport() const { return (_active_viewport); }

		/*
			Set the active viewport
		*/
		void set_active_viewport(const jgl::Viewport* new_viewport) { _active_viewport = new_viewport; }

		/*
			Return the shader named [p_name]

			If the shader don't exist, return nullptr
		*/
		jgl::Shader* shader(jgl::String p_name) const { if (_shader_map.count(p_name) == 0)return (nullptr); return (_shader_map.at(p_name)); }

		/*
			Add the shader [p_name] from file [vertex_shader_path] and [fragment_shader_path] and return a pointer to this shader
		*/
		jgl::Shader* add_shader_from_file(jgl::String name, jgl::String vertex_shader_path, jgl::String fragment_shader_path);

		/*
			Add the shader [p_name] from GLSL code [vertex_content] and [fragment_content] and return a pointer to this shader
		*/
		jgl::Shader* add_shader(jgl::String name, jgl::String vertex_content, jgl::String fragment_content);

		/*
			Return the mouse linked to this application
		*/
		Mouse& mouse() { return (_mouse); }

		/*
			Return the keyboard linked to this application
		*/
		Keyboard& keyboard() { return (_keyboard); }

		/*
			Start the application

			Return 0 if everything end up correctly
		*/
		jgl::Int run();

		/*
			Stop the application
		*/
		void quit();
	};
}