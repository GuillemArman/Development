#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Entity.h"
#include "Animation.h"
#include "PugiXml\src\pugixml.hpp"

#define diff 50;

struct SDL_Texture;
struct Collider;

class j1Player : public Entity
{
public:

	j1Player();
	virtual ~j1Player();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	void OnCollision(Collider* c1, Collider* c2);
	/*iPoint GetPosition();*/ //FUNCTION TO MAKE PRIVATE POSITION, PUBLIC IS MESSY CODE

public:

	bool isDead;
	SDL_Texture* santa;
	Collider* player_collider;
	Collider* pit_collider;
	fPoint collider_move;
	bool touchingFloor;
	bool jump = false;
	int jumps = 0;
	bool won;

	//FX
	uint jump_sound;
	uint walking_sound;
};

#endif
