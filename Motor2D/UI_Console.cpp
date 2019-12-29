#include "UI_Console.h"
#include "j1App.h"
#include "j1Render.h"
#include "Brofiler\Brofiler.h"

void Console::BlitElement()
{
	BROFILER_CATEGORY("Console", Profiler::Color::Wheat);

	if (texture != App->gui->GetAtlas())
		SDL_SetTextureAlphaMod(texture, App->gui->alpha_value);
	iPoint globalPos = calculateAbsolutePosition();
	App->render->Blit(texture, globalPos.x, globalPos.y, &section, false, App->gui->UI_scale);
}

void Console::appendChild(int x, int y, UI_element * child)
{
	child->localPosition = { x, y };
	child->setOriginalPos(x, y);
	child->parent = this;
	text = (Text*)child;
}

void Console::appendChildAtCenter(UI_element * child)
{
	iPoint child_pos(section.w / (2 / App->gui->UI_scale), section.h / (2 / App->gui->UI_scale));
	child_pos.x -= child->section.w / (2 / App->gui->UI_scale);
	child_pos.y -= child->section.h / (2 / App->gui->UI_scale);
	child->localPosition = { child_pos.x, child_pos.y };
	child->setOriginalPos(child_pos.x, child_pos.y);
	child->parent = this;
	text = (Text*)child;
}