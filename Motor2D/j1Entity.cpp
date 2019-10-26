#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "p2Log.h"

bool Entity::Entity_Update()
{
	if (App->player->GodMode == false)
	{
		gravity = -1.5f;
	}
	else if (App->player->GodMode == true)
	{
		gravity = 0.0f;
	}

	v.y += (gravity * ((colliding_bottom) ? 0 : 1));
	if (v.y < -1)
		v.y = -13;
	virtualPosition.y -= v.y;

	if (pos_relCam > 0 || v.x > 0)
		virtualPosition.x += v.x;

	colliding_right = false;
	colliding_left = false;

	setAnimation();
	return true;
}

void Entity::Entity_OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_FLOOR)
	{
		if (((c2->rect.y - v.y + 1) > (c1->rect.y + (c1->rect.h)))) //The collision is from bottom
		{
			//virtualPosition.y = c2->rect.y - animation->GetCurrentFrame().h;
			if (colliding_bottom == false)
			{
				v.y = 0;
				if (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT || v.x == 0)
				{
					v.x = 0;
					state = IDLE;
				}
				else if (v.x > 0)
				{
					state = RIGHT;
				}
				else if (v.x < 0)
				{
					state = LEFT;
				}
				colliding_bottom = true;
			}
			collidingFloor = c2;
		}
		else if ((c2->rect.x + 7) >(c1->rect.x + (c1->rect.w))) // Collision is from right
		{
			if (v.x > 0)
			{
				v.x = 0;
				state = IDLE;
			}
			colliding_right = true;
		}
		else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 5)) // Collision is from left
		{
			if (v.x < 0)
			{
				v.x = 0;
				state = IDLE;
			}
			colliding_left = true;
		}
		else if ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + 4))
		{
			if (v.y > 0)
			{
				v.y = 0;
			}
		}
		App->player->jump = false;
	}

	if (c2->type == COLLIDER_PIT)
	{

		if (App->player->GodMode == false)
		{
			/*App->player->isDead = true;
			state = DEAD;*/
			state = IDLE;

			App->player->isDead = true;


			App->player->virtualPosition.x = 340;
			App->player->virtualPosition.y = 300;
			App->render->camera.x = 0;
			App->render->camera.y = 0;
			App->render->virtualCamPos = App->render->camera.x;

		}

		/*else
		{
		gravity = 0.0f;
		}*/

		if (c2->type == COLLIDER_END)
		{
			App->scene->LoadLvl(0);
		}
	}
}

void Entity::setAnimation()
{
	if (v.x > 0)
	{
		if (state == JUMPING)
		{
			animation = &jumping_right;
		}
		else
		{
			animation = &right;
		}
	}
	else if (v.x < 0)
	{
		if (state == JUMPING)
		{
			animation = &jumping_left;
		}
		else
		{
			animation = &left;
		}
	}
	else
	{
		if (state == IDLE)
		{
			if (animation == &left || animation == &jumping_left)
			{
				animation = &idle_left;
			}
			else if (animation == &right || animation == &jumping_right)
			{
				animation = &idle_right;
			}
		}
		else if (state == JUMPING)
		{
			if (animation == &idle_left)
			{
				animation = &jumping_left;
			}
			else if (animation == &idle_right)
			{
				animation = &jumping_right;
			}
		}
		else if (state == DEAD)
		{
			if (animation == &idle_right || animation == &right || animation == &jumping_right)
			{
				animation = &dying_right;
			}
			else if (animation == &idle_left || animation == &left || animation == &jumping_left)
			{
				animation = &dying_left;
			}

		} 
	}
}
