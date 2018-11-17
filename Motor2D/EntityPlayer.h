#ifndef __ENTITYPLAYER_H
#define  __ENTITYPLAYER_H

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "Entity.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment(lib, "SDL_mixer/libx86/SDL2_mixer.lib")


struct SDL_Texture;
struct SDL_Rect;

class EntityPlayer : public Entity
{
public:
	EntityPlayer(int x, int y);
	~EntityPlayer();


	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	void Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	iPoint GetPosition();
	iPoint SetPosition(iPoint playerPos);
	bool Save(pugi::xml_node&)const;
	bool Load(pugi::xml_node&);
	void OnCollision(Collider*, Collider*);

public:
	bool godMode;
	Collider* feetCollider = nullptr;
	Collider* leftCollider = nullptr;
	Collider* rightCollider = nullptr;
	Collider* headCollider = nullptr;

private:
	iPoint position;
	fPoint speedPlayer;
	p2SString path;
	SDL_Texture* playerTexture;
	Animation idle;
	Animation idleLeft;
	Animation move;
	Animation moveLeft;
	Animation jump;
	Animation fall;
	Animation jumpLeft;
	Animation fallLeft;
	Animation landing;
	Animation landingLeft;
	Animation* currentAnimation = nullptr;
	bool left;
	bool jumping;
	float gravity;
	float jumpPower;
	float maxJumpHeight;
	int doubleJump;
	bool solidGround = false;
};





#endif
