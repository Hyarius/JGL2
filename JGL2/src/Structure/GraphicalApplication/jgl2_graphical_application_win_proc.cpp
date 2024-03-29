#include "Structure/jgl2_openGLContext.h"
#include "Structure/jgl2_graphical_application.h"
#include "jgl2_includes.h"

namespace jgl
{
	std::map<jgl::UInt, std::string> GraphicalApplication::WinMessageToString =
	{
	  { 0, "WM_NULL"},
	  { 1, "WM_CREATE" },
	  { 2, "WM_DESTROY" },
	  { 3, "WM_MOVE" },
	  { 5, "WM_SIZE" },
	  { 6, "WM_ACTIVATE" },
	  { 7, "WM_SETFOCUS" },
	  { 8, "WM_KILLFOCUS" },
	  { 10, "WM_ENABLE" },
	  { 11, "WM_SETREDRAW" },
	  { 12, "WM_SETTEXT" },
	  { 13, "WM_GETTEXT" },
	  { 14, "WM_GETTEXTLENGTH" },
	  { 15, "WM_PAINT" },
	  { 16, "WM_CLOSE" },
	  { 17, "WM_QUERYENDSESSION" },
	  { 18, "WM_QUIT" },
	  { 19, "WM_QUERYOPEN" },
	  { 20, "WM_ERASEBKGND" },
	  { 21, "WM_SYSCOLORCHANGE" },
	  { 22, "WM_ENDSESSION" },
	  { 24, "WM_SHOWWINDOW" },
	  { 25, "WM_CTLCOLOR" },
	  { 26, "WM_WININICHANGE" },
	  { 27, "WM_DEVMODECHANGE" },
	  { 28, "WM_ACTIVATEAPP" },
	  { 29, "WM_FONTCHANGE" },
	  { 30, "WM_TIMECHANGE" },
	  { 31, "WM_CANCELMODE" },
	  { 32, "WM_SETCURSOR" },
	  { 33, "WM_MOUSEACTIVATE" },
	  { 34, "WM_CHILDACTIVATE" },
	  { 35, "WM_QUEUESYNC" },
	  { 36, "WM_GETMINMAXINFO" },
	  { 38, "WM_PAINTICON" },
	  { 39, "WM_ICONERASEBKGND" },
	  { 40, "WM_NEXTDLGCTL" },
	  { 42, "WM_SPOOLERSTATUS" },
	  { 43, "WM_DRAWITEM" },
	  { 44, "WM_MEASUREITEM" },
	  { 45, "WM_DELETEITEM" },
	  { 46, "WM_VKEYTOITEM" },
	  { 47, "WM_CHARTOITEM" },
	  { 48, "WM_SETFONT" },
	  { 49, "WM_GETFONT" },
	  { 50, "WM_SETHOTKEY" },
	  { 51, "WM_GETHOTKEY" },
	  { 55, "WM_QUERYDRAGICON" },
	  { 57, "WM_COMPAREITEM" },
	  { 61, "WM_GETOBJECT" },
	  { 65, "WM_COMPACTING" },
	  { 68, "WM_COMMNOTIFY" },
	  { 70, "WM_WINDOWPOSCHANGING" },
	  { 71, "WM_WINDOWPOSCHANGED" },
	  { 72, "WM_POWER" },
	  { 73, "WM_COPYGLOBALDATA" },
	  { 74, "WM_COPYDATA" },
	  { 75, "WM_CANCELJOURNAL" },
	  { 78, "WM_NOTIFY" },
	  { 80, "WM_INPUTLANGCHANGEREQUEST" },
	  { 81, "WM_INPUTLANGCHANGE" },
	  { 82, "WM_TCARD" },
	  { 83, "WM_HELP" },
	  { 84, "WM_USERCHANGED" },
	  { 85, "WM_NOTIFYFORMAT" },
	  { 123, "WM_CONTEXTMENU" },
	  { 124, "WM_STYLECHANGING" },
	  { 125, "WM_STYLECHANGED" },
	  { 126, "WM_DISPLAYCHANGE" },
	  { 127, "WM_GETICON" },
	  { 128, "WM_SETICON" },
	  { 129, "WM_NCCREATE" },
	  { 130, "WM_NCDESTROY" },
	  { 131, "WM_NCCALCSIZE" },
	  { 132, "WM_NCHITTEST" },
	  { 133, "WM_NCPAINT" },
	  { 134, "WM_NCACTIVATE" },
	  { 135, "WM_GETDLGCODE" },
	  { 136, "WM_SYNCPAINT" },
	  { 160, "WM_NCMOUSEMOVE" },
	  { 161, "WM_NCLBUTTONDOWN" },
	  { 162, "WM_NCLBUTTONUP" },
	  { 163, "WM_NCLBUTTONDBLCLK" },
	  { 164, "WM_NCRBUTTONDOWN" },
	  { 165, "WM_NCRBUTTONUP" },
	  { 166, "WM_NCRBUTTONDBLCLK" },
	  { 167, "WM_NCMBUTTONDOWN" },
	  { 168, "WM_NCMBUTTONUP" },
	  { 169, "WM_NCMBUTTONDBLCLK" },
	  { 171, "WM_NCXBUTTONDOWN" },
	  { 172, "WM_NCXBUTTONUP" },
	  { 173, "WM_NCXBUTTONDBLCLK" },
	  { 176, "EM_GETSEL" },
	  { 177, "EM_SETSEL" },
	  { 178, "EM_GETRECT" },
	  { 179, "EM_SETRECT" },
	  { 180, "EM_SETRECTNP" },
	  { 181, "EM_SCROLL" },
	  { 182, "EM_LINESCROLL" },
	  { 183, "EM_SCROLLCARET" },
	  { 185, "EM_GETMODIFY" },
	  { 187, "EM_SETMODIFY" },
	  { 188, "EM_GETLINECOUNT" },
	  { 189, "EM_LINEINDEX" },
	  { 190, "EM_SETHANDLE" },
	  { 191, "EM_GETHANDLE" },
	  { 192, "EM_GETTHUMB" },
	  { 193, "EM_LINELENGTH" },
	  { 194, "EM_REPLACESEL" },
	  { 195, "EM_SETFONT" },
	  { 196, "EM_GETLINE" },
	  { 197, "EM_LIMITTEXT" },
	  { 197, "EM_SETLIMITTEXT" },
	  { 198, "EM_CANUNDO" },
	  { 199, "EM_UNDO" },
	  { 200, "EM_FMTLINES" },
	  { 201, "EM_LINEFROMCHAR" },
	  { 202, "EM_SETWORDBREAK" },
	  { 203, "EM_SETTABSTOPS" },
	  { 204, "EM_SETPASSWORDCHAR" },
	  { 205, "EM_EMPTYUNDOBUFFER" },
	  { 206, "EM_GETFIRSTVISIBLELINE" },
	  { 207, "EM_SETREADONLY" },
	  { 209, "EM_SETWORDBREAKPROC" },
	  { 209, "EM_GETWORDBREAKPROC" },
	  { 210, "EM_GETPASSWORDCHAR" },
	  { 211, "EM_SETMARGINS" },
	  { 212, "EM_GETMARGINS" },
	  { 213, "EM_GETLIMITTEXT" },
	  { 214, "EM_POSFROMCHAR" },
	  { 215, "EM_CHARFROMPOS" },
	  { 216, "EM_SETIMESTATUS" },
	  { 217, "EM_GETIMESTATUS" },
	  { 224, "SBM_SETPOS" },
	  { 225, "SBM_GETPOS" },
	  { 226, "SBM_SETRANGE" },
	  { 227, "SBM_GETRANGE" },
	  { 228, "SBM_ENABLE_ARROWS" },
	  { 230, "SBM_SETRANGEREDRAW" },
	  { 233, "SBM_SETSCROLLINFO" },
	  { 234, "SBM_GETSCROLLINFO" },
	  { 235, "SBM_GETSCROLLBARINFO" },
	  { 240, "BM_GETCHECK" },
	  { 241, "BM_SETCHECK" },
	  { 242, "BM_GETSTATE" },
	  { 243, "BM_SETSTATE" },
	  { 244, "BM_SETSTYLE" },
	  { 245, "BM_CLICK" },
	  { 246, "BM_GETIMAGE" },
	  { 247, "BM_SETIMAGE" },
	  { 248, "BM_SETDONTCLICK" },
	  { 255, "WM_INPUT" },
	  { 256, "WM_KEYDOWN" },
	  { 256, "WM_KEYFIRST" },
	  { 257, "WM_KEYUP" },
	  { 258, "WM_CHAR" },
	  { 259, "WM_DEADCHAR" },
	  { 260, "WM_SYSKEYDOWN" },
	  { 261, "WM_SYSKEYUP" },
	  { 262, "WM_SYSCHAR" },
	  { 263, "WM_SYSDEADCHAR" },
	  { 264, "WM_KEYLAST" },
	  { 265, "WM_UNICHAR" },
	  { 265, "WM_WNT_CONVERTREQUESTEX" },
	  { 266, "WM_CONVERTREQUEST" },
	  { 267, "WM_CONVERTRESULT" },
	  { 268, "WM_INTERIM" },
	  { 269, "WM_IME_STARTCOMPOSITION" },
	  { 270, "WM_IME_ENDCOMPOSITION" },
	  { 271, "WM_IME_COMPOSITION" },
	  { 271, "WM_IME_KEYLAST" },
	  { 272, "WM_INITDIALOG" },
	  { 273, "WM_COMMAND" },
	  { 274, "WM_SYSCOMMAND" },
	  { 275, "WM_TIMER" },
	  { 276, "WM_HSCROLL" },
	  { 277, "WM_VSCROLL" },
	  { 278, "WM_INITMENU" },
	  { 279, "WM_INITMENUPOPUP" },
	  { 280, "WM_SYSTIMER" },
	  { 287, "WM_MENUSELECT" },
	  { 288, "WM_MENUCHAR" },
	  { 289, "WM_ENTERIDLE" },
	  { 290, "WM_MENURBUTTONUP" },
	  { 291, "WM_MENUDRAG" },
	  { 292, "WM_MENUGETOBJECT" },
	  { 293, "WM_UNINITMENUPOPUP" },
	  { 294, "WM_MENUCOMMAND" },
	  { 295, "WM_CHANGEUISTATE" },
	  { 296, "WM_UPDATEUISTATE" },
	  { 297, "WM_QUERYUISTATE" },
	  { 306, "WM_CTLCOLORMSGBOX" },
	  { 307, "WM_CTLCOLOREDIT" },
	  { 308, "WM_CTLCOLORLISTBOX" },
	  { 309, "WM_CTLCOLORBTN" },
	  { 310, "WM_CTLCOLORDLG" },
	  { 311, "WM_CTLCOLORSCROLLBAR" },
	  { 312, "WM_CTLCOLORSTATIC" },
	  { 512, "WM_MOUSEFIRST" },
	  { 512, "WM_MOUSEMOVE" },
	  { 513, "WM_LBUTTONDOWN" },
	  { 514, "WM_LBUTTONUP" },
	  { 515, "WM_LBUTTONDBLCLK" },
	  { 516, "WM_RBUTTONDOWN" },
	  { 517, "WM_RBUTTONUP" },
	  { 518, "WM_RBUTTONDBLCLK" },
	  { 519, "WM_MBUTTONDOWN" },
	  { 520, "WM_MBUTTONUP" },
	  { 521, "WM_MBUTTONDBLCLK" },
	  { 521, "WM_MOUSELAST" },
	  { 522, "WM_MOUSEWHEEL" },
	  { 523, "WM_XBUTTONDOWN" },
	  { 524, "WM_XBUTTONUP" },
	  { 525, "WM_XBUTTONDBLCLK" },
	  { 528, "WM_PARENTNOTIFY" },
	  { 529, "WM_ENTERMENULOOP" },
	  { 530, "WM_EXITMENULOOP" },
	  { 531, "WM_NEXTMENU" },
	  { 532, "WM_SIZING" },
	  { 533, "WM_CAPTURECHANGED" },
	  { 534, "WM_MOVING" },
	  { 536, "WM_POWERBROADCAST" },
	  { 537, "WM_DEVICECHANGE" },
	  { 544, "WM_MDICREATE" },
	  { 545, "WM_MDIDESTROY" },
	  { 546, "WM_MDIACTIVATE" },
	  { 547, "WM_MDIRESTORE" },
	  { 548, "WM_MDINEXT" },
	  { 549, "WM_MDIMAXIMIZE" },
	  { 550, "WM_MDITILE" },
	  { 551, "WM_MDICASCADE" },
	  { 552, "WM_MDIICONARRANGE" },
	  { 553, "WM_MDIGETACTIVE" },
	  { 560, "WM_MDISETMENU" },
	  { 561, "WM_ENTERSIZEMOVE" },
	  { 562, "WM_EXITSIZEMOVE" },
	  { 563, "WM_DROPFILES" },
	  { 564, "WM_MDIREFRESHMENU" },
	  { 640, "WM_IME_REPORT" },
	  { 641, "WM_IME_SETCONTEXT" },
	  { 642, "WM_IME_NOTIFY" },
	  { 643, "WM_IME_CONTROL" },
	  { 644, "WM_IME_COMPOSITIONFULL" },
	  { 645, "WM_IME_SELECT" },
	  { 646, "WM_IME_CHAR" },
	  { 648, "WM_IME_REQUEST" },
	  { 656, "WM_IMEKEYDOWN" },
	  { 656, "WM_IME_KEYDOWN" },
	  { 657, "WM_IMEKEYUP" },
	  { 657, "WM_IME_KEYUP" },
	  { 672, "WM_NCMOUSEHOVER" },
	  { 673, "WM_MOUSEHOVER" },
	  { 674, "WM_NCMOUSELEAVE" },
	  { 675, "WM_MOUSELEAVE" },
	  { 768, "WM_CUT" },
	  { 769, "WM_COPY" },
	  { 770, "WM_PASTE" },
	  { 771, "WM_CLEAR" },
	  { 772, "WM_UNDO" },
	  { 773, "WM_RENDERFORMAT" },
	  { 774, "WM_RENDERALLFORMATS" },
	  { 775, "WM_DESTROYCLIPBOARD" },
	  { 776, "WM_DRAWCLIPBOARD" },
	  { 777, "WM_PAINTCLIPBOARD" },
	  { 778, "WM_VSCROLLCLIPBOARD" },
	  { 779, "WM_SIZECLIPBOARD" },
	  { 780, "WM_ASKCBFORMATNAME" },
	  { 781, "WM_CHANGECBCHAIN" },
	  { 782, "WM_HSCROLLCLIPBOARD" },
	  { 783, "WM_QUERYNEWPALETTE" },
	  { 784, "WM_PALETTEISCHANGING" },
	  { 785, "WM_PALETTECHANGED" },
	  { 786, "WM_HOTKEY" },
	  { 791, "WM_PRINT" },
	  { 792, "WM_PRINTCLIENT" },
	  { 793, "WM_APPCOMMAND" },
	  { 856, "WM_HANDHELDFIRST" },
	  { 863, "WM_HANDHELDLAST" },
	  { 864, "WM_AFXFIRST" },
	  { 895, "WM_AFXLAST" },
	  { 896, "WM_PENWINFIRST" },
	  { 897, "WM_RCRESULT" },
	  { 898, "WM_HOOKRCRESULT" },
	  { 899, "WM_GLOBALRCCHANGE" },
	  { 899, "WM_PENMISCINFO" },
	  { 900, "WM_SKB" },
	  { 901, "WM_HEDITCTL" },
	  { 901, "WM_PENCTL" },
	  { 902, "WM_PENMISC" },
	  { 903, "WM_CTLINIT" },
	  { 904, "WM_PENEVENT" },
	  { 911, "WM_PENWINLAST" },
	  { 1024, "WM_USER" }
	};

