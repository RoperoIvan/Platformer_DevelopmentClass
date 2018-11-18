#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "EntitySkeleton.h"
#include "j1Collisions.h"
#include "j1Entities.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Scene.h"

EntitySkeleton::EntitySkeleton(int x, int y) : Entity(x, y)
{
	pugi::xml_parse_result result = entitiesXML.load_file("entities.xml");

	enemypath = App->tex->Load("enemies/enemiespath.png");

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

	idleLeft.PushBack({ 1333,311,22,32 });
	idleLeft.PushBack({ 1309,311,22,32 });
	idleLeft.PushBack({ 1285,311,22,32 });
	idleLeft.PushBack({ 1261,311,22,32 });
	idleLeft.PushBack({ 1236,311,22,32 });
	idleLeft.PushBack({ 1211,311,22,32 });
	idleLeft.PushBack({ 1187,311,22,32 });
	idleLeft.PushBack({ 1164,311,22,32 });
	idleLeft.PushBack({ 1141,311,22,32 });
	idleLeft.PushBack({ 1117,311,22,32 });
	idleLeft.PushBack({ 1093,311,22,32 });
	idleLeft.speed = 0.2f;
	idleLeft.loop = true;

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

	chasingLeft.PushBack({ 1337,347,18,32 });
	chasingLeft.PushBack({ 1315,347,18,32 });
	chasingLeft.PushBack({ 1293,347,18,32 });
	chasingLeft.PushBack({ 1270,346,20,33 });
	chasingLeft.PushBack({ 1247,346,21,33 });
	chasingLeft.PushBack({ 1224,346,22,33 });
	chasingLeft.PushBack({ 1198,347,22,32 });
	chasingLeft.PushBack({ 1173,347,22,32 });
	chasingLeft.PushBack({ 1148,347,22,32 });
	chasingLeft.PushBack({ 1124,347,22,32 });
	chasingLeft.PushBack({ 1100,347,20,32 });
	chasingLeft.PushBack({ 1079,347,18,32 });
	chasingLeft.PushBack({ 1058,347,17,32 });
	chasingLeft.speed = 0.4f;
	chasingLeft.loop = true;

	death.PushBack({ 0,275,22,32 });
	death.PushBack({ 30,275,23,32 });
	death.PushBack({ 62,275,23,32 });
	death.PushBack({ 93,275,24,32 });
	death.PushBack({ 126,275,24,32 });
	death.PushBack({ 159,275,24,32 });
	death.PushBack({ 192,275,24,32 });
	death.PushBack({ 225,275,24,32 });
	death.PushBack({ 258,276,23,32 });
	death.PushBack({ 290,279,23,32 });
	death.PushBack({ 323,282,24,32 });
	death.PushBack({ 356,291,25,32 });
	death.PushBack({ 385,299,30,32 });
	death.PushBack({ 418,300,30,32 });
	death.PushBack({ 451,300,30,32 });
	death.speed = 0.4f;
	death.loop = true;
	animation = &idle;

	deathLeft.PushBack({ 1333,275,22,32 });
	deathLeft.PushBack({ 1302,275,23,32 });
	deathLeft.PushBack({ 1270,275,23,32 });
	deathLeft.PushBack({ 1238,275,24,32 });
	deathLeft.PushBack({ 1205,275,24,32 });
	deathLeft.PushBack({ 1172,275,24,32 });
	deathLeft.PushBack({ 1139,275,24,32 });
	deathLeft.PushBack({ 1106,275,24,32 });
	deathLeft.PushBack({ 1074,276,23,32 });
	deathLeft.PushBack({ 1042,279,23,32 });
	deathLeft.PushBack({ 1008,282,24,32 });
	deathLeft.PushBack({ 974,291,25,32 });
	deathLeft.PushBack({ 940,299,30,32 });
	deathLeft.PushBack({ 907,300,30,32 });
	deathLeft.PushBack({ 874,300,30,32 });
	deathLeft.speed = 0.4f;
	deathLeft.loop = true;
	animation = &idle;

	collider = App->collision->AddCollider({ 0,0,22,17 }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->entities);
	/*collider2 = App->collision->AddCollider({ 0,11,22,14 }, COLLIDER_TYPE::COLLIDER_ENEMY_BODY, (j1Module*)App->entities);*/
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
		if (left)
		{
			animation = &idleLeft;
		}
		else
		{
			animation = &idle;
		}
	}
}

void EntitySkeleton::OnCollision(Collider* coll)
{

}

void EntitySkeleton::Chasing()
{
	if (App->pathfinding->CreatePath(skeletonPosition, playerPosition, SKELETON) != -1)
	{
		const p2DynArray<iPoint>* breadcrumbs = App->pathfinding->GetLastPath();
		if (App->map->seeCollisions)
		{
			for (int i = 0; i < breadcrumbs->Count(); ++i)
			{
				iPoint pathPosition = App->map->MapToWorld(breadcrumbs->At(i)->x, breadcrumbs->At(i)->y);
				App->render->Blit(enemypath, pathPosition.x, pathPosition.y);
			}
		}
		if (breadcrumbs->Count() > 0)
		{
			chase = iPoint(breadcrumbs->At(0)->x, breadcrumbs->At(0)->y);
			if (chase.x < skeletonPosition.x)
			{
				position.x--;
				animation = &chasingLeft;
				left = true;
			}
			else if (chase.x > skeletonPosition.x)
			{
				position.x++;
				animation = &chasing;
				left = false;
			}
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