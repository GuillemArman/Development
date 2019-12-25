#include "j1Collectible.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1UIScene.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "Brofiler\Brofiler.h"
#include "j1EntityManager.h"


Collectible::Collectible(int id) : Entity("collectible")
{
	graphics = App->tex->Load("textures/Sprites/png/Object/coin.png");
	ghost_graphics = App->tex->Load("textures/Sprites/png/Object/ghost_coin.png");
	scale = 0.2f;
	collider = App->collision->AddCollider({ position.x, position.y, (int)(collider_size.x*scale), (int)(collider_size.y*scale) }, COLLIDER_COLLECTIBLE, this, this);
	collider_offset.x *= scale;
	collider_offset.y *= scale;

	this->id = id;

	if (earn_coin_fx == 0)
		earn_coin_fx = App->audio->LoadFx("audio/fx/earn_coin.wav");
	j1Player* player = (j1Player*)App->entityManager->getPlayer();
	if (id == 1 && player->coin1)
	{
		SDL_Texture* tmp = graphics;
		graphics = ghost_graphics;
		ghost_graphics = tmp;
	}
	else if (id == 2 && player->coin2)
	{
		SDL_Texture* tmp = graphics;
		graphics = ghost_graphics;
		ghost_graphics = tmp;
	}
	else if (id == 3 && player->coin3)
	{
		SDL_Texture* tmp = graphics;
		graphics = ghost_graphics;
		ghost_graphics = tmp;
	}

}
Collectible::~Collectible()
{
}
bool Collectible::Awake(pugi::xml_node&)
{
	return true;
}
bool Collectible::Update(float dt)
{
	BROFILER_CATEGORY("Collectible Update", Profiler::Color::Cyan);
	return true;
}
bool Collectible::PostUpdate(float dt)
{
	return true;
}
bool Collectible::CleanUp()
{
	App->tex->UnLoad(ghost_graphics);
	ghost_graphics = nullptr;
	return true;
}
void Collectible::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER && !App->entityManager->getPlayer()->dead)
	{
		App->audio->PlayFx(earn_coin_fx, 0);
		App->entityManager->DeleteEntity(this);

		j1Player* player = (j1Player*)App->entityManager->getPlayer();
		if (id == 1 && !player->coin1)
		{
			player->coin1 = true;
		}
		else if (id == 2 && !player->coin2)
		{
			player->coin2 = true;
		}
		else if (id == 3 && !player->coin3)
		{
			player->coin3 = true;
		}
	}
}
bool Collectible::Load(pugi::xml_node&)
{
	return true;
}
bool Collectible::Save(pugi::xml_node&) const
{
	return true;
}