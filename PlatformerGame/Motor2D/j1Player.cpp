#include "j1App.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Window.h"


j1Player::j1Player()
{
	name.create("player");
}

j1Player::~j1Player()
{
}


bool j1Player::Awake(pugi::xml_node& config)
{

	//Importing the values of the variables from the player in the XML config file

	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();
	jumpPower = config.child("jumpPower").attribute("value").as_float();
	maxJumpHeight = config.child("maxjumpHeight").attribute("value").as_float();
	speedPlayer.x = config.child("speedplayer").attribute("x").as_float();
	speedPlayer.y = config.child("speedplayer").attribute("y").as_float();
	dashSpeed.x = config.child("dashSpeed").attribute("x").as_float();
	dashSpeed.y = config.child("dashSpeed").attribute("y").as_float();
	gravity = config.child("gravity").attribute("value").as_float();
	path = config.child("path").attribute("value").as_string();
	initialCamera = config.child("initialcamera").attribute("value").as_bool();
	left = config.child("left").attribute("value").as_bool();
	jumping = config.child("solidground").attribute("value").as_bool();
	jumpAgain = config.child("jumpagain").attribute("value").as_bool();
	sizePlayer.x = config.child("idle").attribute("w").as_int();
	sizePlayer.y = config.child("idle").attribute("h").as_int();
	godMode = config.child("godMode").attribute("value").as_bool();
	//hasDashed = config.child("hasDashed").attribute("value").as_bool();
	//dashAgain = config.child("dashagain").attribute("value").as_bool();
	//dashLength = config.child("dashLength").attribute("value").as_float();
	//dashPower = config.child("dashPower").attribute("value").as_float();


	//Loading of the animations
	dash.PushBack({ 219,269, 20,26});
	dash.PushBack({270, 269, 20,26});
	dash.PushBack({302,272,48,23});
	dash.PushBack({3,313,31,19});
	dash.PushBack({50,312,34,20});
	dash.PushBack({100, 312,34,20});
	for (pugi::xml_node animations = config.child("animation"); animations; animations = animations.next_sibling("animation"))
	{

		p2SString types = animations.attribute("type").as_string();

		if (types == "idle")
		{
			ChargingAnimations(animations, &idle);
		}
		if (types == "idleleft")
		{
			ChargingAnimations(animations, &idleLeft);
		}

		if (types == "move")
		{
			ChargingAnimations(animations, &move);
		}
		if (types == "moveleft")
		{
			ChargingAnimations(animations, &moveLeft);
		}

		if (types == "jump")
		{
			ChargingAnimations(animations, &jump);

		}
		if (types == "jumpLeft")
		{
			ChargingAnimations(animations, &jumpLeft);

		}
		if (types == "fall")
		{
			ChargingAnimations(animations, &fall);

		}
		if (types == "fallLeft")
		{
			ChargingAnimations(animations, &fallLeft);

		}
		if (types == "landing")
		{
			ChargingAnimations(animations, &landing);

		}
		if (types == "landingLeft")
		{
			ChargingAnimations(animations, &landingLeft);

		}
		if (types == "death")
		{
			ChargingAnimations(animations, &death);
		}
		if (types == "deathLeft")
		{
			ChargingAnimations(animations, &deathLeft);

		}
		/*if (types == "dash")
		{
			ChargingAnimations(animations, &dash);

		}*/
		if (types == "dashLeft")
		{
			ChargingAnimations(animations, &dashLeft);

		}
		
	}
	currentAnimation = &idle;

	return true;
}

bool j1Player::Start()
{
	playerTexture = App->tex->Load(path.GetString());
	return true;
}


bool j1Player::PreUpdate()
{
	return true;
}


