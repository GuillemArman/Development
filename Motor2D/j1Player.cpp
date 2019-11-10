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
#include "j1Pathfinding.h"
#include <stdio.h>

j1Player::j1Player() : Entity("player")
{
	name.create("player");

	idle_right.Animation_XML("idle_right", "player");
	idle_left.Animation_XML("idle_left", "player");
	right.Animation_XML("right", "player");
	left.Animation_XML("left", "player");
	jumping_right.Animation_XML("jumping_right", "player");
	jumping_left.Animation_XML("jumping_left", "player");
	dying_right.Animation_XML("dying_right", "player");
	dying_left.Animation_XML("dying_left", "player");


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
	graphics = App->tex->Load("textures/Sprites/SantaSprites/Santafinal.png");

	if (collider == nullptr)
		collider = App->collision->AddCollider({ 0, 0, 110, 130 }, COLLIDER_PLAYER, this);

	collider_offset.x = 3;
	collider_offset.y = 2;

	collidingFloor = nullptr;
	colliding_bottom = false;
	colliding_left = false;
	colliding_right = false;

	animation = &idle_right;


	virtualPosition.x = position.x;
	virtualPosition.y = position.y;

	IsDead = false;
	GodMode = false;

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


	Entity_Update(dt);

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
		if (GodMode == false)
		{
			App->audio->PlayFx(jump_sound);
			v.y = jump_force;
			jumps += 1;

			if (jumps >= 2)
				jump = true;

			state = JUMPING;
		}

	}

	// ONLY DURING GODMODE 

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && GodMode == true)
	{
		v.y += 5;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP && GodMode == true)
	{
		v.y = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && GodMode == true)
	{
		v.y -= 5;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP && GodMode == true)
	{
		v.y = 0;
	}


	collider->SetPos(virtualPosition.x + collider_move.x, virtualPosition.y + collider_move.y);
	

	return true;
}

bool j1Player::PostUpdate(float dt) {

	if (!IsDead)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !colliding_right && v.x == 0)
		{
			v.x = speed;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !colliding_left && v.x == 0)
		{
			v.x = -speed;
		}
	}
	if (IsDead && SDL_GetTicks() > killed_finished + 1500)
	{
		killed_finished = 0;

		sound_one_time = false;
		if (App->scene->current_lvl->data->lvl == 1)
		{
			App->scene->LoadLvl(1);
		}
		if (App->scene->current_lvl->data->lvl == 2)
		{
			App->scene->LoadLvl(2);
		}
		IsDead = false;
		state = IDLE;
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

	
	int win_scale = App->win->GetScale();
	pos_relCam = App->player->position.x + App->render->camera.x / win_scale;
	

	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame(dt));

	

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
	
	if (c2->type == COLLIDER_ENEMY)
	{
		p2SString c2_name = c2->callback->name.GetString();
		if (c2_name == "walking_enemy")
		{
			//App->audio->PlayFx(App->player->dead_sound);
			v.x = 0;
			state = DEAD;
			IsDead = true;
			if (sound_one_time == false && killed_finished == 0)
			{
				killed_finished = SDL_GetTicks();
				App->audio->PlayFx(dead_sound, 0);
				sound_one_time = true;
			}
		}
	}
	Entity_OnCollision(c1, c2);
}

