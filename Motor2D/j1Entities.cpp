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
#include "EntityPlayer.h"
#include "j1FadeToBlack.h"


j1Entities::j1Entities() : j1Module()
{
	name.create("entities");

}

j1Entities::~j1Entities()
{}


bool j1Entities::Awake(pugi::xml_node& config)
{
	textures = config.child("folder").attribute("path").as_string();
	bool ret = true;
	return ret;
}

bool j1Entities::Start()
{
	bool ret = true;
	SpawnEntities(0, 0, PLAYER);
	return ret;
}

bool j1Entities::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool j1Entities::Update(float dt)
{
	bool ret = true;

	for (uint i = 0; i < entities.Count(); i++)
	{
		if (entities.At(i) != nullptr)
			entities[i]->Update(dt);
	}
	for (uint i = 0; i < entities.Count(); i++)
	{
		if (entities[i] != nullptr)
			entities[i]->Draw(entities[i]->sprites);
	}
	return ret;
}

bool j1Entities::PostUpdate()
{
	bool ret = true;
	return ret;
}

bool j1Entities::CleanUp()
{
	App->entities->player->CleanUp();

	return true;
}

bool j1Entities::SpawnEntities(int x, int y, Entities_Type type)
{
	bool ret = false;

	switch (type)
	{
	case Entities_Type::PLAYER: 
		player = new EntityPlayer(x, y);
		player->entity_type = Entities_Type::PLAYER;
		entities.PushBack(player);
		ret = true;
		break;
	}

	return true;
}


void j1Entities::OnCollision(Collider* c1, Collider* c2)
{
	for (int i = 0; i < entities.Count(); i++)
	{
		if (entities[i]->GetCollider() == c1)
			entities[i]->OnCollision(c2);
	}
}


bool j1Entities::Load(pugi::xml_node& data)
{
	bool ret = true;

	for (int i = 0; i < entities.Count(); i++)
	{
		if (entities[i]->entity_type == PLAYER)
		{
			pugi::xml_node player_stats = data.child("player");
			entities[i]->Load(player_stats);
		}
	}

	return ret;
}

bool j1Entities::Save(pugi::xml_node& data)const
{
	bool ret = true;

	for (int i = 0; i < entities.Count(); i++)
	{
		if (entities[i]->entity_type == PLAYER)
		{
			pugi::xml_node player_stats = data.append_child("player");
			entities[i]->Save(player_stats);
		}
	}

	return ret;
}
