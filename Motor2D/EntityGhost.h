#ifndef _ENTITYGHOST_H
#define _ENTITYGHOST_H

#include "Entity.h"


class EntityGhost : public Entity
{

private:
	Animation idle;
	SDL_Texture* batex;
	iPoint ghostPos;
	iPoint playerPos;
	iPoint chase;
public:

	EntityGhost(int x, int y);
	~EntityGhost() {};
	void Update(float dt);
	void OnCollision(Collider*);
	bool Vision();
};











#endif