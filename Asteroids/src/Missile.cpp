#include "Missile.h"

Missile::Missile(const sf::Vector2f& position, const sf::Vector2f& direction, sf::Texture* texture)
{
	m_body.setTexture(*texture);
	m_body.setOrigin(2.0f, 4.0f);
	m_body.setPosition(position);
	m_body.setScale(1.5f, 1.5f);
	float angle = atan2f(direction.y, direction.x);
	m_direction = direction;
	angle = angle * 180.f / 3.14f;
	m_body.setRotation(angle + 90.0f);
	m_animationHandler = new Animation(sf::IntRect(16, 131, 8, 14), sf::Vector2i(8, 0),6 , 0.01f, false);
	m_fSpeed = 1000.0f;
	radius = 4.0f;
}

void Missile::Update(const float& deltaTime, const sf::Vector2i& screenSize)
{
	if (alive)
	{
		m_body.move(m_fSpeed * m_direction * deltaTime);
		position = m_body.getPosition();
		if (m_body.getPosition().x < 0.0f || m_body.getPosition().x > screenSize.x
			|| m_body.getPosition().y < 0.0f || m_body.getPosition().y > screenSize.y)
			alive = false;

		m_animationHandler->Update(deltaTime);
		m_body.setTextureRect(m_animationHandler->GetCurrentStep());
	}
}

Missile::~Missile()
{
	delete m_animationHandler;
}

void Missile::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	if(alive)
		target.draw(m_body, state);
}
