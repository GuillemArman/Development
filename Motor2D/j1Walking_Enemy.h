#ifndef __WALKING_ENEMY_H_
#define __WALKING_ENEMY_H_

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

class j1Walking_Enemy : public Entity
{
public:
	j1Walking_Enemy();
	~j1Walking_Enemy();
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
#endif // !__WALKING_ENEMY_H_