	void GraphicalApplication::_pullWinMessage()
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // If we have a message to process, process it
			int translateResult = TranslateMessage(&msg);
			int dispatchResult = DispatchMessage(&msg);
		}
	}

	void GraphicalApplication::_handleWinMessage()
	{
		while (_messagesToTreat.empty() == false)
		{
			jgl::PolymorphicContainer* tmpMessage = _getWinMessageToTreat();
			
			jgl::UInt messageId;
			*tmpMessage >> messageId;

			switch (messageId)
			{
			case WM_DESTROY:
			{
				quit();
				break;
			}
			case WM_MOVE:
			{
				break;
			}
			case WM_MDIRESTORE:
			{
				break;
			}
			case WM_SETFOCUS:
			{
				break;
			}
			case WM_KILLFOCUS:
			{
				break;
			}

			// ----- Mouse part
			case WM_LBUTTONDOWN:
			{
				if (_mouse._buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Left)] == jgl::InputStatus::Up)
					_mouse._buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Left)] = jgl::InputStatus::Pressed;
				break;
			}
			case WM_MBUTTONDOWN:
			{
				if (_mouse._buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Center)] == jgl::InputStatus::Up)
					_mouse._buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Center)] = jgl::InputStatus::Pressed;
				break;
			}
			case WM_RBUTTONDOWN:
			{
				if (_mouse._buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Right)] == jgl::InputStatus::Up)
					_mouse._buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Right)] = jgl::InputStatus::Pressed;
				break;
			}
			case WM_LBUTTONUP:
			{
				_mouse._buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Left)] = jgl::InputStatus::Released;
				break;
			}
			case WM_MBUTTONUP:
			{
				_mouse._buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Center)] = jgl::InputStatus::Released;
				break;
			}
			case WM_RBUTTONUP:
			{
				_mouse._buttons[static_cast<jgl::Int>(jgl::Mouse::Button::Right)] = jgl::InputStatus::Released;
				break;
			}
			case WM_MOUSEWHEEL:
			{
				jgl::Short value;

				*tmpMessage >> value;
				jgl::Float delta = value / 100.0f;

				_mouse._wheel += delta;
				break;
			}
			case WM_MOUSEMOVE:
			{
				*tmpMessage >> _mouse._pos.values[0];
				*tmpMessage >> _mouse._pos.values[1];

				break;
			}

			// ----- Keyboard part
			case WM_CHAR:
			{
				jgl::UInt value;

				*tmpMessage >> value;

				_keyboard._entry = static_cast<jgl::UChar>(value);
				break;
			}
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				jgl::UInt value;

				*tmpMessage >> value;
				
				if (_keyboard._keys[value] == jgl::InputStatus::Up)
					_keyboard._keys[value] = jgl::InputStatus::Pressed;
				break;
			}
			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				jgl::UInt value;

				*tmpMessage >> value;

				_keyboard._keys[value] = jgl::InputStatus::Released;
				break;
			}
			case WM_QUIT:
			case WM_CLOSE:
			case WM_NCHITTEST :
			case WM_SETCURSOR :
			case WM_NCMOUSEMOVE :
			case WM_CAPTURECHANGED :
			case WM_NCMOUSELEAVE :
			case WM_WINDOWPOSCHANGING :
			case WM_EXITSIZEMOVE:
			case WM_MOVING:
			case WM_NCLBUTTONDOWN:
			case WM_SYSCOMMAND:
			case WM_GETMINMAXINFO:
			case WM_WINDOWPOSCHANGED:
			case WM_PAINT:
			case WM_NCPAINT:
			case WM_ERASEBKGND:
			case WM_NCCREATE:
			case WM_NCCALCSIZE:
			case WM_SHOWWINDOW:
			case WM_ACTIVATEAPP:
			case WM_CREATE:
			case WM_NCACTIVATE:
			case WM_GETICON:
			case WM_IME_SETCONTEXT:
			case WM_ACTIVATE:
			case WM_IME_NOTIFY:
			case WM_ENTERSIZEMOVE:
			case WM_SIZING:
			case WM_MOUSEACTIVATE:
			case WM_DEADCHAR:
			case WM_CONTEXTMENU:
			case WM_ENTERIDLE:
			case WM_MENUSELECT:
			case WM_ENTERMENULOOP:
			case WM_EXITMENULOOP:
			case WM_INITMENU:
			case WM_NCDESTROY:
			case WM_QUERYOPEN:
			case WM_SIZE:
			case WM_SYNCPAINT:
			case 799:
			case 49395:
			case 49367:
			case 144:
			{
				//Ignored message
				break;
			}
			default :
			{
				jgl::cout << "Unexpected message received : ";
				if (WinMessageToString.count(messageId) == 0)
					jgl::cout << "Unregistred message in data base [" << messageId << "]";
				else
					jgl::cout << WinMessageToString[messageId];
				jgl::cout << jgl::endl;
				break;
			}
			}
			_releaseWinMessage(tmpMessage);
		}
	}

	jgl::PolymorphicContainer* GraphicalApplication::_obtainWinMessage()
	{
		jgl::PolymorphicContainer* result = _messagePool.obtain();

		return (result);
	}
	void GraphicalApplication::_releaseWinMessage(jgl::PolymorphicContainer* p_msg)
	{
		_messagePool.release(p_msg);
	}

	void jgl::GraphicalApplication::_insertWinMessageToTreat(jgl::PolymorphicContainer* p_msg)
	{
		_messagesToTreat.push_back(p_msg);
	}

	jgl::PolymorphicContainer* jgl::GraphicalApplication::_getWinMessageToTreat()
	{
		jgl::PolymorphicContainer* result = _messagesToTreat.front();
		_messagesToTreat.pop_front();
		return (result);
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

		jgl::PolymorphicContainer* newMessage = jgl::GraphicalApplication::instance()->_obtainWinMessage();

		newMessage->clear();

		*newMessage << message;
		switch (message) {
		case WM_SIZE:
		{
			jgl::UInt width = LOWORD(lParam);
			jgl::UInt height = HIWORD(lParam);

			if (width != 0 && height != 0)
			{
				jgl::GraphicalApplication::instance()->resize(jgl::Vector2Int(width, height));
			}

			jgl::GraphicalApplication::instance()->_releaseWinMessage(newMessage);
			
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL:
		{
			jgl::Short value = GET_WHEEL_DELTA_WPARAM(wParam);

			*newMessage << value;

			break;
		}
		case WM_MOUSEMOVE:
		{
			jgl::Int x = LOWORD(lParam);
			jgl::Int y = HIWORD(lParam);

			*newMessage << x;
			*newMessage << y;

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
			break;
		}
		}

		jgl::GraphicalApplication::instance()->_insertWinMessageToTreat(newMessage);

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}