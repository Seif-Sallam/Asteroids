#include "Explosion.h"

Explosion::Explosion(const sf::Vector2f& position, int type, sf::Texture* texture)
{
	m_animHandler = nullptr;
	switch (type)
	{
	case SHIP: {
		m_animHandler = new Animation(sf::IntRect(0, 0, 192, 192), sf::Vector2i(192, 0), 64, 0.015f, false);
		break;
	}
	case MISSILE:
	case ASTROID: {

		m_animHandler = new Animation(sf::IntRect(0, 0, 256, 256), sf::Vector2i(256, 0), 47, 0.015f, false);
		break;
	}
	}
	m_body.setTexture(*texture);
	m_body.setOrigin(128, 128);
	m_body.setPosition(position);
	alive = true;
}

void Explosion::Update(float deltaTime)
{
	if (alive)
	{
		m_animHandler->Update(deltaTime);
		m_body.setTextureRect(m_animHandler->GetCurrentStep());

		if (m_animHandler->Finished())
		{
			alive = false;
		}
	}
}

Explosion::~Explosion()
{
	delete m_animHandler;
}

void Explosion::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(m_body, state);
}
