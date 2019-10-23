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

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

	// Add all levels to the list
	level* lvl1 = new level("test.tmx");
	level* lvl2 = new level("platformer.tmx");

	levels.add(lvl1);
	levels.add(lvl2);

	current_lvl = levels.start;
}

// Destructor
j1Scene::~j1Scene()
{

}

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
	App->map->Load(levels.start->data->mapPath.GetString()); //test.tmx
	background = App->tex->Load("textures/BG_.png"); // Should add it throught tiled (ERASE ONCE DONE)
	
	
	//App->audio->PlayMusic("audio/music/music_sadpiano.ogg");

	pit_collider = App->collision->AddCollider({ 0, 0, 3000, 30 }, COLLIDER_PIT, this);
	



	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}
// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->render->camera.x > -1970)
	{
		//App->render->camera.x -= 1; // Coment this line for working
	}


	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		LoadLvl(0);
		

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
			App->SaveGame("save_game.xml");

	//camera
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		App->render->camera.y += 1;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 4;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 4;


	App->win->GetWindowSize(win_width, win_height);
	if (App->player->pos_relCam > (win_width / App->win->GetScale() / 10))
	{
		App->render->virtualCamPos -= App->player->speed * 2;
	}

	App->render->Blit(background, 0, 0);
	App->map->Draw();


		

	pit_collider->SetPos(0, 770);


	// TODO 7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2SString title("Platformer:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

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
		App->map->Load(current_lvl->data->mapPath.GetString());
		// Restart player data
		App->player->player_collider = nullptr; //Has to be null in order to be created
		App->player->Start();


	}
}