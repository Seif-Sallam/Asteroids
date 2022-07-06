#pragma once
#include "SFML/Graphics.hpp"

class Entity : public sf::Drawable 
{
public:
	Entity();
	virtual void Update(const float& deltaTime, const sf::Vector2i& screenSize) = 0;
	~Entity();
	bool alive;
	sf::Vector2f position;
	float radius;
protected:
	sf::CircleShape m_collisionCircle;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default)const override = 0;
};

