#ifndef _ENTITYSKELETON_H
#define _ENTITYSKELETON_H

#include "Entity.h"


class EntitySkeleton : public Entity
{

private:
	Animation idle;
	Animation death;
	Animation chasing;
	SDL_Texture* batex;
	iPoint skeletonPosition;
	iPoint playerPosition;
	iPoint chase;
	iPoint canwalk;
public:

	EntitySkeleton(int x, int y);
	~EntitySkeleton() {};
	void Update(float dt);
	void OnCollision(Collider*);
	void Chasing();
	bool Vision();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;
};











#endif