#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "p2Log.h"

j1Collisions::j1Collisions()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_UP] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_DOWN] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_LEFT] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_RIGHT] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLATFORM] = false;
	

	matrix[COLLIDER_PLAYER_UP][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLATFORM] = false;

	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLATFORM] = true;

	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLATFORM] = false;

	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLATFORM] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;

	matrix[COLLIDER_PLATFORM][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLAYER_DOWN] = true;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLATFORM][COLLIDER_PLAYER] = false;
}
// Destructor
j1Collisions::~j1Collisions()
{}

bool j1Collisions::PreUpdate()
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

	// Calculate collisions
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

// Called before render is available
bool j1Collisions::Update(float dt)
{

	DebugDraw();

	return true;
}

void j1Collisions::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
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
		case COLLIDER_WALL: // blue
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
		case COLLIDER_PLAYER:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_PLATFORM:
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Collisions::CleanUp()
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

Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	// TODO 0: Return true if there is an overlap
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}