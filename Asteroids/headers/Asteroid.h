#pragma once
#include "Animation.h"
#include "Entity.h"

enum AsteroidSize {
	BIG,
	SMALL
};


class Asteroid : public Entity
{
public:
	Asteroid(const sf::Vector2f& position, int type, float angle, sf::Texture* AsteroidTexture);
	void Update(const float& deltaTime, const sf::Vector2i& screenSize) override;
	int type;
	~Asteroid();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

private:
	sf::Sprite m_body;
	Animation* m_animHandler;
	sf::Vector2f m_velocity;
	sf::Vector2f m_direction;
	float m_fMaxSpeed;
	float m_fAngle;
};

