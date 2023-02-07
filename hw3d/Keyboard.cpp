#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (keybuffer.size() > 0u)
	{
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return keybuffer.empty();
}

char Keyboard::ReadChar() noexcept
{
	if (charbuffer.size() > 0u)
	{
		unsigned char charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;
	}
	else
	{
		return 0;
	}
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return charbuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	keybuffer = std::queue<Event>();
}

void Keyboard::FlushChar() noexcept
{
	charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

//bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
//{
//	return keystates[keycode];
//}
//
//Keyboard::Event Keyboard::ReadKey() noexcept
//{
//	if (keybuffer.size() > 0u)
//	{
//		Keyboard::Event e - keybuffer.front();
//		keybuffer.pop();
//		return e;
//	}
//	else
//	{
//		return Keyboard::Event();
//	}
//}
//
//bool Keyboard::KeyIsEmpty() const noexcept
//{
//	return keybuffer.empty();
//}