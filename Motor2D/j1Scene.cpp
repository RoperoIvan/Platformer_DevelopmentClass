#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "j1Entities.h"
#include "j1Pathfinding.h"
#include "Brofiler/Brofiler.h"
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

	///// bool
	controllingCamera = config.child("controllingCamera").attribute("value").as_bool();

	////// float
	firstLevelPosition.x = config.child("firstLevelPosition").attribute("x").as_float();
	firstLevelPosition.y = config.child("firstLevelPosition").attribute("y").as_float();
	secondLevelPosition.x = config.child("secondLevelPosition").attribute("x").as_float();
	secondLevelPosition.y = config.child("secondLevelPosition").attribute("y").as_float();

	///// string
	level1Path = config.child("level1Path").attribute("value").as_string();
	level2Path = config.child("level2Path").attribute("value").as_string();
	song1Path = config.child("song1Path").attribute("value").as_string();
	song2Path = config.child("song2Path").attribute("value").as_string();

	////// int
	collisionCounter = config.child("collisionCounter").attribute("value").as_int();
	levelSelector = config.child("levelSelector").attribute("value").as_int();
	godSelector = config.child("godSelector").attribute("value").as_int();
	Width1 = config.child("Width1").attribute("value").as_int();
	Width2 = config.child("Width2").attribute("value").as_int();
	Height1 = config.child("Height1").attribute("value").as_int();
	Height2 = config.child("Height2").attribute("value").as_int();
	volume = config.child("volume").attribute("value").as_int();
	firstLimit.x = config.child("firstLimit").attribute("x").as_int();
	firstLimit.y = config.child("firstLimit").attribute("y").as_int();
	secondLimit.x = config.child("secondLimit").attribute("x").as_int();
	secondLimit.y = config.child("secondLimit").attribute("y").as_int();
	Wincon1.x = config.child("Wincon1").attribute("x").as_int();
	Wincon1.y = config.child("Wincon1").attribute("y").as_int();
	Wincon2.x = config.child("Wincon2").attribute("x").as_int();
	Wincon2.y = config.child("Wincon2").attribute("y").as_int();
	winCondition = { Wincon1.x,Wincon1.y,Width1,Height1 };

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	App->map->Load(level1Path.GetString());
	
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	
	App->audio->PlayMusic(song1Path.GetString());
	Mix_VolumeMusic(volume);
	SpawnEnemies();
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdateScene", Profiler::Color::Blue);
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("UpdateScene", Profiler::Color::BlueViolet);


	Limits();

	//Debug Functionalities

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		levelSelector = 1;
		LevelChange();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		LevelChange();
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		levelSelector = 2;
		LevelChange();
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		App->LoadGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->SaveGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		collisionCounter++;

		if (collisionCounter == 1)
		{
			App->map->seeCollisions = true;
		}

		if (collisionCounter == 2)
		{
			App->map->seeCollisions = false;
			collisionCounter = 0;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godSelector++;

		if (godSelector == 1)
		{
			App->player->godMode = true;
		}

		if (godSelector == 2)
		{
			App->player->godMode = false;
			godSelector = 0;
		}
	}

	//Control of the music volume
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_REPEAT)
	{
		if (volume < 200)
		{
			Mix_VolumeMusic(volume += 5);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_REPEAT)
	{
		if (volume > 0)
		{
			Mix_VolumeMusic(volume -= 5);
		}
	}


	///// Map drawing

	App->map->Draw();

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
	BROFILER_CATEGORY("PostUpdateScene", Profiler::Color::DarkBlue);
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
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
	if (levelSelector == 2)
	{
		App->player->CleanUp();
		App->entities->CleanUp();
		App->map->CleanUp();
		App->collision->CleanUp();
		App->fade->FadeTo();
		App->player->SetPosition(secondLevelPosition);
		App->player->Start();
		App->entities->Start();
		SpawnEnemies();
		App->collision->Start();

		App->audio->PlayMusic(song2Path.GetString());
		App->map->Load(level2Path.GetString());
	}

	if (levelSelector == 1)
	{
		App->player->CleanUp();
		App->entities->CleanUp();
		App->map->CleanUp();
		App->collision->CleanUp();
		App->fade->FadeTo();
		App->player->SetPosition(firstLevelPosition);
		App->player->Start();
		App->entities->Start();
		SpawnEnemies();
		App->collision->Start();
		App->audio->PlayMusic(song1Path.GetString());
		App->map->Load(level1Path.GetString());
	}
}


///// Save & Load
bool j1Scene::Save(pugi::xml_node& data)const
{
	data.append_child("musicvolume").append_attribute("value") = volume;
	data.append_child("levelSelector").append_attribute("value") = levelSelector;
	data.append_child("Logic").append_attribute("value") = App->map->seeCollisions;

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	volume = Mix_VolumeMusic(data.child("musicvolume").attribute("value").as_int());
	levelSelector = data.child("levelSelector").attribute("value").as_int();
	App->map->seeCollisions = data.child("Logic").attribute("value").as_bool();


	LevelChange();
	LOG("%d", volume);
	LOG("%i", levelSelector);
	return true;
}

void j1Scene::Limits()
{
	if (levelSelector == 1)
	{

		if (App->player->GetPosition().x == firstLimit.x || App->player->GetPosition().y == firstLimit.y)
		{
			LevelChange();
		}
	}

	if (levelSelector == 2)
	{
		if (App->player->GetPosition().x == secondLimit.x || App->player->GetPosition().y == secondLimit.y)
		{
			LevelChange();
		}
	}
}

void j1Scene::SpawnEnemies()
{
	if (levelSelector == 1)
	{
		App->entities->AddEntity(GHOST, 1570, 608);
		App->entities->AddEntity(GHOST, 1626, 417);
		App->entities->AddEntity(GHOST, 1377, 399);
		App->entities->AddEntity(GHOST, 899, 289);
		App->entities->AddEntity(GHOST, 1038, 670);
		App->entities->AddEntity(GHOST, 693, 644);
		App->entities->AddEntity(SKELETON, 560, 257);
		App->entities->AddEntity(SKELETON, 813, 383);
		App->entities->AddEntity(SKELETON, 1135, 384);
		App->entities->AddEntity(SKELETON, 543, 741);
		App->entities->AddEntity(SKELETON, 178, 645);
		App->entities->AddEntity(SKELETON, 1387, 501);
		App->entities->AddEntity(SKELETON, 1390, 627);
		App->entities->AddEntity(SKELETON, 1864, 710);
	}
	if (levelSelector == 2)
	{
		App->entities->AddEntity(GHOST, 595, 194);
		App->entities->AddEntity(GHOST, 398, 456);
		App->entities->AddEntity(GHOST, 474, 449);
		App->entities->AddEntity(SKELETON, 414, 255);
	
		App->entities->AddEntity(SKELETON, 499, 503);;
		App->entities->AddEntity(SKELETON, 657, 568);
	}
	
}
