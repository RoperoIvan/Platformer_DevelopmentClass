#ifndef __j1ENTITIES_H__
#define __j1ENTITIES_H__

#include "j1Module.h"
#include "j1Player.h"
#include "EntityGhost.h"
#include "EntitySkeleton.h"
#define MAX_ENTITY 100


enum EntitiesType
{
	PLAYER,
	GHOST,
	SKELETON,
	NOTYPE,
};

class Entity;


struct EntityInfo
{
	EntitiesType type = EntitiesType::NOTYPE;
	int x, y;
	float  entityLife;

};

class j1Entities : public j1Module
{

public:
	j1Entities();
	~j1Entities();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool CleanUp();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool AddEntity(EntitiesType type, int x, int y);
	void OnCollision(Collider* c1, Collider* c2);
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	EntityInfo queue[MAX_ENTITY];
	Entity* entities[MAX_ENTITY];
	EntitiesType type = EntitiesType::NOTYPE;

private:
	void SpawnEntity(const EntityInfo& info);

public:
	SDL_Texture* textures;
	p2SString path;
};


#endif // __j1ENTITIES_H__

