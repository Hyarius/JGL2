#pragma once

#include "structure/Application/Core/jgl2_application_core.h"
#include "structure/jgl2_color.h"
#include "structure/Application/Core/jgl2_message_consumer.h"

namespace jgl
{
	namespace Application
	{
		namespace Module
		{
			class Window : public Abstract::Application::Utils::MessageConsumer
			{
			public:
				static inline float C_MAX_DEPTH = 10000;
			private:
				static inline int C_MAJOR_VERSION = 4;
				static inline int C_MINOR_VERSION = 1;

				void* _api_module;

				wchar_t* _convertedTitle;
				HINSTANCE _hInstance;
				HWND _windowFrame;
				RECT _windowSize;
				WNDCLASS _windowClass;
				DWORD _windowStyle;
				DWORD _windowExStyle;

				HGLRC _hrc;
				HDC _hdc;
				PIXELFORMATDESCRIPTOR _pfd;

				jgl::Color _backgroundColor;
				Vector2Int _size;
				Vector2Int _viewportSize;
				jgl::Vector2Int _origin = 0;

				void _convertTitle(std::string p_title);
				void _createGhostInstance();
				void _registerWindowClass();
				void _createWindowFrame(jgl::Vector2Int p_size);
				void _composeOpenGLContext();
				void _activateWindow();

				void _treatMessage(jgl::PolymorphicContainer* p_message);

			public:
				Window(jgl::LockedQueue<jgl::PolymorphicContainer*>& p_messageToTreat);

				void connectToAPI(void* p_api_module) { _api_module = p_api_module; }
				void createWindow(std::string p_title, jgl::Vector2Int p_size, jgl::Color p_backgroundColor);

				const jgl::Vector2Int& origin() const { return (_origin); }
				const jgl::Vector2Int& size() const { return (_viewportSize); }
				void reset();
				void resize(int w, int h);
				void clear();
				void render();
				void setOrigin(jgl::Vector2Int p_origin);
				void setViewport(jgl::Vector2Int p_size);
				void setScissorViewport(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size);
			};
		}
	}
}