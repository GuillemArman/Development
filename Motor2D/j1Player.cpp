#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Scene.h"
#include<stdio.h>

j1Player::j1Player() 
{
	name.create("player");


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

	right.speed = 0.02;

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

	jumping_right.speed = 0.02;

	/*idle_left.PushBack({ 7, 66, 369, 392 });

	jumping_right.PushBack({ 2643, 504, 409, 398 });
	jumping_left.PushBack({ 2643, 504, 409, 398 });

	right.PushBack({ 687, 34, 369, 392 });

	right.speed = 0.007f;

	left.PushBack({ -687, -34, 369, 392 });

	left.speed = 0.007f;*/

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

	graphics = App->tex->Load("textures/Sprites/SantaSprites/_Santa1.png");//placeholder for now

	if (player_collider == nullptr)
		player_collider = App->collision->AddCollider({ 0, 0, 75, 110	 }, COLLIDER_PLAYER, this);

	animation = &idle_right;

	position.x = 50;
	position.y = 270;

	virtualPosition.x = position.x;
	virtualPosition.y = position.y;

	isDead = false;

	return true;


}


bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(graphics);
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
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

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (state != JUMPING)
		{
			v.y = jump_force;
			state = JUMPING;
		}
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

	position.x = virtualPosition.x;
	position.y = virtualPosition.y;

	int win_scale = App->win->GetScale();
	pos_relCam = App->player->position.x + App->render->camera.x / win_scale;


	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame());



	return true;
}

bool j1Player::Load(pugi::xml_node& data)
{
	//loading player pos from xml
	virtualPosition.x = data.attribute("position_x").as_int();
	virtualPosition.y = data.attribute("position_y").as_int();
	return true;

}

bool j1Player::Save(pugi::xml_node& data) const
{
	//saving pos into xml

	data.append_attribute("position_x") = position.x;
	data.append_attribute("position_y") = position.y;
	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	Entity_OnCollision(c1, c2);
}

