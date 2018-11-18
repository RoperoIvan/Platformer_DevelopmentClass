#ifndef __j1COLLISIONS_H__
#define __j1COLLISIONS_H__

#define MAX_COLLIDERS 300

#include "j1Module.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_PLAYER_UP,
	COLLIDER_PLAYER_DOWN,
	COLLIDER_PLAYER_LEFT,
	COLLIDER_PLAYER_RIGHT,
	COLLIDER_PLAYER,
	COLLIDER_PLATFORM,
	COLLIDER_ENEMY,
	COLLIDER_WINCONDITION,
	COLLIDER_DEATHCONDITION,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;
	float damage = 1.0;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collisions : public j1Module
{
public:

	j1Collisions();
	~j1Collisions();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	void DebugDraw();
	Collider * colliders[MAX_COLLIDERS];

private:

	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
	bool god_mode = false;

};

#endif