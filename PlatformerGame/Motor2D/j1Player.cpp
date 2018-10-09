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
	position.x = 0;
	position.y = 0;
	speed.x = 1;
	speed.y = 0;

	gravity = config.child("gravity").attribute("value").as_int();
	path = config.child("path").attribute("value").as_string();

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
				idleleft.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}

			idleleft.speed = animations.attribute("speed").as_float();
			idleleft.loop = animations.attribute("loop").as_bool();
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
				moveleft.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			moveleft.speed = animations.attribute("speed").as_float();
			moveleft.loop = animations.attribute("loop").as_bool();
		}

		/*if (types == "jump")
		{
		frame.x = animations.attribute("x").as_int();
		frame.y = animations.attribute("y").as_int();
		frame.w = animations.attribute("w").as_int();
		frame.h = animations.attribute("h").as_int();
		jump.PushBack(frame);
		jump.speed = 0.5f;
		jump.loop = true;
		}*/
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
	if (!left)
	{
		currentAnimation = &idle;
	}

	if (left)
	{
		currentAnimation = &idleleft;
	}

	if (initialCamera == true)
	{
		App->render->camera.x = -position.x + (App->win->width / 2);
		App->render->camera.y = position.y + (App->win->height / 2);
		initialCamera = false;
	}


	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed.x;
		currentAnimation = &move;
		left = false;
	}
	/*if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
	position.x += speed.x;
	walking = false;

	}*/

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed.x;
		currentAnimation = &moveleft;
		left = true;
	}



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



