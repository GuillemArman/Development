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
}

// Destructor
j1Scene::~j1Scene()
{
	delete[] floor_colliders;
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
	background = App->tex->Load("textures/BG_.png");
	App->player->santa = App->tex->Load("textures/Sprites/Santasprites/Idle (1).png");//placeholder for now
	App->map->Load("test.tmx");
	//App->audio->PlayMusic("audio/music/music_sadpiano.ogg");
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
	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 1;

	//player
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		App->player->position.y -= 1;

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		App->player->position.y += 1;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		App->player->position.x += 1;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		App->player->position.x -= 1;

	App->win->GetWindowSize(win_width, win_height);
	if (App->player->pos_relCam > (win_width / App->win->GetScale() / 2))
	{
		App->render->virtualCamPos -= App->player->speed * 2;
	}

	App->render->Blit(background, 0, 0);
		App->map->Draw();


	App->render->Blit(App->player->santa, App->player->position.x, App->player->position.y + 100);//HARDCODED INITIAL Y
	App->player->player_collider->SetPos(App->player->position.x, App->player->position.y + 300);//collider test
	

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

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

