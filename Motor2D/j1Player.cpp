#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"
#include<stdio.h>

j1Player::j1Player() 
{
	name.create("player");


	idle_right.PushBack({ 1128, 1313, 344, 546 });
	idle_right.PushBack({ 2063, 1314, 344, 545 });
	idle_right.PushBack({ 2998, 1315, 344, 544 });
	idle_right.PushBack({ 3934, 1316, 340, 543 });
	idle_right.PushBack({ 4869, 1318, 340, 541 });
	idle_right.PushBack({ 5804, 1319, 339, 540 });
	idle_right.PushBack({ 6739, 1320, 338, 538 });
	idle_right.PushBack({ 202, 1961, 338, 539 });
	idle_right.PushBack({ 1138, 1962, 335, 538 });
	idle_right.PushBack({ 2073, 1962, 335, 538 });
	idle_right.PushBack({ 3008, 1964, 334, 536 });
	idle_right.PushBack({ 3940, 1961, 335, 539 });
	idle_right.PushBack({ 4871, 1959, 338, 541 });
	idle_right.PushBack({ 5803, 1957, 340, 542 });
	idle_right.PushBack({ 6734, 1955, 343, 545 });
	idle_right.PushBack({ 195, 2596, 343, 544 });
	
	idle_right.speed = 0.07f;

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


	return true;
}

bool j1Player::Start() {

	graphics = App->tex->Load("textures/Sprites/SantaSprites/_Santa.png");//placeholder for now

	if (player_collider == nullptr)
		player_collider = App->collision->AddCollider({ 0, 0, 50, 50 }, COLLIDER_PLAYER, this);

	animation = &idle_right;

	position.x = 50;
	position.y = -50;

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

bool j1Player::Update() {

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

	//player_collider->SetPos(virtualPosition.x + collider_move.x, virtualPosition.y + collider_move.y);
	//App->player->colliding_left = false;
	//App->player->colliding_right = false;

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


	//position.x = virtualPosition.x;
	//position.y = virtualPosition.y;

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

//iPoint j1Player::GetPosition()
//{
//	return position;
//}