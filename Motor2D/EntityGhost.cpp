#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "EntityGhost.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "j1Entities.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Scene.h"

EntityGhost::EntityGhost(int x, int y) : Entity(x, y)
{
	

	idle.PushBack({ 0,44,24,42 });
	idle.PushBack({ 62,46,28,41 });
	idle.PushBack({ 127,47,26,42 });
	idle.PushBack({ 192,48,24,42 });
	idle.PushBack({ 256,49,24,42 });
	idle.PushBack({ 319,46,25,44 });
	idle.PushBack({ 383,45,25,43 });
	idle.speed = 0.2f;
	idle.loop = true;

	idleLeft.PushBack({ 1331,44,24,42 });
	idleLeft.PushBack({ 1265,46,28,41 });
	idleLeft.PushBack({ 1202,47,26,42 });
	idleLeft.PushBack({ 1139,48,24,42 });
	idleLeft.PushBack({ 1075,49,24,42 });
	idleLeft.PushBack({ 1011,46,25,44 });
	idleLeft.PushBack({ 947,45,25,43 });
	idleLeft.speed = 0.2f;
	idleLeft.loop = true;
	
	chasing.PushBack({59,96,31,42});
	chasing.PushBack({116,94,46,42});
	chasing.PushBack({180,93,44,43});
	chasing.speed = 0.2f;
	chasing.loop = false;

	chasingLeft.PushBack({ 1331,96,31,42 });
	chasingLeft.PushBack({ 1265,94,46,42 });
	chasingLeft.PushBack({ 180,93,44,43 });
	chasingLeft.speed = 0.2f;
	chasingLeft.loop = false;

	death.PushBack({0,143,24,42});
	death.PushBack({61,148,30,30 });
	death.PushBack({ 123,146,29,26 });
	death.PushBack({ 197,150,15,17 });
	death.PushBack({ 264,153,10,12 });
	death.PushBack({ 324,151,20,18 });
	death.PushBack({ 389,149,17,18 });
	death.speed = 0.4f;
	death.loop = true;
	animation = &idle;

	deathLeft.PushBack({ 0,143,24,42 });
	deathLeft.PushBack({ 61,148,30,30 });
	deathLeft.PushBack({ 123,146,29,26 });
	deathLeft.PushBack({ 197,150,15,17 });
	deathLeft.PushBack({ 264,153,10,12 });
	deathLeft.PushBack({ 324,151,20,18 });
	deathLeft.PushBack({ 389,149,17,18 });
	deathLeft.speed = 0.4f;
	deathLeft.loop = true;
	animation = &idle;

	collider = App->collision->AddCollider({ 0,0,26,21 }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->entities);
}
void EntityGhost::Update(float dt)
{
	if (Vision() == true)
	{
		Chasing();
	}
	else
	{
		animation = &idle;
	}
}

void EntityGhost::OnCollision(Collider* coll)
{

}

void EntityGhost::Chasing()
{
	animation = &chasing;
	ghostPosition = App->map->WorldToMap(position.x, position.y);
	playerPosition = App->player->playerPosition;

	if (App->pathfinding->CreatePath(ghostPosition, playerPosition, GHOST) != -1)
	{
		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
		if (path->Count() > 0)
		{
			chase = iPoint(path->At(0)->x, path->At(0)->y);
			if (chase.x < ghostPosition.x)
			{
				position.x--;
			}
			else if (chase.x > ghostPosition.x)
			{
				position.x++;
			}
			if (chase.y < ghostPosition.y)
			{
				position.y--;
			}
			if (chase.y > ghostPosition.y)
			{
				position.y++;
			}
		}
	}
}

bool EntityGhost::Vision()
{
	return (App->player->GetPosition().x > position.x - 100 
		&& App->player->GetPosition().x < position.x +100 
		&& App->player->GetPosition().y < position.y + 100 
		&& App->player->GetPosition().y > position.y - 100);
}

bool EntityGhost::Save(pugi::xml_node& data)const
{
	data.append_child("position").append_attribute("x") = position.x;
	data.child("position").append_attribute("y") = position.y;

	return true;
}
bool EntityGhost::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	
	return true;
}