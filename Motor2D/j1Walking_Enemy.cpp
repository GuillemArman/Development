#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Walking_Enemy.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1EntityManager.h"

j1Walking_Enemy::j1Walking_Enemy() : Entity("walking_enemy")
{
	name.create("walking_enemy");

	speed = 2;
	jump_force = 6;



	graphics = App->tex->Load("textures/Sprites/FlyingSprites/Enemies.png");
	collider = App->collision->AddCollider({ 0, 0, 97, 72 }, COLLIDER_ENEMY, this);


	idle_right.Animation_XML("idle_right", "walking_enemy");
	jumping_right.Animation_XML("jumping_right", "walking_enemy");
	dying_right.Animation_XML("dying_right", "walking_enemy");
	idle_left.Animation_XML("idle_left", "walking_enemy");
	jumping_left.Animation_XML("jumping_left", "walking_enemy");
	dying_left.Animation_XML("dying_left", "walking_enemy");

	animation = &idle_left;
}
j1Walking_Enemy::~j1Walking_Enemy()
{
}
bool j1Walking_Enemy::Awake(pugi::xml_node& config)
{
	return true;
}
bool j1Walking_Enemy::Start()
{

	return true;
}
bool j1Walking_Enemy::Update(float dt)
{
	if (!dead)
	{
		if (!Calculate_Path())
		{
			if (!counting)
			{
				doStandardPath.Start();
				counting = true;
			}
			if (doStandardPath.Read() >= 1500)
			{
				standardPath();
				slowerPath = true;
			}
		}
		else
		{
			counting = false;
			slowerPath = false;
		}

		if (path_to_player.Count() > 1)
			followPath();
	}


	return true;
}
bool j1Walking_Enemy::PostUpdate(float dt)
{
	return true;
}
bool j1Walking_Enemy::CleanUp()
{
	return true;
}
void j1Walking_Enemy::OnCollision(Collider* c1, Collider* c2)
{
	Entity_OnCollision(c1, c2);
}
bool j1Walking_Enemy::Load(pugi::xml_node&)
{
	return true;
}
bool j1Walking_Enemy::Save(pugi::xml_node&) const
{
	return true;
}


void j1Walking_Enemy::followPath()
{
	iPoint next_cell;
	next_cell = *path_to_player.At(1);
	next_cell = App->map->MapToWorld(next_cell.x, next_cell.y);
	next_cell = { next_cell.x + App->map->data.tile_width / 2, next_cell.y };
	iPoint map_pos(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);

	float usingSpeed = (slowerPath) ? (speed / 2) : speed;

	if (next_cell.x > map_pos.x)
	{
		v.x = usingSpeed;
		state = RIGHT;
		going_right = true;
		going_left = false;
	}
	else if (next_cell.x < map_pos.x)
	{
		v.x = -usingSpeed;
		state = LEFT;
		going_left = true;
		going_right = false;
	}
	else
	{
		v.x = 0;
		going_right = false;
		going_left = false;
	}

	if (next_cell.y > map_pos.y)
	{
		state = FALLING;
		going_down = true;
	}
}


void j1Walking_Enemy::standardPath()
{
	iPoint curr_pos = App->map->WorldToMap(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);
	iPoint rightCell(curr_pos.x - 1, curr_pos.y);
	iPoint leftCell(curr_pos.x + 1, curr_pos.y);
	path_to_player.PushBack(curr_pos);

	if (moving_right && App->pathfinding->isWalkable(rightCell) && App->pathfinding->isTouchingGround(rightCell))
		path_to_player.PushBack(rightCell);
	else if (!moving_left)
	{
		path_to_player.PushBack(curr_pos);
		moving_right = false;
		moving_left = true;
	}
	if (moving_left && App->pathfinding->isWalkable(leftCell) && App->pathfinding->isTouchingGround(leftCell))
		path_to_player.PushBack(leftCell);
	else if (!moving_right)
	{
		path_to_player.PushBack(curr_pos);
		moving_right = true;
		moving_left = false;
	}
}