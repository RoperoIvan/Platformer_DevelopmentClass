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

}

void Entity::OnCollision(Collider* collider)
{

}