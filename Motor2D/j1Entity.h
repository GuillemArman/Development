#ifndef __MODULE_ENTITY_H__
#define __MODULE_ENTITY_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

//#define gravity -1.3

struct SDL_Texture;

enum entity_state
{
	IDLE,
	RIGHT,
	LEFT,
	JUMPING,
	DAMAGED,
	DEAD

};

enum entity_type
{
	FLYING_ENEMY,
	WALKING_ENEMY
};

class Entity : public j1Module
{
public:

	Entity(const char* name);
	bool Entity_Update(float dt);

	void Entity_OnCollision(Collider* c1, Collider* c2);

private:

	void setAnimation();
	float gravity;

public:

	iPoint position;
	fPoint virtualPosition;
	fPoint v;
	fPoint collider_offset;
	bool colliding_bottom = false;
	bool colliding_right = false;
	bool colliding_left = false;
	bool colliding_top = false;
	entity_state state;
	entity_type type;
	SDL_Texture* graphics = nullptr;
	Animation* animation = nullptr;
	Animation idle_right;
	Animation idle_left;
	Animation right;
	Animation left;
	Animation jumping_left;
	Animation jumping_right;
	Animation dying_right;
	Animation dying_left;
	int pos_relCam;

	Collider* collider;
	Collider* collidingFloor;

	float speed;
	float jump_force;
	
};

#endif // !__MODULE_ENTITY_H__
