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
	/*bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);*/

private:

	iPoint position;
	iPoint speedPlayer;
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
	bool initialCamera = true;
	bool left = false;
	int gravity;
	float freeFall;
	float heightFall;
};





#endif
