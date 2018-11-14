#include "j1App.h"
#include "EntityPlayer.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Window.h"


EntityPlayer::EntityPlayer(int x, int y) : Entity(x, y)
{
	/*name.create("player");*/
	
}

EntityPlayer::~EntityPlayer()
{
}


bool EntityPlayer::Awake(pugi::xml_node& config)
{

	//Importing the values of the variables from the player in the XML config file

	///// string
	path = config.child("path").attribute("value").as_string();

	///// int
	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();

	///// float
	speedPlayer.x = config.child("speedplayer").attribute("x").as_float();
	speedPlayer.y = config.child("speedplayer").attribute("y").as_float();


	///// bool
	left = config.child("left").attribute("value").as_bool();
	jumping = config.child("solidground").attribute("value").as_bool();
	godMode = config.child("godMode").attribute("value").as_bool();

	//Loading of the animations

	//Idle animations
	idle.PushBack({ 15,7, 19,30 });
	idle.PushBack({ 66,6, 19,30 });
	idle.PushBack({ 116,6, 19,30 });
	idle.PushBack({ 163,7, 22,30 });
	idle.speed = 0.1f;
	idle.loop = true;

	idleLeft.PushBack({ 669,6, 20,30 });
	idleLeft.PushBack({ 619,6, 20,30 });
	idleLeft.PushBack({ 569,6, 20,30 });
	idleLeft.PushBack({ 519,6, 20,30 });
	idleLeft.speed = 0.1f;
	idleLeft.loop = true;

	//Movement animations
	move.PushBack({ 67,45, 20,28 });
	move.PushBack({ 116,46, 20,27 });
	move.PushBack({ 166,48, 20,26 });
	move.PushBack({ 217,45, 23,29 });
	move.PushBack({ 266,46, 20,27 });
	move.PushBack({ 316,48, 20,25 });
	move.speed = 0.2f;
	move.loop = true;

	moveLeft.PushBack({ 615,45, 20,28 });
	moveLeft.PushBack({ 566,46, 20,27 });
	moveLeft.PushBack({ 516,48, 20,26 });
	moveLeft.PushBack({ 465,45, 23,29 });
	moveLeft.PushBack({ 416,46, 20,27 });
	moveLeft.PushBack({ 366,48, 20,25 });
	moveLeft.speed = 0.2f;
	moveLeft.loop = true;

	//Jump animations
	jump.PushBack({ 15,86, 20,24 });
	jump.PushBack({ 65,88, 20,22 });
	jump.PushBack({ 117,81, 19,27 });
	jump.PushBack({ 164,79, 21,23 });
	jump.PushBack({ 218,82, 15,20 });
	jump.PushBack({ 264,84, 23,17 });
	jump.PushBack({ 320,84, 18,20 });
	jump.PushBack({ 14,124, 23,17 });
	jump.PushBack({ 68,112, 17,31 });
	jump.speed = 0.2f;
	jump.loop = true;

	jumpLeft.PushBack({ 665,87, 20,24 });
	jumpLeft.PushBack({ 615,89, 20,22 });
	jumpLeft.PushBack({ 564,82, 19,27 });
	jumpLeft.PushBack({ 515,80, 21,23 });
	jumpLeft.PushBack({ 467,83, 15,20 });
	jumpLeft.PushBack({ 413,85, 23,17 });
	jumpLeft.PushBack({ 362,85, 18,20 });
	jumpLeft.PushBack({ 663,125, 23,17 });
	jumpLeft.PushBack({ 615,113, 17,31 });
	jumpLeft.speed = 0.2f;
	jumpLeft.loop = true;

	//Falling animations
	fall.PushBack({ 65,88, 20,22 });
	fall.PushBack({ 118,113, 17,30 });
	fall.speed = 0.1f;
	fall.loop = false;

	fallLeft.PushBack({ 615,89, 20,22 });
	fallLeft.PushBack({ 565,113, 17,30 });
	fallLeft.speed = 0.1f;
	fallLeft.loop = false;

	landing.PushBack({ 15,86, 21,24 });
	landing.speed = 0.1f;
	landing.loop = false;

	landingLeft.PushBack({ 665,87, 20,24 });
	landingLeft.speed = 0.1f;
	landingLeft.loop = false;

	currentAnimation = &idle;



	return true;
}

bool EntityPlayer::Start()
{
	playerTexture = App->tex->Load(path.GetString());
	feetCollider = App->collision->AddCollider({ position.x, position.y + 30,22,2 }, COLLIDER_PLAYER_DOWN);
	leftCollider = App->collision->AddCollider({ position.x - 10, position.y + 5,2,16 }, COLLIDER_PLAYER_LEFT);
	rightCollider = App->collision->AddCollider({ position.x + 10, position.y + 5,2,16 }, COLLIDER_PLAYER_RIGHT);
	return true;
}


