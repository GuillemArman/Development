#include "j1App.h"
#include "j1Transition.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

j1Transition::j1Transition()
{
	pugi::xml_document file;
	pugi::xml_parse_result result = file.load_file("config.xml");

	int width = file.child("config").child("window").child("resolution").attribute("width").as_int();
	int height = file.child("config").child("window").child("resolution").attribute("height").as_int();
	int scale = file.child("config").child("window").child("resolution").attribute("scale").as_int();

	screen = { 0, 0, width * scale, height * scale };
	name.create("fade");
}

j1Transition::~j1Transition()
{}


bool j1Transition::Start()
{
	
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}


bool j1Transition::Update(float dt)
{
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;
	}

	
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}


bool j1Transition::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;
	ModuleOff = module_off;
	ModuleOn = module_on;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 250.0f);
		ret = true;
	}
	return ret;
}

bool j1Transition::IsFading() const
{
	return current_step == fade_step::fade_to_black;
}