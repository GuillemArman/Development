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

j1Flying_Enemy::j1Flying_Enemy() : Entity()
{
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
	return true;
}
bool j1Flying_Enemy::PostUpdate()
{
	return true;
}
bool j1Flying_Enemy::CleanUp()
{
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