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
	solidGround = config.child("solidground").attribute("value").as_bool();
	jumpAgain = config.child("jumpagain").attribute("value").as_bool();
	sizePlayer.x = config.child("idle").attribute("w").as_int();
	sizePlayer.y = config.child("idle").attribute("h").as_int();
	//hasDashed = config.child("hasDashed").attribute("value").as_bool();
	//dashAgain = config.child("dashagain").attribute("value").as_bool();
	//dashLength = config.child("dashLength").attribute("value").as_float();
	//dashPower = config.child("dashPower").attribute("value").as_float();


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
		if (types == "jumpLeft")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				jumpLeft.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			jumpLeft.speed = animations.attribute("speed").as_float();
			jumpLeft.loop = animations.attribute("loop").as_bool();

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
		if (types == "death")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				death.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			death.speed = animations.attribute("speed").as_float();
			death.loop = animations.attribute("loop").as_bool();

		}
		if (types == "deathLeft")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				deathLeft.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			deathLeft.speed = animations.attribute("speed").as_float();
			deathLeft.loop = animations.attribute("loop").as_bool();

		}
		if (types == "dash")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				dash.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			dash.speed = animations.attribute("speed").as_float();
			dash.loop = animations.attribute("loop").as_bool();

		}
		if (types == "dashLeft")
		{
			for (pugi::xml_node frames = animations.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				dashLeft.PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("w").as_int(), frames.attribute("h").as_int() });
			}
			dashLeft.speed = animations.attribute("speed").as_float();
			dashLeft.loop = animations.attribute("loop").as_bool();

		}
	}
	currentAnimation = &idle;

	return true;
}

bool j1Player::Start()
{
	playerTexture = App->tex->Load(path.GetString());
	collider_player_down = App->collision->AddCollider({ position.x + 2, position.y + sizePlayer.y, sizePlayer.x - 2, 2 }, COLLIDER_PLAYER_DOWN, this);
	collider_player_up = App->collision->AddCollider({ position.x + 2,position.y - 3,sizePlayer.x - 2, 2 }, COLLIDER_PLAYER_UP, this);
	collider_player_left = App->collision->AddCollider({ position.x,position.y - 3, 2 , sizePlayer.y }, COLLIDER_PLAYER_LEFT, this);
	collider_player_right = App->collision->AddCollider({ position.x + sizePlayer.x,position.y - 3, 2 , sizePlayer.y }, COLLIDER_PLAYER_RIGHT, this);


	return true;
}


bool j1Player::PreUpdate()
{
	return true;
}


bool j1Player::Update(float dt)
{
	//Control of the orientation of the player animations

	if (!left && solidGround)
	{
		currentAnimation = &idle;
	}

	if (left && solidGround)
	{
		currentAnimation = &idleLeft;
	}
	///////////////////

	//Logic of the jump movement of the player

	if (solidGround)
	{
		speedPlayer.y = 0;
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (jumpAgain)
			{
				speedPlayer.y = jumpPower;
				jumpAgain = false;
			}
			currentAnimation = &jump;
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
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			currentAnimation = &jump;
			if (jump.Finished())
			{
				currentAnimation = &fall;
			}
			
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			currentAnimation = &jumpLeft;
			if (jumpLeft.Finished())
			{
				currentAnimation = &fallLeft;
			}
		}
	}
	if (speedPlayer.y > maxJumpHeight)
	{
		speedPlayer.y = maxJumpHeight;
	}

	position.y += speedPlayer.y;

	
	//Camera Movement

	App->render->camera.x = (-position.x * 3) + (App->win->width / 2);
	App->render->camera.y = (-position.y * 3) + (App->win->height / 2);
	

	// Right and left player's movement logic

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speedPlayer.x;
		if (solidGround)
		{
			currentAnimation = &move;
		}
		left = false;
	}


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speedPlayer.x;
		if (solidGround)
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

	//if (hasDashed)
	//{
	//	speedPlayer.x = 0;
	//	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	//	{
	//		if (dashAgain)
	//		{
	//			speedPlayer.x = dashPower;
	//			dashAgain = true;
	//		}
	//		currentAnimation = &dash;
	//		hasDashed = true;
	//	}
	//	else
	//	{
	//		dashAgain = true;
	//	}
	//}

	//if (!hasDashed)
	//{
	//	//speedPlayer.x;
	//	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	//	{
	//		currentAnimation = &dash;
	//		if (dash.Finished())
	//		{
	//			currentAnimation = &idle;
	//		}

	//	}
	//	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	//	{
	//		currentAnimation = &dashLeft;
	//		if (dashLeft.Finished())
	//		{
	//			currentAnimation = &idleLeft;
	//		}
	//	}
	//}
	//if (speedPlayer.x > dashLength)
	//{
	//	speedPlayer.x = dashLength;
	//}

	//position.x += speedPlayer.x;

	//check if player is in a platform
	Collider* c2;

	for (uint k = 0; k < MAX_COLLIDERS; ++k)
	{
		// skip empty colliders
		if (App->collision->colliders[k] == nullptr)
			continue;

		c2 = App->collision->colliders[k];

		if (collider_player_down->CheckCollision(c2->rect) == false)
		{
			solidGround = false;
		}
	}

	//colliders player
	collider_player_up->SetPos(position.x + 2, position.y - 3);
	collider_player_down->SetPos(position.x + 2, position.y + sizePlayer.y);
	collider_player_left->SetPos(position.x, position.y);
	collider_player_right->SetPos(position.x + sizePlayer.x, position.y);

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

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER_LEFT && c2->type == COLLIDER_GROUND)
	{
		position.x += speedPlayer.x;
	}
	else if (c1->type == COLLIDER_PLAYER_RIGHT && c2->type == COLLIDER_GROUND)
	{
		position.x -= speedPlayer.x;
	}
	else if (c1->type == COLLIDER_PLAYER_DOWN && c2->type == COLLIDER_GROUND)
	{
		solidGround = true;
	}
	else if (c1->type == COLLIDER_PLAYER_UP && c2->type == COLLIDER_GROUND)
	{
		/*top_jump = true;*/
	}
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

