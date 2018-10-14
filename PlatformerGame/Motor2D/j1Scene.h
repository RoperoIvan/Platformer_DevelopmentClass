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

	bool Save(pugi::xml_node&)const;

	bool Load(pugi::xml_node&);

public:
	bool controllingCamera;

private:
	int levelSelector, godSelector;
	int counter;
	iPoint firstLevelPosition, secondLevelPosition;
	iPoint Wincon1, Wincon2;
	int Width1, Width2, Height1, Height2;
	SDL_Rect winCondition;
	int volume;
	p2SString level1Path, level2Path, song1Path, song2Path;
};

#endif // __j1SCENE_H__