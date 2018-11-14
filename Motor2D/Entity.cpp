#include "j1App.h"
#include "Entity.h"
#include "j1Collisions.h"
#include "j1Entities.h"
#include "j1Render.h"
#include "j1Scene.h"


Entity::Entity(int x, int y) : position(x, y)
{
}

Entity::~Entity()
{

}

const Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::Draw(SDL_Texture* sprites)
{
	SDL_Rect r = animation->GetCurrentFrame();
	if (this->entity_type == PLAYER) {
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
		collider->SetPos(position.x, position.y);
	}
}

void Entity::OnCollision(Collider* collider)
{

}