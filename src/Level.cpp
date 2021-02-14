#include "Level.h"

Level::Level(GameState* state)
{
	unsigned int sWidth = sf::VideoMode::getDesktopMode().width;
	unsigned int sHeight = sf::VideoMode::getDesktopMode().height;
	m_screenSize.x = sWidth;
	m_screenSize.y = sHeight;

	m_window = new sf::RenderWindow(sf::VideoMode(sWidth, sHeight), "Astroids", sf::Style::Default);
	m_event = new sf::Event;
	//m_window->setFramerateLimit(60u);

	backgroundTextrue.loadFromFile("textures/background.jpg");
	m_background.setTexture(backgroundTextrue);

	m_player = new Player(sf::Vector2f(sWidth / 2, sHeight / 2), m_entitiesVec);

	m_entitiesVec.reserve(50);
	m_explosionsVec.reserve(50);
	m_entitiesVec.push_back((Entity*)m_player);
	m_gameState = state;


	explosionTextureB.loadFromFile("textures/explosions/type_B.png");
	explosionTextureB.setSmooth(true);
	explosionTextureC.loadFromFile("textures/explosions/type_C.png");
	explosionTextureC.setSmooth(true);
	smallAstroidTexture.loadFromFile("textures/rock_small.png");
	smallAstroidTexture.setSmooth(true);
	bigAstroidTexture.loadFromFile("textures/rock.png");
	bigAstroidTexture.setSmooth(true);
	targetTexture.loadFromFile("textures/target.png");
	targetTexture.setSmooth(true);
	lifeTexture.loadFromFile("textures/spaceship.png", sf::IntRect(39, 0, 40, 40));
	lifeTexture.setSmooth(true);


	m_explosionsSoundBuf0.loadFromFile("SoundEffects/explosion0.wav");
	m_explosionsSoundBuf1.loadFromFile("SoundEffects/explosion1.wav");
	m_explosionsSoundBuf2.loadFromFile("SoundEffects/explosion2.wav");
	
	lives = 3;
	lifeSprites = new sf::Sprite[lives];

	for (int i = 0; i < lives; i++)
	{
		lifeSprites[i].setTexture(lifeTexture);
		lifeSprites[i].setPosition(5.0f + i * 40, 0.0f);
	}

	for (int i = 0; i < 15; i++)
	{
		int type = rand() % 2;
		m_entitiesVec.push_back(new Astroid(sf::Vector2f(rand() % m_screenSize.x, rand() % m_screenSize.y), type, rand() % 360, (type == AstroidSize::SMALL)? &smallAstroidTexture : &bigAstroidTexture));
	}

	m_fAstroidsTime = 0.f;

	m_backgroundMusic.openFromFile("Music/space walk.ogg");

	m_fMusicIntervals = 0.1f;
	m_fMusicTime = 0.f;
	m_backgroundMusic.setVolume(5.f);

	m_fTargetTime = 0.f;
	m_bSpawnTargets = false;
	m_bSpawnAstroids = false;
}

void Level::Run()
{
	while (*m_gameState != GameState::Exit)
	{
		EventHandler();
		Update();
		Draw();
	}
}

Level::~Level()
{
	for (auto& e : m_explosionsVec)
		delete e;
	for (auto& e : m_entitiesVec)
		delete e;
}

void Level::EventHandler()
{
	while (m_window->pollEvent(*m_event))
	{
		if (m_event->type == sf::Event::Closed || m_event->type == sf::Event::KeyPressed && m_event->key.code == sf::Keyboard::Escape)
			*m_gameState = GameState::Exit;
		if(m_event->type == sf::Event::KeyPressed && m_event->key.code == sf::Keyboard::K)
			m_entitiesVec.push_back(new Astroid(sf::Vector2f(rand() % m_screenSize.x, rand() % m_screenSize.y), AstroidSize::BIG, rand() % 360, &bigAstroidTexture));
	}
}

