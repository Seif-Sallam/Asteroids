#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <math.h>
#include <string.h>
#include <sstream>

class Animation
{
public:
	Animation(const sf::IntRect& ogRect, const sf::Vector2i& step, int numOfFrames, const float updateInterval = 0.f, bool looped = true);
	sf::IntRect GetCurrentStep();
	void Update(float deltaTime);
	bool Finished();
private:
	sf::Vector2i m_animStep;
	sf::Vector2i m_rectSize;
	std::string m_animPattern;
	int m_iNumberOfFrames;
	int m_iCurrentFrame;
	sf::IntRect m_irCurrentStep;
	sf::IntRect m_ogRect;
	const float m_fUpdateInterval;
	float m_fElapsedTime;
	bool m_bLooped;
	bool m_finished;

};