bool j1Player::Update(float dt)
{
	//Control of the orientation of the player animations

	if (!left && !jumping)
	{
		currentAnimation = &idle;
	}

	if (left && !jumping)
	{
		currentAnimation = &idleLeft;
	}
	///////////////////

	gid = App->map->GetGidPosition(position.x, position.y+30);
	App->render->DrawQuad({ position.x, position.y + 30,16,16 }, 0, 255, 0, 255);
	gid2 = App->map->GetGidPosition(position.x - 10, position.y);
	App->render->DrawQuad({ position.x - 10, position.y,16,16 }, 0, 255, 0, 255);
	gid3 = App->map->GetGidPosition(position.x + 10, position.y);
	App->render->DrawQuad({ position.x + 10, position.y,16,16 }, 0, 255, 0, 255);
	if (App->map->data.mapLayers.end->data->data[gid] != 50 && !godMode)
	{
		position.y += 3;
		inAir = true;
	}
	else
	{
		inAir = false;
	}
	if (App->map->data.mapLayers.end->data->data[gid2] == 51)
	{
		cantgoleft = true;
	}
	else
	{
		cantgoleft = false;
	}
	if (App->map->data.mapLayers.end->data->data[gid3] == 54)
	{
		cantgoright = true;
	}
	else
	{
		cantgoright = false;
	}
	//Logic of the jump movement of the player

	if (!jumping)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
				jumping = true;
				currentAnimation = &jump;
		}
		
	}
	
	//Player death
	if (App->map->data.mapLayers.end->data->data[gid + 1] == 53)
	{

		App->scene->LevelChange();
	}

	//JUMP
	if (maxJumpHeight == 0.0f)
	{
		jumpPower = -1 * position.y;
	}
	if (jumping)
	{
		maxJumpHeight += 0.1f;

		position.y -= gravity;

		if (position.y > jumpPower && maxJumpHeight >= 1.0f)
		{
			position.y += 10;
			maxJumpHeight = 0.0f;
			jumping = false;
			inAir = true;
		}

	}
	
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && inAir && !cantgoright)
		{
			position.x += speedPlayer.x;
			currentAnimation = &fall;
		}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && inAir && !cantgoleft)
		{
			position.x -= speedPlayer.x;
			currentAnimation = &fallLeft;
			left = true;
		}
		if (inAir && !left)
		{
			currentAnimation = &fall;
		}
		if (inAir && left)
		{
			currentAnimation = &fallLeft;
		}
	//Camera Movement

	App->render->camera.x = (-position.x * 3) + (App->win->width / 2);
	App->render->camera.y = (-position.y * 3) + (App->win->height / 2);
	

	// Right and left player's movement logic

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !inAir && !cantgoright)
	{
		position.x += speedPlayer.x;
		if (!jumping)
		{
			currentAnimation = &move;
		}
		left = false;
	}


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !inAir && !cantgoleft)
	{
		position.x -= speedPlayer.x;
		if (!jumping)
		{
			currentAnimation = &moveLeft;
		}
		left = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
			currentAnimation = &idle;
	}

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		currentAnimation = &death;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		//position.x += (dashSpeed.x + speedPlayer.x);
		position.x += speedPlayer.x;
		currentAnimation = &dash;
		left = false;
	}

	//Drawing the animations
	App->render->Blit(playerTexture, position.x, position.y, &currentAnimation->GetCurrentFrame());

	return true;
}

bool j1Player::PostUpdate()
{
	return true;
}

bool j1Player::CleanUp()
{
	LOG("Freeing player textures....");
	App->tex->UnLoad(playerTexture);
	playerTexture = nullptr;
	return true;
}


iPoint j1Player::GetPosition()
{
	return position;
}

iPoint j1Player::SetPosition(iPoint playerPos)
{
	position.x = playerPos.x;
	position.y = playerPos.y;
	return position;
}


void j1Player::ChargingAnimations(pugi::xml_node& animation, Animation* anime)
{
	for (pugi::xml_node frames = animation.child("frame"); frames; frames = frames.next_sibling("frame"))
	{
		anime->PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
	}
	anime->speed = animation.attribute("speed").as_float();
	anime->loop = animation.attribute("loop").as_bool();

}


bool j1Player::Save(pugi::xml_node& data)const
{
	
	data.append_child("position").append_attribute("x") = position.x;
	data.child("position").append_attribute("y") = position.y;

	return true;
}

bool j1Player::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	LOG("%f", position.x);
	LOG("%f", position.y);

	return true;
}

