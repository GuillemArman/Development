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
}
bool j1Walking_Enemy::Load(pugi::xml_node&)
{
	return true;
}
bool j1Walking_Enemy::Save(pugi::xml_node&) const
{
	return true;
}