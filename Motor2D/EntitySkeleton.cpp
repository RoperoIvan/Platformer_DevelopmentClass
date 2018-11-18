#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "EntitySkeleton.h"
#include "j1Collisions.h"
#include "j1Entities.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Scene.h"

EntitySkeleton::EntitySkeleton(int x, int y) : Entity(x, y)
{


	idle.PushBack({ 0,311,22,32 });
	idle.PushBack({ 24,311,22,32 });
	idle.PushBack({ 48,311,22,32 });
	idle.PushBack({ 72,311,22,32 });
	idle.PushBack({ 97,311,22,32 });
	idle.PushBack({ 122,311,22,32 });
	idle.PushBack({ 146,311,22,32 });
	idle.PushBack({ 169,311,22,32 });
	idle.PushBack({ 192,311,22,32 });
	idle.PushBack({ 216,311,22,32 });
	idle.PushBack({ 240,311,22,32 });
	idle.speed = 0.2f;
	idle.loop = true;

	chasing.PushBack({0,347,18,32});
	chasing.PushBack({ 22,347,18,32 });
	chasing.PushBack({ 44,347,18,32 });
	chasing.PushBack({ 65,346,20,33 });
	chasing.PushBack({ 87,346,21,33 });
	chasing.PushBack({ 109,346,22,33 });
	chasing.PushBack({ 135,347,22,32 });
	chasing.PushBack({ 160,347,22,32 });
	chasing.PushBack({ 185,347,22,32 });
	chasing.PushBack({ 209,347,22,32 });
	chasing.PushBack({ 235,347,20,32 });
	chasing.PushBack({ 258,347,18,32 });
	chasing.PushBack({ 280,347,17,32 });
	chasing.speed = 0.4f;
	chasing.loop = true;

	death.PushBack({ 0,143,24,42 });
	death.PushBack({ 61,148,30,30 });
	death.PushBack({ 123,146,29,26 });
	death.PushBack({ 197,150,15,17 });
	death.PushBack({ 264,153,10,12 });
	death.PushBack({ 324,151,20,18 });
	death.PushBack({ 389,149,17,18 });
	death.speed = 0.4f;
	death.loop = true;
	animation = &idle;

	collider = App->collision->AddCollider({ 0,0,22,32 }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->entities);
}
void EntitySkeleton::Update(float dt)
{
	skeletonPosition = App->map->WorldToMap(position.x, position.y);
	playerPosition = App->player->playerPosition;

	canwalk.x = skeletonPosition.x;
	canwalk.y = skeletonPosition.y +2;

	if (App->pathfinding->IsWalkable(canwalk))
	{
		position.y += 4;
	}

	if (Vision() == true)
	{
		Chasing();
	}
	else
	{
		animation = &idle;
	}
}

void EntitySkeleton::OnCollision(Collider* coll)
{

}

void EntitySkeleton::Chasing()
{
	animation = &chasing;

	

	if (App->pathfinding->CreatePath(skeletonPosition, playerPosition, SKELETON) != -1)
	{
		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
		if (path->Count() > 0)
		{
			chase = iPoint(path->At(0)->x, path->At(0)->y);
			if (chase.x < skeletonPosition.x)
			{
				position.x--;
			}
			else if (chase.x > skeletonPosition.x)
			{
				position.x++;
			}
			/*if (chase.y < skeletonPosition.y)
			{
				position.y--;
			}
			if (chase.y > skeletonPosition.y)
			{
				position.y++;
			}*/
		}
	}
}

bool EntitySkeleton::Vision()
{
	return (App->player->GetPosition().x > position.x - 100
		&& App->player->GetPosition().x < position.x + 100
		&& App->player->GetPosition().y < position.y + 100
		&& App->player->GetPosition().y > position.y - 100);
}

bool EntitySkeleton::Save(pugi::xml_node& data)const
{
	data.append_child("position").append_attribute("x") = position.x;
	data.child("position").append_attribute("y") = position.y;

	return true;
}
bool EntitySkeleton::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();

	return true;
}