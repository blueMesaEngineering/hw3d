#include <Windows.h>
#include "WindowsMessageMap.h"
#include <sstream>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageMap mm;
	OutputDebugStringA(mm(msg, lParam, wParam).c_str()); // Had to typecast to the (LPWSTR) type.  NDGuthrie 20230204.  See here: https://learn.microsoft.com/en-us/answers/questions/483237/a-value-of-type-const-char-*-cannot-be-assigned-to
														 // Edit 2: Had to use OutputDebugStringA(), not OutputDebugString().  Difference being 16 bit vs 8 bit: https://stackoverflow.com/questions/64897414/error-c2664-messageboxw-cannot-convert-argument-2-from-const-char-to-lpcwst
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			SetWindowTextA(hWnd, "Respects");
		}
		break;
	case WM_KEYUP:
		if (wParam == 'F')
		{
			SetWindowTextA(hWnd, "Dangerfield");
		}
		break;
	case WM_CHAR:
		{
			static std::string title;
			title.push_back((char)wParam);
			SetWindowTextA(hWnd, title.c_str());
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINTS pt = MAKEPOINTS(lParam);
			std::ostringstream oss;
			oss << "(" << pt.x << "," << pt.y << ")";
			SetWindowTextA(hWnd, oss.str().c_str());
		}
		break;
	}
	//return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int		  nCmdShow )
{
	const auto pClassName = L"hw3dbutts";  // NDGuthrie 20230203 - Had to add the 'L' to the beginning of the string as per the instructions here: https://stackoverflow.com/questions/60044576/e0513-a-value-of-type-const-char-cannot-be-assigned-to-an-entity-of-type-lp
	//// register window class
	//WNDCLASSEX wc = { 0 };
	//wc.cbSize = sizeof(wc);
	//wc.style = CS_OWNDC;
	//wc.lpfnWndProc = WndProc;
	//wc.cbClsExtra = 0;
	//wc.cbWndExtra = 0;
	//wc.hInstance = hInstance;
	//wc.hIcon = nullptr;
	//wc.hCursor = nullptr;
	//wc.hbrBackground = nullptr;
	//wc.lpszMenuName = nullptr;
	//wc.lpszClassName = pClassName;
	//wc.hIconSm = nullptr;
	//RegisterClassEx( &wc );
	// create window instance
	//HWND hWnd = CreateWindowEx(
	//	  0
	//	, pClassName
	//	, L"Happy Hard Window"
	//	, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU
	//	, 200
	//	, 200
	//	, 640
	//	, 480
	//	, nullptr
	//	, nullptr
	//	, hInstance
	//	, nullptr
	//);
	//ShowWindow(hWnd, SW_SHOW);

	// message pump
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}