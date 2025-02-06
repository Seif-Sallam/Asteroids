#pragma once
#include "Player.h"
#include "Explosion.h"
#include "Asteroid.h"
#include "GameState.h"
#include "Target.h"
#include "SFML/Audio.hpp"
#include "KeySprite.h"
#include <queue>

class Level
{
public:
	Level();
	void Run();
	~Level();

private:
	void EventHandler();
	void Update();
	void Draw();

	bool Colliding(Entity* a, Entity* b);

private:
	sf::RenderWindow* m_window;
	sf::Event* m_event;
	sf::Vector2i m_screenSize;

	std::vector<Entity*> m_entitiesVec;
	std::vector<Explosion*> m_explosionsVec;
	Player* m_player;

	sf::Texture backgroundTextures[4];
	sf::Sprite m_background;
	GameState m_gameState;

	sf::Clock m_clock;
	sf::Texture m_explosionTextureC;
	sf::Texture m_explosionTextureB;
	sf::Texture m_smallAsteroidTexture;
	sf::Texture m_bigAsteroidTexture;
	sf::Texture m_targetTexture;

	sf::Texture m_lifeTexture;
	sf::Sprite m_lifeSprite;
	int m_lives;

	float m_fAsteroidsTime;
	float m_fTargetTime;
	bool m_bSpawnTargets;
	bool m_bSpawnAsteroids;
	struct AsteroidAttribs {
		sf::Vector2f pos;
		int type = 0;
		float angle = 0.0f;
	};

	AsteroidAttribs newAsteroids[10];
	sf::Music m_backgroundMusic;
	float m_fMusicIntervals;
	float m_fMusicTime;
	std::queue<sf::Sound*> m_explosionSoundsQ;
	sf::SoundBuffer m_explosionsSoundBuf0;
	sf::SoundBuffer m_explosionsSoundBuf1;
	sf::SoundBuffer m_explosionsSoundBuf2;

	int m_iSelectedBackground = 0;

	sf::Text m_gameOverText;
	sf::Text m_scoreText;
	sf::Font m_font;

	std::vector<KeySprite> m_keySprites;

	int m_score;
};

