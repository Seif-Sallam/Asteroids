#pragma once
#include "Player.h"
#include "Explosion.h"
#include "Astroid.h"
#include "GameState.h"
#include "Target.h"
#include "SFML/Audio.hpp"
#include <queue>

class Level
{
public:
	Level(GameState* state);
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

	sf::Texture backgroundTextrue;
	sf::Sprite m_background;
	GameState* m_gameState;
	
	
	sf::Clock clock;
	sf::Texture explosionTextureC;
	sf::Texture explosionTextureB;
	sf::Texture smallAstroidTexture;
	sf::Texture bigAstroidTexture;
	sf::Texture targetTexture;
	
	
	sf::Texture lifeTexture;
	sf::Sprite* lifeSprites;
	int lives;

	float m_fAstroidsTime;
	float m_fTargetTime;
	bool m_bSpawnTargets;
	bool m_bSpawnAstroids;
	struct AstroidAttribs {
		sf::Vector2f pos;
		int type = 0.f, angle = 0.f;
	};

	AstroidAttribs newAstroids[10];
	sf::Music m_backgroundMusic;
	float m_fMusicIntervals;
	float m_fMusicTime;
	//std::vector<sf::Sound*> m_explosionSounds;
	std::queue<sf::Sound*> m_explosionSoundsQ;
	sf::SoundBuffer m_explosionsSoundBuf0;
	sf::SoundBuffer m_explosionsSoundBuf1;
	sf::SoundBuffer m_explosionsSoundBuf2;
};

