#pragma once
#include "SFML/Graphics.hpp"
#include "Animation.h"
#include "Entity.h"
#include <string>
#include <iostream>
#include <vector>


class Bullet : public Entity
{
public:
	Bullet(sf::Vector2f position, sf::Vector2f direction, sf::Texture* bulletTexture, bool shadow = false);
	void Update(const float& deltaTime, const sf::Vector2i& screenSize) override;
	~Bullet();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

private:
	mutable sf::Sprite m_body;
	sf::Vector2f m_size;
	sf::Vector2f m_direction;
	int frame = 0;
	float m_fSpeed;
	Animation* m_animHandler;
	bool m_shadow;
};

