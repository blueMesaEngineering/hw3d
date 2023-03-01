#pragma once
#include "Window.h"

class App
{
public:
	App();
	// master frame / message loop
	int DoStuff();
private:
	void DoFrame();
private:
	Window wnd;
};