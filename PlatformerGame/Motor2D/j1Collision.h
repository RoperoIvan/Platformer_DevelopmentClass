#ifndef _j1COLLISION_
#define _j1COLLISION_

#define MAX_COLLIDERS 200

#include "j1Module.h"


enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_GROUND,
	COLLIDER_PLAYER,
	COLLIDER_PLAYER_LEFT,
	COLLIDER_PLAYER_RIGHT,
	COLLIDER_PLAYER_DOWN,
	COLLIDER_PLAYER_UP,
	COLLIDER_MAX,
};


struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;
	bool enable = true;

	Collider() {};
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

class j1Collision : public j1Module
{
public:

	j1Collision();
	~j1Collision();

	//bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	void DebugDraw();
	void GodMode();
	bool godMode = false, 
		godMode_used = false;
	Collider * colliders[MAX_COLLIDERS] = { nullptr };
private:

	
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};

#endif // !_j1COLLISION_