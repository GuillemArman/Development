#include "UI_Clock.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"

void Clock::BlitElement()
{
	time_changed = App->gui->clock.ReadSec();
	p2SString("%d", timer);

}