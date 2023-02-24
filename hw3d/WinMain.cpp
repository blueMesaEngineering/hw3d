//#include <Windows.h>
//#include "WindowsMessageMap.h"
#include <sstream>

#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int		  nCmdShow)
{

	try
	{
		Window wnd(800, 300, L"Donkey Fart Box With Noodles!!!");

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);


			/******************** MOUSE MESSAGE TEST ********************/
			while (!wnd.mouse.IsEmpty())
			{
				const auto e = wnd.mouse.Read();
				if (e.GetType() == Mouse::Event::Type::Move)
				{
					std::ostringstream oss;
					//oss << "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
					std::string msg = "Mouse Position: (" + std::to_string(e.GetPosX()) + "," + std::to_string(e.GetPosY()) + ")";
					std::wstring wMsg = std::wstring(msg.begin(), msg.end());
					//const wchar_t* wCStr = wMsg.c_str();
					wnd.SetTitle(wMsg);
				}
			}
			/******************** END MOUSE MESSAGE TEST ********************/

			/******************** KEYBOARD MESSAGE TEST ********************/
			//if (wnd.kbd.KeyIsPressed(VK_SPACE))
			//{
			//	MessageBox(nullptr, L"Something Happen!", L"Spacebar Key Was Pressed!", MB_OK | MB_ICONEXCLAMATION);
			//}
			/******************** END KEYBOARD MESSAGE TEST ********************/
		}

		// check if GetMessage call itself borked
		if (gResult == -1)
		{
			throw CHWND_LAST_EXCEPT();
		}
		// wParam here is the value passed to PostQuitMessage
		return msg.wParam;
	}
	catch(const ChiliException& e)
	{
		MessageBox(nullptr, (LPCWSTR)e.what(), (LPCWSTR)e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, (LPCWSTR)e.what(), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}