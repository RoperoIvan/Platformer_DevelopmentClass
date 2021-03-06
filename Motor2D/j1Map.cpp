#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collisions.h"
#include "Brofiler/Brofiler.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	seeCollisions = config.child("seeCollisions").attribute("value").as_bool();
	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("DrawMap", Profiler::Color::Green);
	if (map_loaded == false)
		return;

	p2List_item<TileSet*>* tilesets_items = data.tileSets.start;
	p2List_item<MapLayer*>* layers_items = data.mapLayers.start;
	while (tilesets_items != NULL)
	{
		while (layers_items != NULL)
		{
			for (int i = 0; i < layers_items->data->width; i++)
			{
				for (int j = 0; j < layers_items->data->height; j++)
				{
					if (layers_items->data->GetPosition(i, j) != 0)
					{
						SDL_Rect pattern1 = tilesets_items->data->GetTileRect(layers_items->data->GetPosition(i, j));
						SDL_Rect pattern2 = tilesets_items->next->data->GetTileRect(layers_items->data->GetPosition(i, j));
						iPoint pos = MapToWorld(i, j);
						GetGidPosition(pos.x, pos.y);

						if (layers_items->data->name == "Background1")
						{
							App->render->Blit(tilesets_items->next->data->texture, pos.x, pos.y, &pattern2, 0.6f);
						}
						if (layers_items->data->name == "Background2")
						{
							App->render->Blit(tilesets_items->next->data->texture, pos.x, pos.y, &pattern2, 0.7f);
						}
						if (layers_items->data->name == "Background3")
						{
							App->render->Blit(tilesets_items->next->data->texture, pos.x, pos.y, &pattern2, 0.8f);
						}
						if (layers_items->data->name == "Background4")
						{
							App->render->Blit(tilesets_items->next->data->texture, pos.x, pos.y, &pattern2, 0.9f);
						}
						if (layers_items->data->name == "Background5")
						{
							App->render->Blit(tilesets_items->next->data->texture, pos.x, pos.y, &pattern2, 1.0f);
						}

						if (layers_items->data->name == "Background6")
						{
							App->render->Blit(tilesets_items->data->texture, pos.x, pos.y, &pattern1, 1.0f);
						}

						if (layers_items->data->name == "Movement")
						{
							App->render->Blit(tilesets_items->data->texture, pos.x, pos.y, &pattern1, 1.0f);
						}
						/*if (layers_items->data->name == "Collision" && seeCollisions)
						{
						App->render->Blit(tilesets_items->data->texture, pos.x, pos.y, &pattern1, 1.0f);
						}*/
					}

				}
			}
			layers_items = layers_items->next;
		}
		tilesets_items = tilesets_items->next;
	}
}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2) - 1;
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tileSets.start;

	for (item; item != NULL; item = item->next)
	{
		if (item->next == nullptr || item->next->data->firstgid > id)
			return item->data;
	}

	return data.tileSets.start->data;
}

int Properties::Get(const char* value, int default_value) const
{
	p2List_item<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tileSets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tileSets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.mapLayers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.mapLayers.clear();

	// Clean up the pugui tree

	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}
	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tileSets.add(set);
	}

	//Load Objects (Colliders) info
	pugi::xml_node object_iterator;
	for (object_iterator = map_file.child("map").child("objectgroup"); object_iterator && ret; object_iterator = object_iterator.next_sibling("objectgroup"))
	{

		if (ret == true)
		{
			LoadObject(object_iterator);
		}

	}

	//Load Attributes of the player info
	pugi::xml_node attributes;
	for (attributes = map_file.child("map").child("properties"); attributes && ret; attributes = attributes.next_sibling("properties"))
	{

		if (ret == true)
		{
			ret = LoadAttributes(attributes, att);
		}
	}
	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, set);
		}
		data.mapLayers.add(set);
	}



	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tileSets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// TODO 4: Add info here about your loaded layers
		// Adapt this vcode with your own variables

		p2List_item<MapLayer*>* item_layer = data.mapLayers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

// TODO 3: Create the definition for a function that loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	
	LoadProperties(node, layer->properties);
	layer->data = new uint[layer->width*layer->height];
	memset(layer->data, 0, layer->width * layer->height * sizeof(uint));
	int i = 0;
	for (pugi::xml_node& gids = node.child("data").child("tile"); gids; gids = gids.next_sibling("tile"))
	{
		layer->data[i] = gids.attribute("gid").as_uint();
		i++;
	}
	return true;
}

bool j1Map::LoadObject(pugi::xml_node& node)
{
	bool ret = true;

	pugi::xml_node col_object = node.child("object");
	if (node == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
	}
	else
	{
		int i = 0;
		for (col_object; col_object; col_object = col_object.next_sibling("object"))
		{
			if (strcmp(node.attribute("name").as_string(), "Colliders") == 0)
				App->collision->AddCollider({ col_object.attribute("x").as_int(0),col_object.attribute("y").as_int(0),col_object.attribute("width").as_int(0),col_object.attribute("height").as_int(0) }, COLLIDER_WALL);
			else if(strcmp(node.attribute("name").as_string(), "Colliders2") == 0)
				App->collision->AddCollider({ col_object.attribute("x").as_int(0),col_object.attribute("y").as_int(0),col_object.attribute("width").as_int(0),col_object.attribute("height").as_int(0) }, COLLIDER_PLATFORM);
			else if(strcmp(node.attribute("name").as_string(), "WinCon") == 0)
				App->collision->AddCollider({ col_object.attribute("x").as_int(0),col_object.attribute("y").as_int(0),col_object.attribute("width").as_int(0),col_object.attribute("height").as_int(0) }, COLLIDER_WINCONDITION);
			else if (strcmp(node.attribute("name").as_string(), "DeathCon") == 0)
				App->collision->AddCollider({ col_object.attribute("x").as_int(0),col_object.attribute("y").as_int(0),col_object.attribute("width").as_int(0),col_object.attribute("height").as_int(0) }, COLLIDER_DEATHCONDITION);
			i++;
		}
	}

	return ret;
}

bool j1Map::LoadAttributes(pugi::xml_node& node, Attributes* att)
{
	for (pugi::xml_node& gids = node.child("property"); gids; gids = gids.next_sibling("property"))
	{
		att->name = gids.attribute("name").as_string();

		if (att->name == "gravity")
			att->gravity = gids.attribute("value").as_float();

		if (att->name == "jumpPower")
			att->jumpPower = gids.attribute("value").as_float();

		if (att->name == "maxJumpHeight")
			att->maxJumpHeight = gids.attribute("value").as_float();
	}
	return true;
}

bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();
			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();
			properties.list.add(p);
		}
	}

	return ret;
}

uint j1Map::GetGidPosition(int x, int y)
{
	iPoint ret;
	ret.x = x / data.tile_width;
	ret.y = y / data.tile_height;

	return data.mapLayers.start->data->width*ret.y + ret.x;
}


uint MapLayer::GetPosition(int x, int y)const
{

	return data[width*y + x];
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	item = data.mapLayers.start;

	for (item = data.mapLayers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->GetPosition(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
					map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}