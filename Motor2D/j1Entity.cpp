#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"

bool Entity::Entity_Update()
{
	if (pos_relCam > 0 || v.x > 0)
		virtualPosition.x += v.x;

	return true;
}



void Entity::setAnimation()
{

}