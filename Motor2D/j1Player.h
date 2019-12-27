#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"

struct SDL_Texture;
struct Collider;

class j1Player : public Entity
{
public:
	j1Player();
	~j1Player();

	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

	uint double_jump_fx = 0, jump_fx = 0, step_fx = 0, die_fx = 0, godmode_on = 0, godmode_off = 0, killed_fx = 0;
	float step_time = 0;
	float killed_finished = 0;
	bool sound_one_time = false;
	uint lives = 0;
	uint score = 0;

	Animation* jump = nullptr;
	iPoint jump_offset;
	iPoint jump_pos;

	SDL_Texture* graphics_god = nullptr;
	Animation* godMode = nullptr;
	iPoint godmode_offset;

	bool coins[3];
	
	bool double_jump = false;
	bool won = false;
	bool loading = false;
	float end_reached = 0;
};

#endif