bool EntityPlayer::PreUpdate()
{
	return true;
}


void EntityPlayer::Update(float dt)
{
	LOG("%i", solidGround);
	//Control of the orientation of the player animations
	currentAnimation = &fall;


	if (!left && solidGround)
	{
		currentAnimation = &idle;
	}

	if (left && solidGround)
	{
		currentAnimation = &idleLeft;
	}
	///////////////////

	//Player death
	/*if (App->map->data.mapLayers.end->data->data[feetCollider] == 53)
	{
	App->scene->LevelChange();
	}*/


	if (solidGround)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			speedPlayer.y = App->map->att->jumpPower;
			solidGround = false;
		}

	}
	if (speedPlayer.y < App->map->att->maxJumpHeight)
	{
		speedPlayer.y += App->map->att->gravity;
	}
	position.y += speedPlayer.y;


	//Camera Movement

	App->render->camera.x = (-position.x * 3) + (App->win->width / 2);
	App->render->camera.y = (-position.y * 3) + (App->win->height / 2);


	// Right and left player's movement logic

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && solidGround)
	{
		position.x += speedPlayer.x;
		if (solidGround)
		{
			currentAnimation = &move;
		}
		left = false;
	}


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && solidGround)
	{
		position.x -= speedPlayer.x;
		if (solidGround)
		{
			currentAnimation = &moveLeft;
		}
		left = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !solidGround)
	{
		position.x += speedPlayer.x;
		currentAnimation = &fall;
		left = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !solidGround)
	{
		position.x -= speedPlayer.x;
		currentAnimation = &fallLeft;
		left = true;
	}
	if (!solidGround && !left)
	{
		currentAnimation = &fall;
	}
	if (!solidGround && left)
	{
		currentAnimation = &fallLeft;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (left)
		{
			currentAnimation = &idleLeft;
		}
		if (!left)
		{
			currentAnimation = &idle;
		}

	}
	//Tracing of the player's position by its colliders

	feetCollider->SetPos(position.x, position.y + 30);
	leftCollider->SetPos(position.x, position.y + 5);
	rightCollider->SetPos(position.x + 20, position.y + 5);


	//Drawing the animations
	App->render->Blit(playerTexture, position.x, position.y, &currentAnimation->GetCurrentFrame());




}

bool EntityPlayer::PostUpdate()
{
	return true;
}

bool EntityPlayer::CleanUp()
{
	LOG("Freeing player textures....");
	App->tex->UnLoad(playerTexture);
	playerTexture = nullptr;
	return true;
}


iPoint EntityPlayer::GetPosition()
{
	return position;
}

iPoint EntityPlayer::SetPosition(iPoint playerPos)
{
	position.x = playerPos.x;
	position.y = playerPos.y;
	return position;
}

bool EntityPlayer::Save(pugi::xml_node& data)const
{

	data.append_child("position").append_attribute("x") = position.x;
	data.child("position").append_attribute("y") = position.y;

	return true;
}

bool EntityPlayer::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	LOG("%f", position.x);
	LOG("%f", position.y);

	return true;
}

void EntityPlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER_LEFT && c2->type == COLLIDER_WALL)
	{
		position.x += speedPlayer.x;
	}
	if (c1->type == COLLIDER_PLAYER_RIGHT && c2->type == COLLIDER_WALL)
	{
		position.x -= speedPlayer.x;
	}

	if (c1->type == COLLIDER_PLAYER_DOWN && c2->type == COLLIDER_WALL)
	{
		speedPlayer.y = 0;
		position.y -= speedPlayer.y;
		solidGround = true;
	}
	else
	{
		solidGround = false;
	}

	if (c1->type == COLLIDER_PLAYER_DOWN && c2->type == COLLIDER_PLATFORM)
	{
		speedPlayer.y = 0;
		position.y -= speedPlayer.y;
		solidGround = true;
	}
	if (c1->type == COLLIDER_PLAYER_DOWN && c2->type == COLLIDER_WALL && c1->type == COLLIDER_PLAYER_LEFT && c2->type == COLLIDER_WALL || c1->type == COLLIDER_PLAYER_DOWN && c2->type == COLLIDER_WALL && c1->type == COLLIDER_PLAYER_RIGHT && c2->type == COLLIDER_WALL)
	{
		currentAnimation = &idle;
	}
}