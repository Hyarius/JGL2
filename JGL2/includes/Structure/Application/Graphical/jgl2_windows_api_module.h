#pragma once

#include "structure/Application/Core/jgl2_application_core.h"
#include "structure/jgl2_color.h"
#include "structure/jgl2_pool.h"
#include "structure/jgl2_polymorphic_container.h"
#include "structure/jgl2_locked_queue.h"

namespace jgl
{
	namespace Application
	{
		namespace Module
		{
			class WindowsAPI
			{
			public:
				using MessagePool = jgl::Singleton<jgl::Pool<jgl::PolymorphicContainer>>;
				static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
				{
					WindowsAPI* pThis = NULL;

					if (uMsg == WM_NCCREATE)
					{
						CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
						pThis = (WindowsAPI*)pCreate->lpCreateParams;
						SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

						pThis->_windowFrame = hwnd;
					}
					else
					{
						pThis = (WindowsAPI*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
					}

					if (pThis)
					{
						return pThis->_handleMessage(uMsg, wParam, lParam);
					}
					else
					{
						return DefWindowProc(hwnd, uMsg, wParam, lParam);
					}
				}

			private:
				LRESULT _handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
				{
					jgl::PolymorphicContainer* newMessage = MessagePool::instance()->obtain();

					newMessage->clear();

					*newMessage << uMsg;
					switch (uMsg)
					{
					case WM_DESTROY:
					{
						_systemMessagesToTreat.push_back(newMessage);
						break;
					}
					case WM_MOVE:
					case WM_MDIRESTORE:
					case WM_SETFOCUS:
					case WM_KILLFOCUS:
					{
						_windowMessagesToTreat.push_back(newMessage);
						break;
					}
					case WM_SIZE:
					{
						jgl::UInt width = LOWORD(lParam);
						jgl::UInt height = HIWORD(lParam);

						*newMessage << width;
						*newMessage << height;

						_windowMessagesToTreat.push_back(newMessage);
						break;
					}

					case WM_LBUTTONDOWN:
					case WM_MBUTTONDOWN:
					case WM_RBUTTONDOWN:
					case WM_LBUTTONUP:
					case WM_MBUTTONUP:
					case WM_RBUTTONUP:
					{
						_mouseMessagesToTreat.push_back(newMessage);
						break;
					}
					case WM_MOUSEHWHEEL:
					case WM_MOUSEWHEEL:
					{
						jgl::Short value = GET_WHEEL_DELTA_WPARAM(wParam);

						*newMessage << value;

						_mouseMessagesToTreat.push_back(newMessage);
						break;
					}
					case WM_MOUSEMOVE:
					{
						jgl::Int x = LOWORD(lParam);
						jgl::Int y = HIWORD(lParam);

						*newMessage << x;
						*newMessage << y;

						_mouseMessagesToTreat.push_back(newMessage);
						break;
					}

					// ----- Keyboard part
					case WM_CHAR:
					case WM_KEYDOWN:
					case WM_KEYUP:
					case WM_HOTKEY:
					case WM_SYSKEYDOWN:
					case WM_SYSKEYUP:
					{
						jgl::UInt value = wParam;

						*newMessage << value;

						_keyboardMessagesToTreat.push_back(newMessage);
						break;
					}

					default:
					{
						MessagePool::instance()->release(newMessage);
						return DefWindowProc(_windowFrame, uMsg, wParam, lParam);
					}
					}
					return TRUE;
				}

				HWND _windowFrame;

				jgl::LockedQueue<jgl::PolymorphicContainer*> _mouseMessagesToTreat;
				jgl::LockedQueue<jgl::PolymorphicContainer*> _keyboardMessagesToTreat;
				jgl::LockedQueue<jgl::PolymorphicContainer*> _windowMessagesToTreat;
				jgl::LockedQueue<jgl::PolymorphicContainer*> _systemMessagesToTreat;

			public:
				WindowsAPI()
				{
					jgl::Singleton<jgl::Pool<jgl::PolymorphicContainer>>::instanciate();

					_windowFrame = NULL;

					jgl::Singleton<jgl::Pool<jgl::PolymorphicContainer>>::instance()->setDefaultObject(new jgl::PolymorphicContainer());
				}

				void pullWinMessage()
				{
					MSG msg = {};
					if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
						int translateResult = TranslateMessage(&msg);
						int dispatchResult = DispatchMessage(&msg);
					}
				}

				jgl::LockedQueue<jgl::PolymorphicContainer*>& mouseMessagesToTreat() { return (_mouseMessagesToTreat); }
				jgl::LockedQueue<jgl::PolymorphicContainer*>& keyboardMessagesToTreat() { return (_keyboardMessagesToTreat); }
				jgl::LockedQueue<jgl::PolymorphicContainer*>& windowMessagesToTreat() { return (_windowMessagesToTreat); }
				jgl::LockedQueue<jgl::PolymorphicContainer*>& systemMessagesToTreat() { return (_systemMessagesToTreat); }
			};
		}
	}
}