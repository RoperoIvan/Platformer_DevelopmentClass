#ifndef __J1PLAYER_H
#define  __J1PLAYER_H

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment(lib, "SDL_mixer/libx86/SDL2_mixer.lib")


struct SDL_Texture;
struct SDL_Rect;

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
	void ChargingAnimations(pugi::xml_node&, Animation*);

public:
	bool godMode;
	int feetCollider, leftCollider, rightCollider, headCollider, doublejump = 0;

private:
	iPoint sizePlayer;
	iPoint position;
	fPoint speedPlayer;
	fPoint dashSpeed;
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
	Animation death;
	Animation deathLeft;
	Animation dash;
	Animation dashLeft;
	Animation slide;
	Animation* currentAnimation = nullptr;
	bool initialCamera;
	bool left;
	bool jumping;
	bool inAir = false;
	bool cantgoleft = false;
	bool cantgoright = false;
	bool dontFly = false;
	bool cantjump = false;
	bool hasJumped = false;
	//bool hasDashed;
	bool jumpAgain;
	//bool dashAgain;
	float gravity;
	float jumpPower;
	//float dashPower;
	float maxJumpHeight;
	//float dashLength;
	bool initialPosCamera = true;
	
};





#endif
