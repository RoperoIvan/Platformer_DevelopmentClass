#ifndef __J1PLAYER_H
#define  __J1PLAYER_H

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2List.h"
#include "j1Collision.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment(lib, "SDL_mixer/libx86/SDL2_mixer.lib")


struct SDL_Texture;
struct SDL_Rect;


struct COLLIDER_INFO {
	Collider* collider = nullptr;
	iPoint offset;
	int width = 0;
	int height = 0;
	COLLIDER_TYPE type;
};

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();


	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	iPoint GetPosition();
	iPoint SetPosition(iPoint playerPos);

	bool Save(pugi::xml_node&)const;

	bool Load(pugi::xml_node&);

	void OnCollision(Collider* c1, Collider* c2);
	void AddColliders();
	void SetCollidersPos();
public:
	bool godMode;
	int feetCollider, leftCollider, rightCollider, headCollider, doubleJump;

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
	COLLIDER_INFO colliderPlayer;
	COLLIDER_INFO colliderPlayer_down;
	COLLIDER_INFO colliderPlayer_up;
	COLLIDER_INFO colliderPlayer_left;
	COLLIDER_INFO colliderPlayer_right;
	bool left;
	bool jumping;
	bool inAir;
	bool cantGoLeft;
	bool cantGoRight;
	bool dontFly;
	bool cantJump;
	bool jumpAgain;
	bool falling = true;
	bool solidGround = false;
	float gravity;
	float jumpPower;
	float maxJumpHeight;
	float jumpSpeed = 27.3f;
};





#endif






