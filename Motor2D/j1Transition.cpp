#include "j1Transition.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Window.h"

Transition::Transition()
{
	pausable = false;
	alpha_value = 0;
}
Transition::~Transition()
{
}
bool Transition::Update(float dt)
{
	if (doingMenuTransition)
	{
		float Dalpha = 255 / (total_time / dt);
		if (menuState == GIN)
		{
			App->gui->alpha_value -= Dalpha;
			if (App->gui->alpha_value <= 0)
			{
				App->gui->alpha_value = 0;
				menuState = GOUT;
				App->uiScene->loadMenu(newMenuID);
			}
		}
		else if (menuState == GOUT)
		{
			App->gui->alpha_value += Dalpha;
			if (App->gui->alpha_value >= 255)
			{
				App->gui->alpha_value = 255;
				menuState = UNACTIVE;
				doingMenuTransition = false;
			}
		}
	}

	if (doingSceneTransition)
	{
		float Dalpha = 255 / (total_time / dt);
		if (sceneState == GOUT)
		{
			alpha_value -= Dalpha;
			if (alpha_value <= 0)
			{
				alpha_value = 0;
				sceneState = UNACTIVE;
				doingSceneTransition = false;
			}
		}
		else if (sceneState == GIN)
		{
			alpha_value += Dalpha;
			if (alpha_value >= 255)

			{
				alpha_value = 255;
				sceneState = GOUT;
				App->scene->newLvl = newLvl;
				App->scene->load_lvl = true;
			}

		}
	}

	return true;
}

bool Transition::PostUpdate(float dt)
{
	if (doingSceneTransition)
		App->render->DrawQuad({ 0, 0, App->win->screen_surface->w, App->win->screen_surface->h }, 0, 0, 0, alpha_value, true, false);
	return true;
}

void Transition::menuTransition(menu_id newMenuID, float time)
{

	this->newMenuID = newMenuID;
	timer.Start();
	total_time = time * 0.5f;
	doingMenuTransition = true;
	menuState = GIN;

}
void Transition::sceneTransition(int newLvl, float time)
{

	this->newLvl = newLvl;
	timer.Start();
	total_time = time * 0.5f;
	doingSceneTransition = true;
	sceneState = GIN;

}