#include "UI_Clock.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"

void Clock::BlitElement()
{
	if (timer < time_changed)
	{
		timer++;
		p2SString secs = p2SString("%d", timer);
		text->setText(secs);
	}

	time_changed = App->gui->clock.ReadSec();
	
	text->BlitElement();
}