#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"
#include<stdio.h>

j1Player::j1Player() :Entity()
{
	name.create("player1");
}

j1Player::~j1Player()
{

}

bool j1Player::Awake(pugi::xml_node& config)
{
	speed = config.attribute("speed").as_float();


	return true;
}

bool j1Player::Start() {

	virtualPosition.x = position.x;
	virtualPosition.y = position.y;

	return true;

}

bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update() {

	// Defining states

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
	return true;
}

bool j1Player::PostUpdate() {

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && v.x == 0)
	{
		v.x = speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && v.x == 0)
	{
		v.x = -speed;
	}



	int win_scale = App->win->GetScale();
	pos_relCam = App->player->position.x + App->render->camera.x / win_scale;

	return true;
}

bool j1Player::Load(pugi::xml_node& data)
{
	//loading player pos from xml
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	return true;

}

bool j1Player::Save(pugi::xml_node& data) const
{
	//saving pos into xml

	pugi::xml_node player = data.append_child("position");
	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;
	return true;
}

//iPoint j1Player::GetPosition()
//{
//	return position;
//}