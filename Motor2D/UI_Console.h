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

	Console(int x, int y, j1Module* callback) : UI_element(x, y, element_type::CONSOLE, { 0, 0, 0, 0 }, callback)
	{}

	~Console()
	{}

	void BlitElement();

	// By now we will only show LOG's
	void appendChild(int x, int y, UI_element* child);
	void appendChildAtCenter(UI_element* child); 

public:

	// 2 windows in console. 1 with inputbox text, reads the string and execute the command. The other one shows only text (LOGS)
	Window* window;
	InputBox* inputbox;
	Text* text;
	Image* image;

};

#endif