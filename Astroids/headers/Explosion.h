#pragma once
#include "Animation.h"

enum ExplosionTypes {
	ASTROID,
	SHIP,
	MISSILE
};


class Explosion : public sf::Drawable
{
public:
	Explosion(const sf::Vector2f& position, int type, sf::Texture* texture);
	void Update(float deltaTime);
	bool alive;
	~Explosion();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default)const override;
private:
	sf::Sprite m_body;
	sf::Texture m_texture;
	Animation* m_animHandler;
};

