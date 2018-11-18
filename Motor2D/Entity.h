#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class Entity
{
public:
	Animation* animation = nullptr;
	Collider* collider = nullptr;

public:
	iPoint position;
	pugi::xml_document entitiesXML;
	int alpha = 80;
	SDL_Texture* enemypath;

public:
	Entity(int x, int y);
	virtual ~Entity();

	const Collider* GetCollider() const;
	virtual void Update(float dt) = 0;
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider) = 0;
	virtual bool Load(pugi::xml_node&) { return true; }
	virtual bool Save(pugi::xml_node&) const { return true; }


};

#endif // __ENTITY_H__

