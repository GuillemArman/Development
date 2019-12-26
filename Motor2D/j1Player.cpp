#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1entityManager.h"
#include "j1Transition.h"
#include "j1PathFinding.h"
#include "Brofiler\Brofiler.h"

#include<stdio.h>


j1Player::j1Player() : Entity("player")
{
	name.create("player");

	// Animations
	jump = LoadAnimation("animations/player.tmx", "santa");
	jump_offset.x = -16;
	jump_offset.y = 17;

	godMode = LoadAnimation("animations/player.tmx", "santagod");
	godmode_offset.x = -7;
	godmode_offset.y = -12;





	type = PLAYER;
}

j1Player::~j1Player()
{
	App->tex->UnLoad(graphics);
	graphics = nullptr;
	App->tex->UnLoad(graphics_god);
	graphics_god = nullptr;
}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");



	if (graphics == nullptr)
		graphics = App->tex->Load("textures/character/Santafinal.png");

	if (graphics_god == nullptr)
		graphics_god = App->tex->Load("textures/character/Santafinal.png");

	if (collider == nullptr)
		collider = App->collision->AddCollider({ 0, 0, 40, 70 }, COLLIDER_PLAYER, this);

	else
		collider->SetPos(0, 0);

	collider_offset.x = 3;
	collider_offset.y = 2;

	collidingFloor = nullptr;
	colliding_bottom = false;
	colliding_left = false;
	colliding_right = false;

	going_right = false;
	going_left = false;
	going_down = false;
	jumping = false;

	dead = false;
	sound_one_time = false;

	old_savedCol = nullptr;
	App->SaveGame(true);

	v.x = 0;
	v.y = 0;

	if (lives <= 0)
	{
		score = 0;
		lives = 3;
		coins[0] = coins[1] = coins[2] = false;
	}

	animation = idle_right;

	virtualPosition.x = position.x;
	virtualPosition.y = position.y;

	if (step_fx == 0)
		step_fx = App->audio->LoadFx("audio/fx/step.wav");
	if (jump_fx == 0)
		jump_fx = App->audio->LoadFx("audio/fx/jump.wav");
	if (double_jump_fx == 0)
		double_jump_fx = App->audio->LoadFx("audio/fx/double_jump.wav");
	if (landing_fx == 0)
		landing_fx = App->audio->LoadFx("audio/fx/landing.wav");
	if (die_fx == 0)
		die_fx = App->audio->LoadFx("audio/fx/die.wav");
	if (SSJ_transformation == 0)
		SSJ_transformation = App->audio->LoadFx("audio/fx/SSJ_transformation.wav");
	if (SSJ_off == 0)
		SSJ_off = App->audio->LoadFx("audio/fx/SSJ_off.wav");
	if (killed_fx == 0)
		killed_fx = App->audio->LoadFx("audio/fx/killed_by_enemy.wav");

	return true;
}



