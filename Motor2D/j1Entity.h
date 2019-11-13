#ifndef __MODULE_ENTITY_H__
#define __MODULE_ENTITY_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"

struct SDL_Texture;

enum entity_state
{
	IDLE,
	RIGHT,
	LEFT,
	JUMPING,
	FALLING,
	DAMAGED,
	DEAD,
	NONE

};

enum entity_type
{
	FLYING_ENEMY,
	WALKING_ENEMY,
	NO_ENEMY
};

class Entity : public j1Module
{
public:



	Entity(const char* name);
	bool Entity_Update(float dt);

	void Do_Path();

	void Entity_OnCollision(Collider* c1, Collider* c2);

	virtual bool specificAnimation()
	{
		return false;
	}

private:

	void setAnimation();
	float gravity = 5;

public:

	iPoint position;
	fPoint virtualPosition;
	fPoint v;
	fPoint collider_offset;
	bool colliding_bottom = false;
	bool colliding_right = false;
	bool colliding_left = false;
	bool colliding_top = false;
	bool going_right = false;
	bool going_left = false;
	bool jumping = false;
	bool going_down = false;
	entity_state state = NONE;
	entity_type type = NO_ENEMY;
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
	Animation* dying;
	int pos_relCam = 0;

	Collider* collider = nullptr;
	Collider* collidingFloor = nullptr;

	p2DynArray<iPoint> path_to_player;

	bool sound_one_time = false;
	float killed_finished = 0;

	float speed = 0;
	float jump_force = 0;
	uint max_jump_value = 0;
	bool flying = false;

	float scale = 1;
	iPoint sprite_pos, collider_pos, collider_size;

	bool IsDead = false;
	float prev_dt = 0;

};

#endif // !__MODULE_ENTITY_H__
