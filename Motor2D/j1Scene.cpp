#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1PathFinding.h"
#include "j1EntityManager.h"
#include "Brofiler\Brofiler.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "UI_element.h"
#include "UI_Clock.h"
#include "j1Transition.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
	
	// Add all levels to the list
	level* main_lvl = new level(1, "main_menu.tmx", START_MENU, "audio/music/menu_music.ogg", true);
	level* lvl1 = new level(2, "platformer.tmx", INGAME_MENU, "audio/music/ambient_music.wav");
	level* lvl2 = new level(3, "platformer2.tmx", INGAME_MENU, "audio/music/ambient_music.wav");
	level* endGame_lvl = new level(4, "main_menu.tmx", FINAL_MENU, "audio/music/menu_music.ogg", true);

	levels.add(main_lvl);
	levels.add(lvl1);
	levels.add(lvl2);
	levels.add(endGame_lvl);

	current_lvl = levels.start;

}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{

	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(levels.start->data->mapPath.GetString(), current_lvl->data->length, current_lvl->data->end_rect); //hello2.tmx
	//Background music
	App->audio->PlayMusic(current_lvl->data->music.GetString(), 1.0f);
	//App->audio->PlayMusic("audio/music/ambient_music.wav");
	complete_level_fx = App->audio->LoadFx("audio/fx/level_complete.wav");
	win_fx = App->audio->LoadFx("audio/fx/win.wav");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene PreUpdate", Profiler::Color::White);
	if (current_lvl->data->default_paused)
		App->render->virtualCamPos = 0;
	if (load_lvl)
	{
		LoadLvl(newLvl);
		load_lvl = false;
		newLvl = 0;
	}

	return true;
}


// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::White);
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		load_lvl = true;
		newLvl = 2;
	}

	
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		load_lvl = true;
		newLvl = 2;
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		if (current_lvl->data->lvl == 3)
		{
			load_lvl = true;
			newLvl = 3;
		}
		else if (current_lvl->data->lvl == 2)
		{
			load_lvl = true;
			newLvl = 2;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && !App->entityManager->getPlayer()->dead)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && !App->entityManager->getPlayer()->dead)
		App->LoadGame();




	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		App->cap_frames = !App->cap_frames;
	}

	// camera move -----------------------
	uint win_width, win_height;
	App->win->GetWindowSize(win_width, win_height);
	
	if ((App->entityManager->getPlayer()->pos_relCam > (win_width / App->win->GetScale() / 2)) && (App->render->virtualCamPos > max_camera_pos))
	{
		App->render->virtualCamPos -= App->entityManager->getPlayer()->speed * 2 * dt; //*dt
	}




	// Parallax
	p2List_item<ImageLayer*>* image = nullptr; // Parallax when player moves
	for (image = App->map->data.image_layers.start; image; image = image->next)
	{
		if (image->data->speed > 0)
		{
			if (image->data->constant_movement)
			{
				image->data->position.x -= image->data->speed * dt; //*dt
			}
			else if (App->entityManager->getPlayer()->v.x > 0 && (App->entityManager->getPlayer()->pos_relCam > (win_width / App->win->GetScale() / 2)) && (App->render->virtualCamPos > max_camera_pos))
			{
				image->data->position.x -= image->data->speed * dt; //*dt
			}
		}
	}

	

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Scene PostUpdate", Profiler::Color::White);
	bool ret = true;

	App->map->Draw();

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)	
	{
		if (current_lvl->data->default_paused)
			ret = false;
		else
		{
			if (!App->paused)
			{
				App->paused = true;
				App->transition->menuTransition(PAUSE_MENU, 0.3);
			}
			else
			{
				App->paused = false;
				App->transition->menuTransition(INGAME_MENU, 0.3);
			}
		}
	}


	return ret;
}



// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::LoadLvl(int num)
{	
	p2SString previous_music = current_lvl->data->music;

	if (num == 0)
	{
		current_lvl = current_lvl->next;
		if (current_lvl == nullptr)
		{
			current_lvl = levels.start;
		}
	}
	else
	{
		p2List_item<level*>* lvl = levels.start;
		for (int i = 1; i < num; i++)
		{
			lvl = lvl->next;
			if (lvl == nullptr)
			{
				LOG("There is no level %d to load", num);
				break;
			}
		}
		current_lvl = lvl;
	}

	if (current_lvl != nullptr)
	{
		App->entityManager->cleanCollectibles();
		if (respawn_enemies && num == 2)
		{
			j1Player* player = (j1Player*)App->entityManager->getPlayer();
			player->coins[0] = player->coins[1] = player->coins[2] = false;
			player->lives = 3;
			player->score = 0;
			App->uiScene->clock->counter.Play();
			App->uiScene->clock->counter.Start();
		}
		App->map->Load(current_lvl->data->mapPath.GetString(), current_lvl->data->length, current_lvl->data->end_rect, !respawn_enemies);
		App->uiScene->loadMenu(current_lvl->data->default_menu);
		respawn_enemies = true;
		if (current_lvl->data->default_paused)
			App->paused = true;

		if (previous_music != current_lvl->data->music)
			App->audio->PlayMusic(current_lvl->data->music.GetString(), 1.0f);
		uint win_width, win_height;
		App->win->GetWindowSize(win_width, win_height);
		max_camera_pos = current_lvl->data->length + (win_width);
		max_camera_pos *= -1;

	}
}