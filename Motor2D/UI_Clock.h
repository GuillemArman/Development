#ifndef __UI_CLOCK__
#define __UI_CLOCK__
#include "UI_element.h"
#include "UI_Text.h"
#include "SDL/include/SDL_pixels.h"
struct _TTF_Font;

class Clock : public UI_element
{
public:

	Clock()
	{}
	Clock(int x, int y, _TTF_Font* font, SDL_Color color, j1Module* callback) : UI_element(x, y, element_type::CLOCK, { 0, 0, 0, 0 }, callback, nullptr)
	{
		text = new Text("", x, y, font, color, nullptr);
		text->setOutlined(true);
	}
	~Clock()
	{
		delete text;
	}
	void BlitElement();
	

public:
	uint timer = 0;
	uint time_changed = 0;
	Text* text;
};

#endif