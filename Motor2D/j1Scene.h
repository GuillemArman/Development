#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2Point.h"

struct SDL_Texture;

struct level
{
	level(int num, char* mapPath)
	{
		this->mapPath.create(mapPath);
		lvl = num;
	}
	p2SString mapPath = nullptr;
	iPoint player_start_pos;
	iPoint end;
	int lvl = 0;
	int length = 0;
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// If num is 0 it will load the next level or the first one if there is no next level
	void LoadLvl(int num = 0);

public:

	p2List<level*> levels;
	p2List_item<level*>* current_lvl;

	float gravity_speed = 0;
	float jump_speed = 0;
	fPoint player_initial_position;

private:
	uint win_width = 0;
	uint win_height = 0;
	SDL_Texture* background = nullptr;
	Collider* pit_collider = nullptr;

};

#endif // __j1SCENE_H__