#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Flying_Enemy.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1EntityManager.h"
#include "Brofiler\Brofiler.h"
#include <time.h>

j1Flying_Enemy::j1Flying_Enemy() : Entity ("flying_enemy")
{

	name.create("flying_enemy");

	if (graphics == nullptr)
		graphics = App->tex->Load("textures/Sprites/FlyingSprites/Enemies.png");

	if (collider == nullptr) 
		collider = App->collision->AddCollider({ 0, 0, 97, 72 }, COLLIDER_ENEMY, this);



	idle_right.Animation_XML("idle_right", "flying_enemy");
	idle_left.Animation_XML("idle_left", "flying_enemy");

	animation = &idle_left;

 
}
j1Flying_Enemy::~j1Flying_Enemy()
{
}

bool j1Flying_Enemy::Awake(pugi::xml_node&)
{
	return true;
}
bool j1Flying_Enemy::Start()
{
	return true;
}
bool j1Flying_Enemy::Update(float dt)
{
	//BROFILER_CATEGORY("Flying Update", Profiler::Color::Red);
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

	//if (dying->Finished())
	// 	App->entityManager->destroyEntity(this);

	return true;
}
bool j1Flying_Enemy::PostUpdate(float dt)
{
	return true;
}
bool j1Flying_Enemy::CleanUp()
{
	return true;
}
void j1Flying_Enemy::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER && dead && !App->entityManager->getPlayer()->dead && Collision_from_top(c1, c2))
	{
		// FX
		v = { 0,0 };
	}
}
bool j1Flying_Enemy::Load(pugi::xml_node&)
{
	return true;
}
bool j1Flying_Enemy::Save(pugi::xml_node&) const
{
	return true;
}

void j1Flying_Enemy::standardPath()
{
	srand(time(NULL));

	int newPosX = rand() % 10;
	newPosX = (newPosX < 5) ? 1 : -1;
	int newPosY = rand() % 10;
	newPosY = (newPosY < 5) ? 1 : -1;

	iPoint curr_pos = App->map->WorldToMap(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);

	curr_pos = { curr_pos.x + newPosX, curr_pos.y + newPosY }; // 1
	if (App->pathfinding->isWalkable(curr_pos))
		path_to_player.PushBack(curr_pos);

	curr_pos = { curr_pos.x + newPosX, curr_pos.y + newPosY }; // 2
	if (App->pathfinding->isWalkable(curr_pos))
		path_to_player.PushBack(curr_pos);
}

void j1Flying_Enemy::followPath()
{
	iPoint curr_cell;
	iPoint next_cell;
	curr_cell = *path_to_player.At(1);
	if (path_to_player.Count() > 2)
		next_cell = *path_to_player.At(2);
	iPoint map_pos(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);
	iPoint map_cell = App->map->WorldToMap(map_pos.x, map_pos.y);

	iPoint curr_pos = App->map->MapToWorld(curr_cell.x, curr_cell.y);
	curr_pos = { curr_pos.x + App->map->data.tile_width / 2, curr_pos.y + App->map->data.tile_height / 2 };
	iPoint next_pos = App->map->MapToWorld(next_cell.x, next_cell.y);
	next_pos = { next_pos.x + App->map->data.tile_width / 2, next_pos.y + App->map->data.tile_height / 2 };

	float usingSpeed = (slowerPath) ? (speed / 2) : speed;

	if (curr_pos.x > map_pos.x)
	{
		v.x = usingSpeed;
		if (curr_cell.x > map_cell.x)
			state = RIGHT;
	}
	else if (curr_pos.x < map_pos.x)
	{
		v.x = -usingSpeed;
		if (curr_cell.x < map_cell.x)
			state = LEFT;
	}
	else
		v.x = 0;

	if (curr_pos.y > map_pos.y || (next_pos.y > map_pos.y && !App->pathfinding->isTouchingGround({ map_cell.x, map_cell.y + 1 })))
	{
		v.y = -usingSpeed;
	}
	else if (curr_pos.y < map_pos.y || (next_pos.y < map_pos.y && !App->pathfinding->isTouchingGround({ map_cell.x, map_cell.y - 2 })))
	{
		v.y = usingSpeed;
	}
	else
		v.y = 0;
}