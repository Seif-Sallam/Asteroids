#include "Entity.h"

Entity::Entity()
{
	alive = true;
	radius = 1;
	position = sf::Vector2f();
}

Entity::~Entity()
{
}
