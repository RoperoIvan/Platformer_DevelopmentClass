#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "EntityGhost.h"
#include "j1Collisions.h"
#include "j1Player.h"
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
	collider = App->collision->AddCollider({ 0,0,26,21 },COLLIDER_TYPE::COLLIDER_ENEMY,(j1Module*)App->entities);
	animation = &idle;
}
void EntityGhost::Update(float dt)
{

	ghostPos = App->map->WorldToMap(position.x, position.y);
	playerPos = App->player->playerPos;


	if (Vision() == true)
	{
		/*position.x--;*/
		if (App->pathfinding->CreatePath(ghostPos, playerPos) != -1)
		{
			const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
			if (path->Count() > 0)
			{
				chase = iPoint(path->At(0)->x, path->At(0)->y);
				if (chase.x < ghostPos.x)
				{
					position.x--;
				}
				else if (chase.x > ghostPos.x)
				{
					position.x++;
				}
				if (chase.y < ghostPos.y)
				{
					position.y--;
				}
				if (chase.y > ghostPos.y)
				{
					position.y++;
				}
			}
		}
	}
}

void EntityGhost::OnCollision(Collider* coll)
{
	
}

bool EntityGhost::Vision()
{
	return (App->player->GetPosition().x > position.x - 100 
		&& App->player->GetPosition().x < position.x +100 
		&& App->player->GetPosition().y < position.y + 100 
		&& App->player->GetPosition().y > position.y - 100);
}