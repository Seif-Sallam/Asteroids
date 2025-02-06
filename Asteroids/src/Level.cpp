#include "Level.h"

Level::Level()
{
	m_gameState = GameState::Playing;

	m_screenSize.x = sf::VideoMode::getDesktopMode().width;
	m_screenSize.y = sf::VideoMode::getDesktopMode().height;
	srand(static_cast<unsigned int>(time(0)));
	m_window = new sf::RenderWindow(sf::VideoMode(m_screenSize.x, m_screenSize.y), "Asteroids", sf::Style::Fullscreen);
	m_event = new sf::Event;
	m_window->setFramerateLimit(60u);
	for (int i = 0; i < 4; i++)
		backgroundTextures[i].loadFromFile(RSC_DIR "textures/background" + std::to_string(i) + ".jpg");
	int number = rand() % 4;

	m_background.setTexture(backgroundTextures[number]);

	m_player = new Player(sf::Vector2f(m_screenSize.x / 2.0f, m_screenSize.y / 2.0f), m_entitiesVec);

	m_entitiesVec.reserve(50);
	m_explosionsVec.reserve(50);
	m_entitiesVec.push_back((Entity *)m_player);

	m_explosionTextureB.loadFromFile(RSC_DIR "textures/explosions/type_B.png");
	m_explosionTextureB.setSmooth(true);
	m_explosionTextureC.loadFromFile(RSC_DIR "textures/explosions/type_C.png");
	m_explosionTextureC.setSmooth(true);
	m_smallAsteroidTexture.loadFromFile(RSC_DIR "textures/rock_small.png");
	m_smallAsteroidTexture.setSmooth(true);
	m_bigAsteroidTexture.loadFromFile(RSC_DIR "textures/rock.png");
	m_bigAsteroidTexture.setSmooth(true);
	m_targetTexture.loadFromFile(RSC_DIR "textures/target.png");
	m_targetTexture.setSmooth(true);
	m_lifeTexture.loadFromFile(RSC_DIR "textures/spaceship.png", sf::IntRect(39, 0, 40, 40));
	m_lifeTexture.setSmooth(true);

	m_explosionsSoundBuf0.loadFromFile(RSC_DIR "SoundEffects/explosion0.wav");
	m_explosionsSoundBuf1.loadFromFile(RSC_DIR "SoundEffects/explosion1.wav");
	m_explosionsSoundBuf2.loadFromFile(RSC_DIR "SoundEffects/explosion2.wav");

	m_lives = 3;
	m_lifeSprite.setTexture(m_lifeTexture);


	for (int i = 0; i < 15; i++)
	{
		int type = rand() % 2;
		m_entitiesVec.push_back(new Asteroid(sf::Vector2f(static_cast<float>(rand() % m_screenSize.x), static_cast<float>(rand() % m_screenSize.y)),
		 			type,
					static_cast<float>(rand() % 360),
					(type == AsteroidSize::SMALL) ? &m_smallAsteroidTexture : &m_bigAsteroidTexture));
	}


	if (m_font.loadFromFile(RSC_DIR "fonts/KnowingHow.ttf"))
		std::cout << "Font loaded successfully" << std::endl;
	else
		std::cout << "Font failed to load" << std::endl;
	m_gameOverText.setFont(m_font);


	m_gameOverText.setString("                Game Over\nPress 'ESC' to close the game");
	m_gameOverText.setCharacterSize(100);
	m_gameOverText.setFillColor(sf::Color::Red);
	m_gameOverText.setPosition(m_screenSize.x / 2.0f - m_gameOverText.getGlobalBounds().width / 2.0f,
							 m_screenSize.y / 2.0f - m_gameOverText.getGlobalBounds().height / 2.0f);

	m_score = 0;
	m_scoreText.setFont(m_font);
	m_scoreText.setString("Score: 0");
	m_scoreText.setCharacterSize(30);
	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setPosition(m_screenSize.x - 200.0f, 0.0f);

	m_fAsteroidsTime = 0.f;

	m_backgroundMusic.openFromFile(RSC_DIR "Music/space walk.ogg");

	m_fMusicIntervals = 0.1f;
	m_fMusicTime = 0.f;
	m_backgroundMusic.setVolume(5.f);

	m_fTargetTime = 0.f;
	m_bSpawnTargets = false;
	m_bSpawnAsteroids = false;
}

void Level::Run()
{
	while (m_gameState != GameState::Exit)
	{
		EventHandler();
		Update();
		Draw();
	}
}

Level::~Level()
{
	for (auto &e : m_explosionsVec)
		delete e;
	for (auto &e : m_entitiesVec)
		delete e;

	delete m_window;
	delete m_event;
	delete m_player;
}

