#ifndef _UI_CONSOLE_
#define _UI_CONSOLE_

#include "UI_element.h"
#include "UI_Window.h"
#include "UI_InputBox.h"
#include "UI_Text.h"


class Console : public UI_element
{
public:

	Console()
	{}

	Console(int x, int y, SDL_Color color, j1Module* callback) : UI_element(x, y, element_type::CONSOLE, { 0, 0, 0, 0 }, callback, nullptr)
	{
	}

	~Console()
	{}

	void BlitElement();


public:

	Window* window;
	InputBox* inputbox;
	Text* text;
	Image* image;

};

#endif