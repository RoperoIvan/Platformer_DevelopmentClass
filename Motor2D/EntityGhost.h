#ifndef _ENTITYGHOST_H
#define _ENTITYGHOST_H

#include "Entity.h"


class EntityGhost : public Entity
{

private:
	Animation idle;
	Animation death;
	SDL_Texture* batex;
	iPoint ghostPosition;
	iPoint playerPosition;
	iPoint chase;
public:

	EntityGhost(int x, int y);
	~EntityGhost() {};
	void Update(float dt);
	void OnCollision(Collider*);
	void Chasing();
	bool Vision();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;
};











#endif