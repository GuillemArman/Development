#ifndef __FLYING_ENEMY__
#define __FLYING_ENEMY__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

class Flying_Enemy : public Entity
{
public:
	Flying_Enemy();
	~Flying_Enemy();

	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void followPath();
	void standardPath();

public:
	uint die_fx = 0;
};

#endif // !Flying_Enemy
