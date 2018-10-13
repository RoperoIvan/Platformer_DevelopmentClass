#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void LevelChange();

public:
	bool controllingCamera;
	//int Mix_VolumeMusic(int volume);

private:
	int levelSelector;
	int counter;
	fPoint firstLevelPosition, secondLevelPosition;
	iPoint Wincon1, Wincon2;
	int Width1, Width2, Height1, Height2;
	SDL_Rect winCondition;
	int volume = 0;
};

#endif // __j1SCENE_H__