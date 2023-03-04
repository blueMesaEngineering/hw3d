#include "App.h"

App::App()
	:
	wnd(800, 600, L"The Donkey Fart Box - Now with RAMEN!")
{}

int App::DoStuff()
{
	//MSG msg;
	//bool gResult;
	//while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	//{
	//	// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);

	//	DoFrame();
	//}

	//// check if GetMessage call itself borked
	//if (gResult == -1)
	//{
	//	throw CHWND_LAST_EXCEPT();
	//}

	//// wParam here is the value passed to PostQuitMessage
	//return msg.wParam;

	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	wnd.Gfx().EndFrame();

	//const float t = timer.Peek(); // std::setprecision(1) + std::fixed + 
	//std::string msg = "Time elapsed: " + std::to_string((int)t) + "s";
	//std::wstring wMsg = std::wstring(msg.begin(), msg.end());
	//wnd.SetTitle(wMsg);

		// test code
		//static int i = 0;
		//while (!wnd.mouse.IsEmpty())
		//{
		//	const auto e = wnd.mouse.Read();
		//	switch (e.GetType())
		//	{
		//		/******************** MOUSE WHEEL POSITION TEST ********************/
		//	case Mouse::Event::Type::WheelUp:
		//		i++;
		//		{
		//			std::string msg = "Up: " + std::to_string(i);
		//			std::wstring wMsg = std::wstring(msg.begin(), msg.end());
		//			wnd.SetTitle(wMsg);
		//		}
		//		break;
		//	case Mouse::Event::Type::WheelDown:
		//		i--;
		//		{
		//			std::string msg = "Down: " + std::to_string(i);
		//			std::wstring wMsg = std::wstring(msg.begin(), msg.end());
		//			wnd.SetTitle(wMsg);
		//		}
		//		break;

				/******************** END MOUSE WHEEL POSITION TEST ********************/

				/******************** MOUSE POSITION TEST ********************/
				//case Mouse::Event::Type::Leave:
				//	wnd.SetTitle(L"GONE!");
				//	break;

				//case Mouse::Event::Type::Move:
				//	{
				//		std::string msg = "Mouse Position: (" + std::to_string(e.GetPosX()) + "," + std::to_string(e.GetPosY()) + ")";
				//		std::wstring wMsg = std::wstring(msg.begin(), msg.end());
				//		wnd.SetTitle(wMsg);
				//	}
				//	break;
				/******************** END MOUSE POSITION TEST ********************/
		//	}
		//}

		/******************** MOUSE MESSAGE TEST ********************/
		//while (!wnd.mouse.IsEmpty())
		//{
		//	const auto e = wnd.mouse.Read();
		//	if (e.GetType() == Mouse::Event::Type::Move)
		//	{
		//		std::ostringstream oss;
		//		//oss << "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
		//		std::string msg = "Mouse Position: (" + std::to_string(e.GetPosX()) + "," + std::to_string(e.GetPosY()) + ")";
		//		std::wstring wMsg = std::wstring(msg.begin(), msg.end());
		//		//const wchar_t* wCStr = wMsg.c_str();
		//		wnd.SetTitle(wMsg);
		//	}
		//}
		/******************** END MOUSE MESSAGE TEST ********************/

		/******************** KEYBOARD MESSAGE TEST ********************/
		//if (wnd.kbd.KeyIsPressed(VK_SPACE))
		//{
		//	MessageBox(nullptr, L"Something Happen!", L"Spacebar Key Was Pressed!", MB_OK | MB_ICONEXCLAMATION);
		//}
		/******************** END KEYBOARD MESSAGE TEST ********************/
}