void Level::EventHandler()
{
	while (m_window->pollEvent(*m_event))
	{
		if (m_event->type == sf::Event::Closed || m_event->type == sf::Event::KeyPressed && m_event->key.code == sf::Keyboard::Escape)
			m_gameState = GameState::Exit;
		if (m_event->type == sf::Event::KeyPressed && m_event->key.code == sf::Keyboard::K)
		{
			m_entitiesVec.push_back(new Asteroid(sf::Vector2f(static_cast<float>(rand() % m_screenSize.x), static_cast<float>(rand() % m_screenSize.y)),
			AsteroidSize::BIG,
			static_cast<float>(rand() % 360),
			&m_bigAsteroidTexture));
		}
		if (m_event->type == sf::Event::KeyPressed && m_event->key.code == sf::Keyboard::B)
		{
			if (m_iSelectedBackground == 3)
				m_iSelectedBackground = 0;
			m_background.setTexture(backgroundTextures[m_iSelectedBackground]);
			m_iSelectedBackground++;
		}
	}
}

void Level::Update()
{
	float deltaTime = m_clock.restart().asSeconds();
	m_fAsteroidsTime += deltaTime;
	m_fTargetTime += deltaTime;

	if (m_backgroundMusic.getStatus() == sf::Music::Stopped)
	{
		m_backgroundMusic.play();
		m_backgroundMusic.setPlayingOffset(sf::seconds(0.0f));
		m_fMusicTime = 0.f;
		m_backgroundMusic.setVolume(10.0f);
	}
	m_backgroundMusic.setVolume(30.0f);

	if (m_backgroundMusic.getVolume() < 100.f)
	{
		m_fMusicTime += deltaTime;
		if (m_fMusicTime >= m_fMusicIntervals)
		{
			m_fMusicIntervals += 0.1f;
			m_backgroundMusic.setVolume(m_backgroundMusic.getVolume() + 5.0f);
		}
	}

	if (m_fTargetTime > 20.0f)
	{
		m_fTargetTime = 0.0f;
		m_bSpawnTargets = true;
	}
	if (m_fAsteroidsTime > 25.f)
	{

		m_fAsteroidsTime = 0.f;
		m_fTargetTime = 0.0f;
		m_bSpawnAsteroids = true;
	}

	if (m_bSpawnTargets && m_lives > 0)
	{
		m_bSpawnTargets = false;
		for (auto &a : m_entitiesVec)
			if (typeid(*a) == typeid(Target))
				a->alive = false;

		for (int i = 0; i < 10; i++)
		{
			sf::Vector2f pos = sf::Vector2f(static_cast<float>(rand() % m_screenSize.x), static_cast<float>(rand() % m_screenSize.y));
			int type = rand() % 2;
			float angle = static_cast<float>(rand() % 360);

			m_entitiesVec.push_back(new Target(pos, &m_targetTexture, 5.f));
			newAsteroids[i].pos = pos;
			newAsteroids[i].type = type;
			newAsteroids[i].angle = angle;
		}
	}

	if (m_bSpawnAsteroids && m_lives > 0)
	{
		m_bSpawnAsteroids = false;
		for (int i = 0; i < 10; i++)
		{
			int type = newAsteroids[i].type;
			m_entitiesVec.push_back(new Asteroid(newAsteroids[i].pos, newAsteroids[i].type, newAsteroids[i].angle, (type == AsteroidSize::BIG) ? &m_bigAsteroidTexture : &m_smallAsteroidTexture));
		}
	}

	// Checking Collision
	if (m_lives > 0)
	{
		for (int i = 0; i < m_entitiesVec.size(); i++)
		{
			auto a = m_entitiesVec[i];
			for (int j = 0; j < m_entitiesVec.size(); j++)
			{
				auto b = m_entitiesVec[j];
				if (!(typeid(*a) == typeid(Bullet) && typeid(*b) == typeid(Player)) && typeid(*b) != typeid(*a))
				{
					if (a->alive && b->alive && Colliding(a, b))
					{
						if (typeid(*a) == typeid(Bullet) && typeid(*b) == typeid(Asteroid))
						{
							// Send it to our garbage collector in the end of the update
							a->alive = false;
							b->alive = false;
							Asteroid *asteroid = (Asteroid*)b;
							if (asteroid->type == AsteroidSize::BIG)
							{
								m_score += 100;
								m_entitiesVec.push_back(new Asteroid(sf::Vector2f(b->position),
								AsteroidSize::SMALL,
								static_cast<float>(rand() % 360),
								&m_smallAsteroidTexture));
								m_entitiesVec.push_back(new Asteroid(sf::Vector2f(b->position), AsteroidSize::SMALL, static_cast<float>(rand() % 360), &m_smallAsteroidTexture));
							}
							else
							{
								m_score += 50;
							}

							m_scoreText.setString("Score: " + std::to_string(m_score));

							m_explosionsVec.push_back(new Explosion(sf::Vector2f(b->position), (int)ExplosionTypes::ASTEROID, &m_explosionTextureC));
							int explosionType = rand() % 3;
							switch (explosionType)
							{
							case 0:
								m_explosionSoundsQ.push(new sf::Sound(m_explosionsSoundBuf0));
								break;
							case 1:
								m_explosionSoundsQ.push(new sf::Sound(m_explosionsSoundBuf1));
								break;
							case 2:
								m_explosionSoundsQ.push(new sf::Sound(m_explosionsSoundBuf2));
								break;
							}
							m_explosionSoundsQ.back()->play();
						}
						else if (typeid(*a) == typeid(Player) && typeid(*b) == typeid(Asteroid))
						{
							a->alive = false;
							b->alive = false;

							Asteroid *asteroid = (Asteroid *)b;
							if (asteroid->type == AsteroidSize::BIG)
							{
								m_entitiesVec.push_back(new Asteroid(sf::Vector2f(a->position), AsteroidSize::SMALL, static_cast<float>(rand() % 360), &m_smallAsteroidTexture));
								m_entitiesVec.push_back(new Asteroid(sf::Vector2f(a->position), AsteroidSize::SMALL, static_cast<float>(rand() % 360), &m_smallAsteroidTexture));
							}
							m_explosionsVec.push_back(new Explosion(sf::Vector2f(b->position), (int)ExplosionTypes::SHIP, &m_explosionTextureB));
							m_player->CleanBullets();
							m_lives--;

							m_player = new Player(sf::Vector2f(m_screenSize.x / 2.0f, m_screenSize.y / 2.0f), m_entitiesVec);
							if (m_lives <= 0)
								m_player->makeAShadow();
							m_entitiesVec.push_back(m_player);
							int explosionType = rand() % 3;
							switch (explosionType)
							{
							case 0:
								m_explosionSoundsQ.push(new sf::Sound(m_explosionsSoundBuf0));
								break;
							case 1:
								m_explosionSoundsQ.push(new sf::Sound(m_explosionsSoundBuf1));
								break;
							case 2:
								m_explosionSoundsQ.push(new sf::Sound(m_explosionsSoundBuf2));
								break;
							}
							m_explosionSoundsQ.back()->play();
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < m_entitiesVec.size(); i++)
	{
		m_entitiesVec[i]->Update(deltaTime, m_screenSize);

		if (!m_entitiesVec[i]->alive)
		{
			if (typeid(*m_entitiesVec[i]) == typeid(Bullet))
				m_player->activeBullets--;
			if (typeid(*m_entitiesVec[i]) == typeid(Missile))
				m_player->activeMissiles--;

			delete m_entitiesVec[i];
			m_entitiesVec.erase(m_entitiesVec.begin() + i);
		}
	}

	for (int i = 0; i < m_explosionsVec.size(); i++)
	{
		m_explosionsVec[i]->Update(deltaTime);
		if (!m_explosionsVec[i]->alive)
		{
			delete m_explosionsVec[i];
			m_explosionsVec.erase(m_explosionsVec.begin() + i);
		}
	}

	while (m_explosionSoundsQ.size() > 0 && m_explosionSoundsQ.front()->getStatus() == sf::Sound::Stopped)
	{
		delete m_explosionSoundsQ.front();
		m_explosionSoundsQ.pop();
	}
}

void Level::Draw()
{
	m_window->clear();
	m_window->draw(m_background);
	for (auto &e : m_entitiesVec)
		m_window->draw(*e);

	for (auto &e : m_explosionsVec)
		m_window->draw(*e);

	if (m_lives <= 0)
		m_window->draw(m_gameOverText);

	for (int i = 0; i < m_lives; i++)
	{
		m_lifeSprite.setPosition(5.0f + i * 40, 0.0f);
		m_window->draw(m_lifeSprite);
	}

	m_window->draw(m_scoreText);

	m_window->display();
}

bool Level::Colliding(Entity *a, Entity *b)
{
	float rhs = (a->position.x - b->position.x) * (a->position.x - b->position.x) +
				(a->position.y - b->position.y) * (a->position.y - b->position.y);
	float lhs = (a->radius + b->radius) * (a->radius + b->radius);

	return rhs < lhs;
}