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

j1Walking_Enemy::j1Walking_Enemy() : Entity("walking")
{
	//graphics = App->tex->Load("textures/Sprites/FlyingSprites/Enemies.png");

	speed = 2;
	jump_force = 6;

	collider = App->collision->AddCollider({ 0, 0, 97, 72 }, COLLIDER_ENEMY, this);
}
j1Walking_Enemy::~j1Walking_Enemy()
{
}
bool j1Walking_Enemy::Awake(pugi::xml_node&)
{
	return true;
}
bool j1Walking_Enemy::Start()
{
	if (graphics == nullptr)
		graphics = App->tex->Load("textures/Sprites/FlyingSprites/Enemies.png");


	virtualPosition.x = position.x;
	virtualPosition.y = position.y;


	return true;
}
bool j1Walking_Enemy::Update(float dt)
{
	return true;
}
bool j1Walking_Enemy::PostUpdate()
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