#include "Player.h"

Player::Player(const sf::Vector2f &position, std::vector<Entity *> &entitiesVector)
	: m_entitiesVec(entitiesVector)
{
	this->position = position;
	m_shipTexture.loadFromFile(RSC_DIR "textures/spaceship.png");
	m_shipTexture.setSmooth(true);
	m_bulletTexture.loadFromFile(RSC_DIR "textures/fire_blue.png");
	m_bulletTexture.setSmooth(true);
	m_body.setTexture(m_shipTexture);
	m_body.setOrigin(20.f, 18.f);
	m_body.setPosition(position);
	m_bThrusting = false;
	m_velocity = {1.0f, 1.0f};
	m_direction = {0.0f, 0.0f};
	m_fMaxSpeed = 350.0f;
	m_currentDir = Direction::FRONT;
	m_animationElapsedTime = 0.f;
	m_fAngle = 0.f;
	m_bulletVec.reserve(50);
	m_missileVec.reserve(4);
	bulletFireRate = 0.1f;
	totalElapsedTime = 0.f;
	bulletNextShot = 0.1f;
	missileNextShot = 0.4f;
	missileFireRate = 0.4f;
	radius = 15.0f;

	m_bulletSoundBuf.loadFromFile(RSC_DIR "SoundEffects/laserShot.wav");
}

void Player::Update(const float &deltaTime, const sf::Vector2i &screenSize)
{
	if (alive)
	{
		totalElapsedTime += deltaTime;

		auto toRadian = [](float degree)
		{ return degree * 3.14 / 180.f; };
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			m_bThrusting = true;
			m_currentDir = Direction::FRONT;
		}
		else
		{
			m_bThrusting = false;
			m_currentDir = Direction::FRONT;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			m_fAngle -= 300.0f * deltaTime;
			m_currentDir = Direction::LEFT;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			m_fAngle += 300.0f * deltaTime;
			m_currentDir = Direction::RIGHT;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && activeBullets < 50 && totalElapsedTime > bulletNextShot)
		{
			AddBullet();
			bulletNextShot = totalElapsedTime + bulletFireRate;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && activeMissiles < 5 && totalElapsedTime > missileNextShot)
		{
			AddMissile();
			missileNextShot = totalElapsedTime + missileFireRate;
		}

		m_direction.x = cos(toRadian(m_fAngle));
		m_direction.y = sin(toRadian(m_fAngle));

		float dis = sqrt(pow(m_direction.x, 2) + pow(m_direction.y, 2));

		m_direction = m_direction / dis;

		if (m_bThrusting)
		{
			m_velocity.x += m_direction.x * 4.0f;
			m_velocity.y += m_direction.y * 4.0f;
		}
		else
		{
			m_velocity.x *= 0.999f;
			m_velocity.y *= 0.999f;
		}
		float speed = sqrt(pow(m_velocity.x, 2) + pow(m_velocity.y, 2));
		if (speed > m_fMaxSpeed)
			m_velocity *= m_fMaxSpeed / speed;

		sf::Vector2f moveDis = sf::Vector2f(m_velocity.x * deltaTime, m_velocity.y * deltaTime);
		m_body.move(moveDis);

		position = m_body.getPosition();
		if (position.x > screenSize.x)
			m_body.setPosition(0.0f, position.y);
		else if (position.y > screenSize.y)
			m_body.setPosition(position.x, 0.0f);
		else if (position.x < 0.f)
			m_body.setPosition(screenSize.x, position.y);
		else if (position.y < 0.f)
			m_body.setPosition(position.x, screenSize.y);

		position = m_body.getPosition();
		m_body.setRotation(m_fAngle + 90.0f);

		UpdateAnimation(deltaTime);

		if (m_sounds.size() > 0)
		{
			if (m_sounds[0]->getStatus() == sf::Sound::Stopped)
			{
				delete m_sounds[0];
				m_sounds.erase(m_sounds.begin());
			}
		}
	}
}

void Player::AddMissile()
{
	sf::Vector2f pos = sf::Vector2f(position.x + m_direction.x * 2.0f, position.y + m_direction.y * 2.0f);
	m_entitiesVec.push_back(new Missile(pos, m_direction, &m_shipTexture));
	activeMissiles++;
}

void Player::AddBullet()
{
	sf::Vector2f pos = sf::Vector2f(position.x + m_direction.x * 2.0f, position.y + m_direction.y * 2.0f);
	m_entitiesVec.push_back(new Bullet(pos, m_direction, &m_bulletTexture));
	activeBullets++;

	m_sounds.push_back(new sf::Sound(m_bulletSoundBuf));
	m_sounds.back()->setVolume(100.0f);
	m_sounds.back()->play();
}

void Player::CleanBullets()
{
	for (int i = 0; i < m_sounds.size(); i++)
	{
		delete m_sounds[i];
		m_sounds.erase(m_sounds.begin() + i);
	}
	for (auto &e : m_entitiesVec)
	{
		if (typeid(*e) == typeid(Bullet))
			e->alive = false;
	}
}

Player::~Player()
{
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates state) const
{
	target.draw(m_body, state);
}

void Player::UpdateAnimation(const float &deltaTime)
{
	m_animationElapsedTime += deltaTime;
	sf::IntRect currentRect = m_body.getTextureRect();
	if (m_currentDir == Direction::FRONT)
	{
		if (!m_bThrusting)
		{
			currentRect = sf::IntRect(39, 0, 40, 40);
			m_animationElapsedTime = 0.0f;
		}
		else if (m_bThrusting && m_animationElapsedTime < 0.5f)
		{
			currentRect = sf::IntRect(39, 40, 40, 43);
		}
		else if (m_bThrusting && m_animationElapsedTime > 0.5f)
		{
			currentRect = sf::IntRect(39, 86, 40, 40);
			if (m_animationElapsedTime > 1.0f)
				m_animationElapsedTime = 0.f;
		}
	}
	else if (m_currentDir == Direction::RIGHT)
	{
		if (!m_bThrusting)
		{
			currentRect = sf::IntRect(0, 0, 38, 40);
			m_animationElapsedTime = 0.0f;
		}
		else if (m_bThrusting && m_animationElapsedTime < 0.5f)
		{
			currentRect = sf::IntRect(0, 40, 38, 43);
		}
		else if (m_bThrusting && m_animationElapsedTime > 0.5f)
		{
			currentRect = sf::IntRect(0, 86, 38, 40);
			if (m_animationElapsedTime > 1.0f)
				m_animationElapsedTime = 0.0f;
		}
	}
	else if (m_currentDir == Direction::LEFT)
	{
		if (!m_bThrusting)
		{
			currentRect = sf::IntRect(86, 0, 36, 40);
			m_animationElapsedTime = 0.f;
		}
		else if (m_bThrusting && m_animationElapsedTime < 0.5f)
		{
			currentRect = sf::IntRect(86, 40, 40, 43);
		}
		else if (m_bThrusting && m_animationElapsedTime > 0.5f)
		{
			currentRect = sf::IntRect(86, 86, 40, 40);
			if (m_animationElapsedTime > 1.0f)
				m_animationElapsedTime = 0.0f;
		}
	}
	m_body.setTextureRect(currentRect);
}