// Update: draw background
bool j1Player::Update(float dt)
{
	BROFILER_CATEGORY("Player Update", Profiler::Color::Red);

	LOG("Lives: %d", lives);
 
	if (!dead)
	{
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			if (App->entityManager->player_god_mode)
			{

				App->entityManager->player_god_mode = false;
				App->audio->PlayFx(SSJ_off, 0);
			}
			else
			{

				App->entityManager->player_god_mode = true;
				App->audio->PlayFx(SSJ_transformation, 0);

			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			v.x = -speed * 1.2;
			going_left = true;
			if (state != JUMPING && state != DEAD)
			{
				state = LEFT;
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			going_left = false;
			if (state == LEFT)
			{
				v.x = 0;
				state = IDLE;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			v.x = speed * 1.2;
			going_right = true;
			if (state != JUMPING && state != DEAD)
			{
				state = RIGHT;
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			going_right = false;
			if (state == RIGHT)
			{
				v.x = 0;
				state = IDLE;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			jumping = true;
			if (!double_jump)
			{
				if (state == JUMPING || state == FALLING)
				{
					double_jump = true;
					jump_pos.x = position.x + jump_offset.x;
					jump_pos.y = position.y + jump_offset.y;
					v.y = (jump_force * 2.5 / 3);
					if (state == FALLING)
					{
						state = JUMPING;
					}
					App->audio->PlayFx(double_jump_fx, 0);
				}
				else
				{
					v.y = jump_force;
					state = JUMPING;
					App->audio->PlayFx(jump_fx, 0);
				}
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		{
			// If player release space in mid jump, the character won't reach max height
			if (!double_jump && v.y > (jump_force * 2 / 3) / 2)
			{
				v.y = (jump_force * 2 / 3) / 2;
			}
		}
	}
	if (v.x != 0 && colliding_bottom && SDL_GetTicks() > step_time)
	{
		App->audio->PlayFx(step_fx, 0);
		step_time = SDL_GetTicks() + (1 / right->speed) + 450;
	}



	return true;
}

bool j1Player::PostUpdate(float dt)
{
	if (!dead)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !colliding_right && v.x == 0)
		{
			v.x = speed;
			state = RIGHT;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !colliding_left && v.x == 0)
		{
			v.x = -speed;
			state = LEFT;
		}
	}

	if (App->paused)
	{
		v.x = 0;
		state = IDLE;
	}

	// Win condition
	if ((((collider->rect.x + collider->rect.w) > App->scene->current_lvl->data->end_rect.x) && (position.y + collider->rect.h) < (App->scene->current_lvl->data->end_rect.y + App->scene->current_lvl->data->end_rect.h)) && !won) {
		
			if (end_reached == 0)
			{
				won = true;
				App->uiScene->pauseClock();
				end_reached = SDL_GetTicks();
				if (App->scene->current_lvl == App->scene->levels.end - 1)
				{
					App->audio->PlayFx(App->scene->win_fx, 0);
					App->scene->load_lvl = 4;
				}
				else
				{
					App->audio->PlayFx(App->scene->complete_level_fx, 0);
				}
			}
		}
	if (won && (/*(App->scene->current_lvl == App->scene->levels.end && SDL_GetTicks() > end_reached + 5000) || */(App->scene->current_lvl != App->scene->levels.end && SDL_GetTicks() > end_reached + 500)))
	{
			end_reached = 0;
			won = false;
			dead = false;
			App->scene->load_lvl = true;
	}

		// Lose condition
		//By enemyy
		if (dead && SDL_GetTicks() > killed_finished + 1500)
		{
			App->scene->load_lvl = true;
			if (lives > 0)
			{
				App->scene->newLvl = App->scene->current_lvl->data->lvl;
				App->scene->respawn_enemies = false;
			}
			else
				App->scene->newLvl = 1;

			killed_finished = 0;

		}
		//By falling
		int win_scale = App->win->GetScale();
		if (position.y > App->win->screen_surface->h / win_scale + 50 && !won && !dead)
		{
			if (App->entityManager->player_god_mode)
			{
				App->LoadGame(true);
			}
			else
			{
				lives--;
				App->scene->load_lvl = true;
				if (lives > 0)
				{
					App->scene->newLvl = App->scene->current_lvl->data->lvl;
					App->scene->respawn_enemies = false;
					App->audio->PlayFx(killed_fx, 0);
				}
				else
				{
					App->scene->newLvl = 1;
					App->audio->PlayFx(die_fx, 0);
				}
				App->scene->load_lvl = true;
				App->scene->newLvl = App->scene->current_lvl->data->lvl;
			}
		}

		//When f10 is clicked he converts into god mode
		if (App->entityManager->player_god_mode)
		{
			App->render->Blit(graphics_god, position.x + godmode_offset.x, position.y + godmode_offset.y, &godMode->GetCurrentFrame(dt));
			App->render->Blit(graphics_god, position.x, position.y, &animation->GetCurrentFrame(dt));
			App->render->Blit(graphics_god, position.x + godmode_offset.x, position.y + godmode_offset.y, &godMode->GetCurrentFrame(dt));

			//GODMODE MOVEMENT

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && App->entityManager->player_god_mode)
			{
				v.y += 150;
			}

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP && App->entityManager->player_god_mode)
			{
				v.y = 0;
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && App->entityManager->player_god_mode)
			{
				v.y -= 150;
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP && App->entityManager->player_god_mode)
			{
				v.y = 0;
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && App->entityManager->player_god_mode)
			{
				v.x += 5;
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP && App->entityManager->player_god_mode)
			{
				v.x = 0;
			}

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && App->entityManager->player_god_mode)
			{
				v.x -= 5;
			}

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP && App->entityManager->player_god_mode)
			{
				v.x = 0;
			}

		}
		else if (App->entityManager->player_god_mode == false)
		{
			App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame(dt));
		}

		if (double_jump)
		{
			App->render->Blit(graphics, jump_pos.x, jump_pos.y, &jump->GetCurrentFrame(dt));
		}

		return true;
		
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_FLOOR || c2->type == COLLIDER_JUMPABLE)
	{
		if (Collision_from_bottom(c1, c2))
		{
			
			double_jump = false;
			jump->Reset();
		}
	}

	//, if the player hits the Enemy from the top the  enemy must die
	if (!dead && c2->type == COLLIDER_ENEMY)
	{
		p2SString c2_name = c2->callback->name.GetString();
		if ((c2_name == "flying" || c2_name == "walking") && Collision_from_bottom(c1, c2, 3) && v.y < 0)
		{
			score += 200;
			v.y = (jump_force * 2 / 3);
			c2->entity->dead = true;
			c2->to_delete = true;
		}
		else if (!App->entityManager->player_god_mode && (c2_name == "walking" || c2_name == "flying") && (!c2->entity->dead && !Collision_from_bottom(c1, c2, 3)))
		{
			v.x = 0;
			dead = true;
			if (!sound_one_time && killed_finished == 0)
			{
				lives--;
				killed_finished = SDL_GetTicks();

				sound_one_time = true;
				if (lives > 0)
					App->audio->PlayFx(killed_fx, 0);
				else
					App->audio->PlayFx(die_fx, 0);
			}
		}
		
	}

	Entity_OnCollision(c1, c2);
}

bool j1Player::Load(pugi::xml_node& data)
{
	App->scene->respawn_enemies = false;
	App->scene->LoadLvl(data.attribute("level").as_int());
	lives = data.attribute("lives").as_uint();
	virtualPosition.x = data.attribute("position_x").as_int();
	virtualPosition.y = data.attribute("position_y").as_int();
	coins[0] = data.attribute("coin1").as_bool();
	coins[1] = data.attribute("coin2").as_bool();
	coins[2] = data.attribute("coin3").as_bool();
	App->render->virtualCamPos = -(virtualPosition.x * (int)App->win->GetScale() - 300);
	if (App->render->virtualCamPos > 0)
	{
		App->render->virtualCamPos = 0;
	}
	if (App->render->virtualCamPos < App->scene->max_camera_pos)
	{
		App->render->virtualCamPos = App->scene->max_camera_pos;
	}

	return true;
}

bool j1Player::Save(pugi::xml_node& data) const
{
	data.append_attribute("position_x") = position.x;

	data.append_attribute("position_y") = position.y - 5;

	data.append_attribute("level") = App->scene->current_lvl->data->lvl;

	data.append_attribute("lives") = lives;

	data.append_attribute("coin1") = coins[0];

	data.append_attribute("coin2") = coins[1];

	data.append_attribute("coin3") = coins[2];

	return true;
}