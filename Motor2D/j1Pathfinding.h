#ifndef __PATH_FINDING_H__
#define __PATH_FINDING_H__


#include "j1Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "j1Entity.h"

class PathList;

struct PathNode
{
	PathNode();
	PathNode(int g, int h, int jump_value, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	iPoint coords;
	float g, h, F;
	int jump_value;

	uint FindWalkableAdjacents(PathList& list_to_fill, const uint& max_jump_value) const;
	bool touchingGround() const;
	void calculateJumpValue(const uint& max_jump_value);
	int calculateF(const iPoint& destination);

	const PathNode* parent;
};

struct PathList
{
	p2List_item<PathNode>* Find(const iPoint& coords) const;
	p2List_item<PathNode>* FindLowestValue() const;

	p2List<PathNode> list;
};

class j1PathFinding : public j1Module
{
public:

	j1PathFinding()
	{}
	~j1PathFinding()
	{}

	PathList getPath(Entity* entity, const iPoint& destination) const;
	void SetMap(uint width, uint height, uchar* data);
	bool isWalkable(const iPoint& coords) const;

private:

	uint width, height;
	uchar* map;
};

#endif // !__PATH_FINDING_H__