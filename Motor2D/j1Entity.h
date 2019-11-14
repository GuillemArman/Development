#ifndef __MODULE_ENTITY_H__
#define __MODULE_ENTITY_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"


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
	PLAYER = 0,
	FLYING_ENEMY,
	WALKING_ENEMY,
	NO_ENEMY
};

class Entity : public j1Module
{
public:

	Entity(const char* name);
	bool Entity_Update(float dt);

	bool Calculate_Path();

	void Entity_OnCollision(Collider* c1, Collider* c2);

	virtual bool specificAnimation()
	{
		return false;
	}

	bool Collision_from_bottom(Collider* c1, Collider* c2, int margin = 1) const;
	bool Collision_from_left(Collider* c1, Collider* c2) const;
	bool Collision_from_right(Collider* c1, Collider* c2) const;
	bool Collision_from_top(Collider* c1, Collider* c2) const;

private:

	void setAnimation();
	float gravity = 5;

public:

	iPoint position;
	fPoint virtualPosition;
	fPoint v;
	fPoint collider_offset;
	j1Timer doStandardPath;
	bool counting = false;
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
	Animation* dying = nullptr;
	int pos_relCam = 0;

	Collider* collider = nullptr;
	Collider* collidingFloor = nullptr;

	p2DynArray<iPoint> path_to_player;

	bool sound_one_time = false;
	bool slowerPath = false;
	float killed_finished = 0;

	float speed = 0;
	float jump_force = 0;
	uint max_jump_value = 0;
	bool flying = false;
	bool IsDead = false;
	float prev_dt = 0;
	bool dead = false;

	float scale = 1;
	iPoint sprite_pos, collider_pos, collider_size;


};

#endif // !__MODULE_ENTITY_H__
