#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Bullet.h"
#include "Missile.h"
#include "Entity.h"
#include <queue>

enum class Direction {
	LEFT,
	RIGHT,
	FRONT
};


class Player : public Entity
{
public:
	Player(const sf::Vector2f& position, std::vector<Entity*>& entitiesVector);
	void Update(const float& deltaTime, const sf::Vector2i& screenSize) override;
	void AddMissile();
	void AddBullet();
	void CleanBullets();
	int activeBullets = 0;
	int activeMissiles = 0;

	~Player();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

private:
	void UpdateAnimation(const float& deltaTime);

	sf::Texture m_shipTexture;
	sf::Texture m_bulletTexture;
	sf::Sprite m_body;
	float m_fAngle;
	sf::Vector2f m_velocity;
	sf::Vector2f m_direction;
	float m_fMaxSpeed;
	bool m_bThrusting;

	Direction m_currentDir;
	std::vector<Bullet*> m_bulletVec;
	std::vector<Missile*> m_missileVec;
	std::vector<Entity*>& m_entitiesVec;
	float m_animationElapsedTime;

	float bulletNextShot;
	float totalElapsedTime;
	float bulletFireRate;

	float missileNextShot;
	float missileFireRate;
	
	sf::SoundBuffer m_bulletSoundBuf;
	std::vector<sf::Sound*> m_sounds;
	sf::Sound m_bulletSound;

};

