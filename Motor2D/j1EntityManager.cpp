#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Walking_Enemy.h"
#include "j1Flying_Enemy.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "p2Defs.h"
#include "j1Scene.h"

j1EntityManager::j1EntityManager()
{}

j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	this->config = config;	
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Awake(config.child(entity->data->name.GetString()));
	}

	return true;
}

bool j1EntityManager::Start()
{
	path_marker = App->tex->Load("textures/non_walkable_tile.png");
	return true;
}

bool j1EntityManager::Update(float dt)
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Entity_Update(dt);
		entity->data->Update(dt);
	}
	return true;
}

bool j1EntityManager::PostUpdate(float dt)
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->PostUpdate(dt);

		

		int i = 0;
		while (i < entity->data->path_to_player.Count())
		{
			iPoint coords = App->map->MapToWorld(entity->data->path_to_player.At(i)->x, entity->data->path_to_player.At(i)->y);
			App->render->Blit(path_marker, coords.x, coords.y);
			i++;
		}
		App->render->Blit(entity->data->graphics, entity->data->position.x, entity->data->position.y, &entity->data->animation->GetCurrentFrame(dt), entity->data->scale);
	}
	return true;
}

bool j1EntityManager::CleanUp()
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->CleanUp();
	}
	return true;
}

Entity* j1EntityManager::createEntity(entity_type type, int x, int y)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case WALKING_ENEMY:
		ret = new j1Walking_Enemy();
		break;
	case FLYING_ENEMY:
		ret = new j1Flying_Enemy();
		ret->flying = true;
		break;
	}
	ret->type = type;
	ret->virtualPosition.x = ret->position.x = x;
	ret->virtualPosition.y = ret->position.y = y;
	entities.add(ret);
	return ret;
}

Entity* j1EntityManager::getPlayer() const
{
	Entity* ret = nullptr;

	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		if (entity->data->type == PLAYER)
		{
			ret = entity->data;
			break;
		}
	}

	return ret;
}

void j1EntityManager::destroyEntity(Entity* to_destroy)
{
	p2List_item<Entity*>* entity_to_destroy = entities.start;

	while (entity_to_destroy != NULL)
	{
		if (entity_to_destroy->data == to_destroy)
		{
			if (entity_to_destroy->data == getPlayer())
				getPlayer()->CleanUp();
			entities.del(entity_to_destroy);
			RELEASE(entity_to_destroy->data);
			break;
		}

		entity_to_destroy = entity_to_destroy->next;
	}
}