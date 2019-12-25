#ifndef __UI_CLOCK__
#define __UI_CLOCK__
#include "UI_element.h"

class Clock : public UI_element
{
public:

	Clock()
	{}
	Clock(int x, int y)
	{}
	~Clock()
	{}
	void BlitElement();
	void Time();

public:
	uint timer = 0;
	uint time_changed = 0;
};

#endif