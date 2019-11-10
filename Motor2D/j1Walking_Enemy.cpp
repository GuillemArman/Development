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
	Do_Path();
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