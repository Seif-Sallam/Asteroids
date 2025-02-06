#include "Asteroid.h"

Asteroid::Asteroid(const sf::Vector2f& position, int type, float angle, sf::Texture* AsteroidTexture)
{
	this->type = type;

	if (type == BIG)
	{
		radius = 20.0f;
		m_fMaxSpeed = 150.0f;
	}
	else if (type == SMALL)
	{
		radius = 10.0f;
		m_fMaxSpeed = 200.0f;
	}
	m_animHandler = new Animation(sf::IntRect(0, 0, 64, 64), sf::Vector2i(64, 0), 15, 0.1f, true);
	m_body.setTexture(*AsteroidTexture);
	m_body.setOrigin(32.0f, 32.0f);
	m_body.setPosition(position);
	this->position = position;
	m_fAngle = angle;
	auto toRadians = [](float degree) { return degree * 3.14f / 180.0f; };
	m_direction = { cos(toRadians(m_fAngle)), sin(toRadians(m_fAngle)) };
	float dis = static_cast<float>(sqrt(static_cast<double>(pow(m_direction.x, 2) + pow(m_direction.y, 2))));
	m_direction /= dis;
}

void Asteroid::Update(const float& deltaTime, const sf::Vector2i& screenSize)
{
	if (alive) {
		m_animHandler->Update(deltaTime);
		m_body.setTextureRect(m_animHandler->GetCurrentStep());

		auto toRadians = [](float degree) { return degree * 3.14f / 180.0f; };
		m_direction = { cos(toRadians(m_fAngle)), sin(toRadians(m_fAngle)) };
		float dis = static_cast<float>(sqrt(static_cast<double>(pow(m_direction.x, 2) + pow(m_direction.y, 2))));
		m_direction /= dis;
		m_velocity.x += m_direction.x;
		m_velocity.y += m_direction.y;

		float speed = static_cast<float>(sqrt(static_cast<double>(pow(m_velocity.x, 2) + pow(m_velocity.y, 2))));
		if (speed > m_fMaxSpeed)
			m_velocity *= m_fMaxSpeed / speed;

		m_body.move(m_velocity * deltaTime);

		if (position.x > screenSize.x) {
			m_body.setPosition(0.0f, position.y);
			m_fAngle += rand() % 90;
		}
		else if (position.y > screenSize.y) {
			m_body.setPosition(position.x, 0.0f);
			m_fAngle += rand() % 90;
		}
		else if (position.x < 0.f) {
			m_body.setPosition(static_cast<float>(screenSize.x), position.y);
			m_fAngle += rand() % 90;
		}
		else if (position.y < 0.f) {
			m_body.setPosition(position.x, static_cast<float>(screenSize.y));
			m_fAngle += rand() % 90;
		}
		position = m_body.getPosition();
	}
}

Asteroid::~Asteroid()
{
	delete m_animHandler;
}

void Asteroid::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	if (alive) {
		target.draw(m_body, state);

	}
}
