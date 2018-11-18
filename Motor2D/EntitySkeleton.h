#ifndef _ENTITYSKELETON_H
#define _ENTITYSKELETON_H

#include "Entity.h"


class EntitySkeleton : public Entity
{

private:
	Animation idle;
	Animation death;
	Animation chasing;
	iPoint skeletonPosition;
	iPoint playerPosition;
	iPoint chase;
	iPoint canwalk;
	bool left;
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