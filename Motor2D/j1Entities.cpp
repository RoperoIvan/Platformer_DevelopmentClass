#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Entities.h"
#include "Entity.h"
#include "Brofiler/Brofiler.h"
#include "j1FadeToBlack.h"

#define SPAWN_MARGIN 50
#define SCREEN_SIZE 1

j1Entities::j1Entities() : j1Module()
{
	name.create("entities");

}

j1Entities::~j1Entities()
{}


bool j1Entities::Awake(pugi::xml_node& config)
{
	bool ret = true;
	path = config.child("folder").attribute("path").as_string();
	return ret;
}

bool j1Entities::Start()
{
	bool ret = true;
	textures = App->tex->Load("enemies/enemies.png");

	return ret;
}

bool j1Entities::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdateEntity", Profiler::Color::Pink);
	bool ret = true;
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENTITY; ++i)
	{
		if (queue[i].type != EntitiesType::NOTYPE)
		{
			if (queue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				SpawnEntity(queue[i]);
				queue[i].type = EntitiesType::NOTYPE;
				LOG("Spawning entity at %d", queue[i].x* SCREEN_SIZE);
			}
		}
	}
	return ret;
}

bool j1Entities::Update(float dt)
{
	BROFILER_CATEGORY("UpdateEntity", Profiler::Color::LightPink);
	bool ret = true;

	for (uint i = 0; i < MAX_ENTITY; ++i)
		if (entities[i] != nullptr)
			entities[i]->Update(dt);

	for (uint i = 0; i < MAX_ENTITY; ++i)
		if (entities[i] != nullptr) entities[i]->Draw(textures);
	return ret;
}

bool j1Entities::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateEntity", Profiler::Color::HotPink);
	bool ret = true;

	// check camera position to decide what to despawn
	for (uint i = 0; i < MAX_ENTITY; ++i)
	{
		if (entities[i] != nullptr)
		{
			if (entities[i]->position.x * SCREEN_SIZE < (App->render->camera.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning entity at %d", entities[i]->position.x * SCREEN_SIZE);
				delete entities[i];
				entities[i] = nullptr;
			}
		}
	}

	return ret;
}

bool j1Entities::CleanUp()
{
	LOG("Freeing all entities");

	for (uint i = 0; i < MAX_ENTITY; ++i)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
		if (queue[i].type != EntitiesType::NOTYPE)
		{
			queue[i].type = EntitiesType::NOTYPE;
		}
	}


	return true;
}

bool j1Entities::AddEntity(EntitiesType type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENTITY; ++i)
	{
		if (queue[i].type == EntitiesType::NOTYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void j1Entities::SpawnEntity(const EntityInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for (; entities[i] != nullptr && i < MAX_ENTITY; ++i);

	if (i != MAX_ENTITY)
	{
		switch (info.type)
		{
			/*case EntitiesType::PLAYER:
			entities[i] = new EntityPlayer(info.x, info.y);*/
		case EntitiesType::GHOST:
			entities[i] = new EntityGhost(info.x, info.y);
			break;
		case EntitiesType::SKELETON:
			entities[i] = new EntitySkeleton(info.x, info.y);
			break;
		}
	}
}


void j1Entities::OnCollision(Collider* c1, Collider* c2)
{
	for (int i = 0; i < MAX_ENTITY; i++)
	{
		if (entities[i] != nullptr && entities[i]->GetCollider() == c1)
		{
			entities[i]->OnCollision(c2);
			delete entities[i];
			entities[i] = nullptr;
		}

	}
}

bool j1Entities::Load(pugi::xml_node& data)
{
	bool ret = true;

	for (int i = 0; i < MAX_ENTITY; i++)
	{
		if (entities[i] != nullptr)
		{
			/*if (queue[i].type == EntitiesType::GHOST)
			{*/
				pugi::xml_node ghostData = data.child("ghost");
				entities[i]->Load(ghostData);
			/*}*/
		/*	if (queue[i].type == EntitiesType::SKELETON)
			{
				pugi::xml_node skeletonData = data.child("skeleton");
				entities[i]->Load(skeletonData);
			}*/
		}	
	}

	return ret;
}

bool j1Entities::Save(pugi::xml_node& data)const
{
	bool ret = true;

	for (int i = 0; i < MAX_ENTITY; i++)
	{
		if (entities[i] != nullptr)
		{
			/*if (queue[i].type == EntitiesType::GHOST)
			{*/
				pugi::xml_node ghostData = data.append_child("ghost");
				entities[i]->Save(ghostData);
				LOG("GOLA");
			/*}*/
		/*	if (queue[i].type == EntitiesType::SKELETON)
			{
				pugi::xml_node skeletonData = data.append_child("skeleton");
				entities[i]->Save(skeletonData);
			}*/
		}		
	}

	return ret;
}