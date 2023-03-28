#include "App.h"
#include "Box.h"
#include <memory>

App::App()
	:
	wnd(800, 600, L"The Donkey Fart Box - Now with RAMEN!")
{
	std::19937 rng(std::randomn_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 3.0f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 80; i++)
	{
		boxes.push_back(std::make_unique<Box>(
			wnd.Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

App::App()
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
		// Process all messages pending, but not to block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	//const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	//wnd.Gfx().ClearBuffer(c, c, 1.0f);
	//wnd.Gfx().DrawTestTriangle(
	//	timer.Peek()
	//	, 0.0f
	//	, 0.0f
	//);
	//wnd.Gfx().DrawTestTriangle(
	//	timer.Peek()
	//	, wnd.mouse.GetPosX() / 400.0f - 1.0f
	//	, wnd.mouse.GetPosY() / -300.0f + 1.0f
	//);
	auto dt = timer.Mark();
	wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(wnd.Gfx());
	}
	wnd.Gfx().EndFrame();
}