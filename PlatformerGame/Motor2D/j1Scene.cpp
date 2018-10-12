#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	controllingCamera = config.child("controllingCamera").attribute("value").as_bool();
	counter = config.child("counter").attribute("value").as_int();
	levelSelector = config.child("levelSelector").attribute("value").as_int();
	firstLevelPosition.x = config.child("firstLevelPosition").attribute("x").as_float();
	firstLevelPosition.y = config.child("firstLevelPosition").attribute("y").as_float();
	secondLevelPosition.x = config.child("secondLevelPosition").attribute("x").as_float();
	secondLevelPosition.y = config.child("secondLevelPosition").attribute("y").as_float();
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	
		App->map->Load("level1.tmx");
		App->audio->PlayMusic("audio/Grasslands_Theme.ogg");	

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	


	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 10;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 10;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 10;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 10;

	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT)
	{
		controllingCamera = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		counter++;
		if (counter == 1)
		{
			App->map->seeCollisions = true;
			
		}
		if (counter == 2)
		{
			App->map->seeCollisions = false;
			counter = 0;
		}
		
	}


	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		LevelChange();
	}
	
	App->map->Draw();

	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tileSets.count());

	App->win->SetTitle(title.GetString());
	return true;
}


// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	return true;
}


void j1Scene::LevelChange()
{

	if (levelSelector == 0)
	{
		App->player->CleanUp();
		App->map->CleanUp();
		App->fade->FadeTo();
		App->player->SetPosition(secondLevelPosition);
		App->player->Start();
		App->map->Load("level2.tmx");
	}
	levelSelector++;
	if (levelSelector == 2)
	{
		App->player->CleanUp();
		App->map->CleanUp();
		App->fade->FadeTo();
		App->player->SetPosition(firstLevelPosition);
		App->player->Start();
		App->map->Load("level1.tmx");
		levelSelector = 0;
	}


}
