#include "j1EntityManager.h"

#include "j1App.h"
#include "j1Render.h"
#include "j1Walking_Enemy.h"
#include "j1Flying_Enemy.h"

j1EntityManager::j1EntityManager()
{}

j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	this->config = config;
	return true;
}

bool j1EntityManager::Start()
{
	createEntity(entity_type::WALKING_ENEMY, 600, 50);
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

bool j1EntityManager::PostUpdate()
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->PostUpdate();
		App->render->Blit(entity->data->graphics, entity->data->position.x, entity->data->position.y);
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
