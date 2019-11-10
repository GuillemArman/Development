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
#include "j1PathFinding.h"
#include "j1Player.h"

j1Flying_Enemy::j1Flying_Enemy() : Entity("Flying")
{
	name.create("flying_enemy");

	speed = 2;
	jump_force = 6;

	if (graphics == nullptr)
		graphics = App->tex->Load("textures/Sprites/FlyingSprites/Enemies.png");

	if (collider == nullptr)
		collider = App->collision->AddCollider({ 0, 0, 97, 72 }, COLLIDER_ENEMY, this);


	//idle_right.Animation_XML("idle_right", "flying_enemy");
	//idle_left.Animation_XML("idle_left", "flying_enemy");
	//
	//animation = &idle_left;
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
	App->pathfinding->getPath(this, App->player->position);
	return true;
}
bool j1Flying_Enemy::PostUpdate()
{
	return true;
}
bool j1Flying_Enemy::CleanUp()
{
	App->tex->UnLoad(graphics);
	return true;
}
void j1Flying_Enemy::OnCollision(Collider* c1, Collider* c2)
{
}
bool j1Flying_Enemy::Load(pugi::xml_node&)
{
	return true;
}
bool j1Flying_Enemy::Save(pugi::xml_node&) const
{
	return true;
}