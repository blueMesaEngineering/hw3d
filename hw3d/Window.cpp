#include "Window.h"
#include <sstream>
#include "resource.h"

// Window Class Stuff
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	// register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();;
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName(); 
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass((LPCWSTR)wndClassName, GetInstance());
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

// Window Stuff
Window::Window(int width, int height, const wchar_t* name)
	:
	width(width),
	height(height)
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}

	//throw CHWND_EXCEPT(ERROR_ARENA_TRASHED);
	//throw std::runtime_error("butts butts buuuuuuuuutttttttsssssss");

	// create window and get hWnd
	hWnd = CreateWindowEx(	// Had to edit to work with CreateWindowEx
		0
		/*, (LPCWSTR)WindowClass::GetName()
		, (LPCWSTR)name*/
		, WindowClass::GetName()
		, name
		, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, wr.right - wr.left
		, wr.bottom - wr.top
		, nullptr
		, nullptr
		, WindowClass::GetInstance()
		, this
	);

	// check for error
	if (hWnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}

	// newly created windows start off as hidden
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	// create graphics object
	pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::wstring& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)	// Simple typecast. NDG 202302230014
	{
		throw CHWND_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// check for quit because peekmessage does not signal this via return
		if (msg.message == WM_QUIT)
		{
			// return optional wrapping int (arg to PostQuitMessage is in wParam
			return msg.wParam;
		}

		// TranslateMessage will post auxiliary WM_CHAR message from key msg
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// return empty optional when not quitting app
	return {};
}

Graphics& Window::Gfx()
{
	if (!pGfx)
	{
		throw CHWND_NOGFX_EXCEPT();
	}
	return *pGfx;
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindowEx() to store window class pointer
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));  // What do you mean? Now we can finally play the game....
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	// clear keystate when window loses focus to prevent input from getting lost
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;

	/******************** KEYBOARD MESSAGES ********************/
	case WM_KEYDOWN:
	// syskey commands need to be handled to track ALT key (VK_MENU)
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000 || kbd.AutorepeatIsEnabled()))  // Filter autorepeat
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	/******************** END KEYBOARD MESSAGES ********************/

	/******************** MOUSE MESSAGES ********************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// In client region -> log move, and log enter + capture mouse (if not previously in window)
		if(pt.x >= 0 && pt.x < width && pt.y >=0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else {
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);

	}

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Window Exception Stuff
Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}

const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "Chili Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr
		, hr
		, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
		, reinterpret_cast<LPWSTR>(&pMsgBuf)
		, 0
		, nullptr
	);
	if (nMsgLen == 0)
	{
		return "Unidentified Error Code";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::HrException::GetErrorDescription() const noexcept
{
	return Exception::TranslateErrorCode(hr);
}

const char* Window::NoGfxException::GetType() const noexcept
{
	return "Chili Window Exception [No Graphics]";
}