void Level::Update()
{
	float deltaTime = clock.restart().asSeconds();
	m_window->setTitle("Astroids,  FPS: " + std::to_string(1.0f / deltaTime));
	m_fAstroidsTime += deltaTime;
	m_fTargetTime += deltaTime;

	
	if (m_backgroundMusic.getStatus() == sf::Music::Stopped)
	{
		m_backgroundMusic.play();
		m_backgroundMusic.setPlayingOffset(sf::seconds(0.0f));
		m_fMusicTime = 0.f;
		m_backgroundMusic.setVolume(10.0f);
	}

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
	if (m_fAstroidsTime > 25.f) {
	
		m_fAstroidsTime = 0.f;		
		m_fTargetTime = 0.0f;
		m_bSpawnAstroids = true;
	}


	if (m_bSpawnTargets)
	{
		m_bSpawnTargets = false;
		for(auto& a : m_entitiesVec)
			if (typeid(*a) == typeid(Target))
				a->alive = false;


		for (int i = 0; i < 10; i++)
		{
			sf::Vector2f pos = sf::Vector2f(rand() % m_screenSize.x , rand() % m_screenSize.y);
			int type = rand() % 2;
			int angle = rand() % 360;

			m_entitiesVec.push_back(new Target(pos, &targetTexture, 5.f));
			newAstroids[i].pos = pos;
			newAstroids[i].type = type;
			newAstroids[i].angle = angle;
		}
	}

	if (m_bSpawnAstroids)
	{
		m_bSpawnAstroids = false;
		for (int i = 0; i < 10; i++)
		{
			int type = newAstroids[i].type;
			m_entitiesVec.push_back(new Astroid(newAstroids[i].pos, newAstroids[i].type, newAstroids[i].angle, (type == AstroidSize::BIG) ? 
																								&bigAstroidTexture : &smallAstroidTexture));
		}
	}

	//Chceking Collision
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
					if (typeid(*a) == typeid(Bullet) && typeid(*b) == typeid(Astroid))
					{
						// Send it to our garbage collector in the end of the update
						a->alive = false;
						b->alive = false;
						Astroid* astroid = (Astroid*)b;
						if (astroid->type == AstroidSize::BIG) {
							m_entitiesVec.push_back(new Astroid(sf::Vector2f(b->position), AstroidSize::SMALL, rand() % 360, &smallAstroidTexture));
							m_entitiesVec.push_back(new Astroid(sf::Vector2f(b->position), AstroidSize::SMALL, rand() % 360, &smallAstroidTexture));
						}
						m_explosionsVec.push_back(new Explosion(sf::Vector2f(b->position), (int)ExplosionTypes::ASTROID, &explosionTextureC));
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
					else if (typeid(*a) == typeid(Player) && typeid(*b) == typeid(Astroid))
					{
						a->alive = false;
						b->alive = false;

						Astroid* astroid = (Astroid*)b;
						if (astroid->type == AstroidSize::BIG) {
							m_entitiesVec.push_back(new Astroid(sf::Vector2f(a->position), AstroidSize::SMALL, rand() % 360, &smallAstroidTexture));
							m_entitiesVec.push_back(new Astroid(sf::Vector2f(a->position), AstroidSize::SMALL, rand() % 360, &smallAstroidTexture));
						}
						m_explosionsVec.push_back(new Explosion(sf::Vector2f(b->position), (int)ExplosionTypes::SHIP, &explosionTextureB));
						m_player->CleanBullets();
						lives--;
						m_player = new Player(sf::Vector2f(m_screenSize.x / 2, m_screenSize.y / 2), m_entitiesVec);
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
	for (int i = 0; i < m_explosionsVec.size(); i++) {
		m_explosionsVec[i]->Update(deltaTime);
		if (!m_explosionsVec[i]->alive)
		{
			delete m_explosionsVec[i];
			m_explosionsVec.erase(m_explosionsVec.begin() + i);
		}
	}

	while(m_explosionSoundsQ.size() > 0 && m_explosionSoundsQ.front()->getStatus() == sf::Sound::Stopped)
	{
		delete m_explosionSoundsQ.front();
		m_explosionSoundsQ.pop();
	}
}

void Level::Draw()
{
	m_window->clear();
	m_window->draw(m_background);
	for (auto& e : m_entitiesVec)
		m_window->draw(*e);

	for (auto& e : m_explosionsVec)
		m_window->draw(*e);

	if (lives > 0)
	{
		for (int i = 0; i < lives; i++)
			m_window->draw(lifeSprites[i]);
	}

	m_window->display();
}

bool Level::Colliding(Entity* a, Entity* b)
{
	float rhs = (a->position.x - b->position.x) * (a->position.x - b->position.x) + 
				(a->position.y - b->position.y) * (a->position.y - b->position.y);
	float lhs = (a->radius + b->radius) * (a->radius + b->radius);

	return rhs < lhs;
}