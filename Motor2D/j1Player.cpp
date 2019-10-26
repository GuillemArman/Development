#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Map.h"
#include <stdio.h>

j1Player::j1Player() 
{
	name.create("player");

	idle_right.Animation_XML("idle_right");
	idle_left.Animation_XML("idle_left");
	right.Animation_XML("right");
	left.Animation_XML("left");
	jumping_right.Animation_XML("jumping_right");
	jumping_left.Animation_XML("jumping_left");
	dying_right.Animation_XML("dying_right");
	dying_left.Animation_XML("dying_left");

	idle_right.PushBack({ 304, 308, 73, 113 });
	idle_right.PushBack({ 494, 308, 73, 113 });
	idle_right.PushBack({ 684, 308, 73, 113 });
	idle_right.PushBack({ 874, 308, 72, 113 });
	idle_right.PushBack({ 1064, 308, 72, 113 });
	idle_right.PushBack({ 1255, 309, 71, 112 });
	idle_right.PushBack({ 1445, 309, 71, 112 });
	idle_right.PushBack({ 116, 439, 71, 113 });
	idle_right.PushBack({ 306, 439, 71, 113 });
	idle_right.PushBack({ 496, 439, 71, 113 });
	idle_right.PushBack({ 686, 439, 71, 113 });
	idle_right.PushBack({ 875, 439, 71, 113 });
	idle_right.PushBack({ 1065, 438, 71, 114 });
	idle_right.PushBack({ 1254, 438, 72, 114 });
	idle_right.PushBack({ 1444, 438, 72, 114 });
	
	
	idle_right.speed = 0.05;

	right.PushBack({ 309, 829, 71, 114 });
	right.PushBack({ 497, 828, 73, 114 });
	right.PushBack({ 687, 828, 78, 114 });
	right.PushBack({ 877, 828, 73, 115 });
	right.PushBack({ 1068, 828, 70, 115 });
	right.PushBack({ 1259, 829, 69, 114 });
	right.PushBack({ 1444, 828, 84, 117 });
	right.PushBack({ 108, 958, 94, 116 });
	right.PushBack({ 306, 958, 82, 117 });
	right.PushBack({ 499, 959, 71, 115 });
	right.PushBack({ 689, 959, 70, 115 });

	right.speed = 0.25;

	jumping_right.PushBack({313, 572, 72, 110});
	jumping_right.PushBack({ 506, 575, 72, 107 });
	jumping_right.PushBack({ 697, 575, 71, 107 });
	jumping_right.PushBack({ 882, 572, 72, 115 });
	jumping_right.PushBack({ 1060, 569, 89, 115 });
	jumping_right.PushBack({ 1241, 567, 98, 111 });
	jumping_right.PushBack({ 1429, 567, 100, 111 });
	jumping_right.PushBack({ 100, 697, 100, 112 });
	jumping_right.PushBack({ 288, 697, 102, 113 });
	jumping_right.PushBack({ 478, 697, 101, 113 });
	jumping_right.PushBack({ 667, 697, 102, 114 });
	jumping_right.PushBack({ 857, 697, 102, 114 });
	jumping_right.PushBack({ 1060, 699, 88, 118 });
	jumping_right.PushBack({ 1259, 701, 74, 116 });
	jumping_right.PushBack({ 1452, 702, 71, 113 });
	jumping_right.PushBack({ 123, 833, 72, 110 });

	jumping_right.speed = 0.2;

	/*idle_left.PushBack({ 7, 66, 369, 392 });

	jumping_right.PushBack({ 2643, 504, 409, 398 });
	jumping_left.PushBack({ 2643, 504, 409, 398 });

	right.PushBack({ 687, 34, 369, 392 });

	right.speed = 0.007f;

	left.PushBack({ -687, -34, 369, 392 });

	left.speed = 0.007f;*/

	position.x = 80;
	position.y = 240;
}

j1Player::~j1Player()
{

}

