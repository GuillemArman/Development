#ifndef _Walking_Enemy_
#define _Walking_Enemy_ 

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

class Walking_Enemy : public Entity
{
public:
	Walking_Enemy();
	~Walking_Enemy();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void followPath();
	void standardPath();

public:

	bool moving_right = false;
	bool moving_left = false;
};

#endif // !Walking_Enemy

