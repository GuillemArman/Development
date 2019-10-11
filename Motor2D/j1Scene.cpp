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
	
	App->map->Load("test.tmx");
	//App->audio->PlayMusic("audio/music/music_sadpiano.ogg");

	pit_collider = App->collision->AddCollider({ 0, 0, 3000, 30 }, COLLIDER_PIT, this);
	App->player->touchingFloor = false;
	gravity_speed = 0.002f;
	jump_speed = 0.01f;


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


	/*if (App->player->touchingFloor == false)
	{
	App->player->position.y += gravity_speed;
	}
	else if (App->player->touchingFloor == true)
	{
	App->player->position.y = 0.0f;
	}*/

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
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

	

	



	App->render->Blit(background, 0, 0);
	App->map->Draw();


	App->player->player_collider->SetPos(App->player->position.x, App->player->position.y); //need to resize	

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
