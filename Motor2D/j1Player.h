#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Entity.h"
#include "Animation.h"
#include "PugiXml\src\pugixml.hpp"

class j1Player : public j1Module
{
public:

	j1Player();

	virtual ~j1Player();

	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	/*iPoint GetPosition();*/ //FUNCTION TO MAKE PRIVATE POSITION, PUBLIC IS MESSY CODE

public:

	bool isDead = false;
	iPoint position;//should be private
};

#endif