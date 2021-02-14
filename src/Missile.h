#pragma once
#include "Animation.h"
#include "Entity.h"

class Missile : public Entity
{
public:
	Missile(const sf::Vector2f& position, const sf::Vector2f& direction, sf::Texture* texture);
	void Update(const float& deltaTime, const sf::Vector2i& screenSize) override;
	~Missile();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

private:
	sf::Sprite m_body;
	sf::Vector2f m_direction;
	Animation* m_animationHandler;
	float m_fSpeed;
	std::string pattern;
};
