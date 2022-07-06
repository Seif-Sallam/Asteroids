#pragma once
#include "Animation.h"
#include "Entity.h"

enum AstroidSize {
	BIG,
	SMALL
};


class Astroid : public Entity
{
public:
	Astroid(const sf::Vector2f& position, int type, float angle, sf::Texture* astroidTexture);
	void Update(const float& deltaTime, const sf::Vector2i& screenSize) override;	
	int type;
	~Astroid();
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

