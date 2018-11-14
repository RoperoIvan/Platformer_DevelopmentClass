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

	return ret;
}

bool j1Entities::PostUpdate()
{
	bool ret = true;
	return ret;
}

bool j1Entities::CleanUp()
{

	return true;
}

bool j1Entities::SpawnEntities(int x, int y, Entities_Type type)
{
	bool ret = false;


	return true;
}


void j1Entities::OnCollision(Collider* c1, Collider* c2)
{


}


bool j1Entities::Load(pugi::xml_node& data)
{
	bool ret = true;

	return ret;
}

bool j1Entities::Save(pugi::xml_node& data)const
{
	bool ret = true;

	return ret;
}
