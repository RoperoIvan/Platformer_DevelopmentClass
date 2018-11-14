#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class Entity
{
private:
	SDL_Rect last_frame = { 0,0,0,0 };
public:
	Animation * animation = nullptr;
	Collider* collider = nullptr;

public:
	fPoint position = fPoint(0, 0);
	int entity_type = 0;
	bool to_destroy = false;

public:
	Entity(int x, int y);
	virtual ~Entity();

	const Collider* GetCollider() const;

	virtual void Update(float dt) {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);

	virtual void LoadAnimation(pugi::xml_node animation_node, Animation* animation) {};

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	SDL_Texture* sprites;
};

#endif // __ENTITY_H__

