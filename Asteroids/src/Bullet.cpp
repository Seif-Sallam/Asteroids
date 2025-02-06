#include "Bullet.h"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f direction, sf::Texture* bulletTexture)
{
	m_body.setTexture(*bulletTexture);
	m_body.setPosition(position);
	m_body.setOrigin(16.0f, 32.0f);
	m_body.setScale(0.7f, 0.7f);
	float angle = atan2f(direction.y , direction.x);
	angle = angle * 180 / 3.14f;
	m_body.setRotation(angle + 90.0f);
	m_direction = direction;
	m_fSpeed = 800.0f;
	alive = true;
	m_animHandler = new Animation(sf::IntRect(0, 0, 32, 64), sf::Vector2i(32, 0), 16 , 0.1f);
	radius = 4.0f;
}

void Bullet::Update(const float& deltaTime, const sf::Vector2i& screenSize)
{
	if (alive) {
		m_animHandler->Update(deltaTime);
		m_body.setTextureRect(m_animHandler->GetCurrentStep());
		m_body.move(m_fSpeed * m_direction * deltaTime);
		position = m_body.getPosition();
		if (m_body.getPosition().x < 0.0f || m_body.getPosition().x > screenSize.x
			|| m_body.getPosition().y < 0.0f || m_body.getPosition().y > screenSize.y)
			alive = false;
	}
}


Bullet::~Bullet()
{
	delete m_animHandler;
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	if (alive) {
		target.draw(m_body, state);
	}
}
