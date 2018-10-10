#include "j1App.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Scene.h"
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
	jumpPower = config.child("jumpPower").attribute("value").as_int();
	maxJumpHeight = config.child("maxjumpHeight").attribute("value").as_int();
	speedPlayer.x = config.child("speedplayer").attribute("x").as_float();
	speedPlayer.y = config.child("speedplayer").attribute("y").as_float();
	gravity = config.child("gravity").attribute("value").as_float();
	path = config.child("path").attribute("value").as_string();
	initialCamera = config.child("initialcamera").attribute("value").as_bool();
	left = config.child("left").attribute("value").as_bool();
	solidGround = config.child("solidground").attribute("value").as_bool();
	jumpAgain = config.child("jumpagain").attribute("value").as_bool();

	//Loading of the animations

	for (pugi::xml_node animations = config.child("animation"); animations; animations = animations.next_sibling("animation"))
	{

		p2SString types = animations.attribute("type").as_string();

		if (types == "idle")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				idle.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}

			idle.speed = animations.attribute("speed").as_float();
			idle.loop = animations.attribute("loop").as_bool();
		}
		if (types == "idleleft")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				idleLeft.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}

			idleLeft.speed = animations.attribute("speed").as_float();
			idleLeft.loop = animations.attribute("loop").as_bool();
		}

		if (types == "move")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				move.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			move.speed = animations.attribute("speed").as_float();
			move.loop = animations.attribute("loop").as_bool();
		}
		if (types == "moveleft")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				moveLeft.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			moveLeft.speed = animations.attribute("speed").as_float();
			moveLeft.loop = animations.attribute("loop").as_bool();
		}

		if (types == "jump")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				jump.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			jump.speed = animations.attribute("speed").as_float();
			jump.loop = animations.attribute("loop").as_bool();

		}
		if (types == "fall")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				fall.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			fall.speed = animations.attribute("speed").as_float();
			fall.loop = animations.attribute("loop").as_bool();

		}
		if (types == "fallLeft")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				fallLeft.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			fallLeft.speed = animations.attribute("speed").as_float();
			fallLeft.loop = animations.attribute("loop").as_bool();

		}
		if (types == "landing")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				landing.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			landing.speed = animations.attribute("speed").as_float();
			landing.loop = animations.attribute("loop").as_bool();

		}
		if (types == "landingLeft")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				landingLeft.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			landingLeft.speed = animations.attribute("speed").as_float();
			landingLeft.loop = animations.attribute("loop").as_bool();

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

	if (!left)
	{
		currentAnimation = &idle;
	}

	if (left)
	{
		currentAnimation = &idleLeft;
	}
	///////////////////

	//Logic of the jump movement of the player

	if (solidGround)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (jumpAgain)
			{
				speedPlayer.y = jumpPower;
				jumpAgain = false;
			}
			solidGround = false;
		}
		else
		{
			jumpAgain = true;
		}
	}
	if (!solidGround)
	{
		speedPlayer.y += gravity;
	}


	if (speedPlayer.y > maxJumpHeight)
	{
		speedPlayer.y = maxJumpHeight;
	}

	position.y += speedPlayer.y;

	/////////////////////////////


	if (initialCamera == true)
	{
		App->render->camera.x = -position.x + (App->win->width / 2);
		App->render->camera.y = position.y + (App->win->height / 2);
		initialCamera = false;
	}


	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speedPlayer.x;
		currentAnimation = &move;
		left = false;
	}


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speedPlayer.x;
		currentAnimation = &moveLeft;
		left = true;
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
	App->tex->UnLoad(playerTexture);
	playerTexture = nullptr;


	return true;
}