bool j1Player::Awake(pugi::xml_node& config)
{
	speed = config.attribute("speed").as_float();

	jump_force = config.attribute("jump_force").as_float();

	return true;
}

bool j1Player::Start() {

	if (graphics == nullptr)
	graphics = App->tex->Load("textures/Sprites/SantaSprites/_Santa1.png");//placeholder for now

	if (player_collider == nullptr)
		player_collider = App->collision->AddCollider({ 0, 0, 75, 110	 }, COLLIDER_PLAYER, this);


	collidingFloor = nullptr;
	colliding_bottom = false;
	colliding_left = false;
	colliding_right = false;

	animation = &idle_right;


	virtualPosition.x = position.x;
	virtualPosition.y = position.y;

	isDead = false;

	//LOADING PLAYER FX
	jump_sound = App->audio->LoadFx("audio/fx/jump_fx.ogg");
	walking_sound = App->audio->LoadFx("audio/fx/walk_fx.ogg");
	dead_sound = App->audio->LoadFx("audio/fx/dead_fx.wav");

	return true;


}


bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(graphics);
	if (player_collider != nullptr)
	{
		player_collider->to_delete = true;
		player_collider = nullptr;
	}

	return true;
}

bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {

	// Defining states


	Entity_Update();

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		App->audio->PlayFx(walking_sound, 1);
		v.x = -speed;
		if (state != JUMPING && state != DEAD)
		{
			state = LEFT;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		if (v.x < 0 && state != JUMPING)
		{
			v.x = 0;
			state = IDLE;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		App->audio->PlayFx(walking_sound, 1);
		v.x = speed;
		if (state != JUMPING && state != DEAD)
		{
			state = RIGHT;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		if (v.x > 0 && state != JUMPING)
		{
			v.x = 0;
			state = IDLE;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jump == false)
	{

		App->audio->PlayFx(jump_sound);

		v.y = jump_force;
		jumps += 1;

		if (jumps >= 2)
			jump = true;

		state = JUMPING;

	}

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		state = DEAD;
		isDead = true;


	}

	player_collider->SetPos(virtualPosition.x + collider_move.x, virtualPosition.y + collider_move.y);
	App->player->colliding_left = false;
	App->player->colliding_right = false;

	return true;
}

bool j1Player::PostUpdate() {

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !colliding_right && v.x == 0)
	{
		v.x = speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !colliding_left && v.x == 0)
	{
		v.x = -speed;
	}

	p2List_item<ImageLayer*>* image = nullptr;
	for (image = App->map->data.image_layers.start; image; image = image->next)
	{
		if (image->data->speed > 0)
		{
			if (image->data->constant_movement)
			{
				image->data->position.x -= image->data->speed;
			}
			else if (v.x > 0)
			{
				image->data->position.x -= image->data->speed;
			}
			else if (v.x < 0)
			{
				image->data->position.x += image->data->speed;
			}

		}
	}

	position.x = virtualPosition.x;
	position.y = virtualPosition.y;

	int win_scale = App->win->GetScale();
	pos_relCam = App->player->position.x + App->render->camera.x / win_scale;

	/*if (position.y > App->win->screen_surface->h / win_scale + 50 && !won)
	{
	App->scene->LoadLvl(App->scene->current_lvl->data->lvl);
	}*/


	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame());

	LOG("%d", position.x);
	LOG("%f", App->render->virtualCamPos);

	return true;
}

bool j1Player::Load(pugi::xml_node& data)
{
	//Load player from xml
	App->scene->LoadLvl(data.attribute("level").as_int());
	virtualPosition.x = data.attribute("position_x").as_int();
	virtualPosition.y = data.attribute("position_y").as_int() - diff; // diff is a value that makes sures that the player doesn't restart inside the collider
	return true;

}

bool j1Player::Save(pugi::xml_node& data) const
{
	//saving pos into xml

	data.append_attribute("position_x") = position.x;
	data.append_attribute("position_y") = position.y;
	data.append_attribute("level") = App->scene->current_lvl->data->lvl;
	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	Entity_OnCollision(c1, c2);
}

