#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"

j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;
	matrix[COLLIDER_GROUND][COLLIDER_GROUND] = false;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER_UP] = true;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER_DOWN] = true;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER_LEFT] = true;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER_RIGHT] = true;

	matrix[COLLIDER_PLAYER_UP][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_RIGHT] = false;

	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_RIGHT] = false;

	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_RIGHT] = false;

	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_RIGHT] = false;
}

//destructor
j1Collision::~j1Collision() {}

// call before render is available
/*bool j1Collision::Awake(pugi::xml_node& config)
{
	LOG("Init Collision");
	bool ret = true;
	for (uint i = 0; 1 < MAX_COLLIDERS; i++)
		colliders[i] = nullptr;
	return ret;
}*/

bool j1Collision::Start() 
{
	return true;
}

//call every loop iteration
bool j1Collision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// Collision detection and callbacks 
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}
	return true;
}

bool j1Collision::Update(float dt)
{

	DebugDraw();

	return true;
}

// Called before render is available
void j1Collision::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_GROUND: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER_UP: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_PLAYER_DOWN: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_PLAYER_LEFT: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_PLAYER_RIGHT: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rectangle, type, callback);
			break;
		}
	}
	return ret;
}

void j1Collision::GodMode()
{
	godMode = !godMode;
	godMode_used = true;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if (enable) {
		if ((r.x < rect.x + rect.w) && (rect.x < r.x + r.w)
			&& (r.y < rect.y + rect.h) && (rect.y < r.y + r.h)) {
			return true;
		}
		return false;
	}
	else return false;
}