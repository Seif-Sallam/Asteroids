#include "Animation.h"

Animation::Animation(const sf::IntRect& ogRect, const sf::Vector2i& step, int num, const float updateInterval, bool looped)
	: m_fUpdateInterval(updateInterval)
{
	m_rectSize = { ogRect.width , ogRect.height };
	m_animStep = step;
	m_ogRect = ogRect;
	m_iCurrentFrame = 0;
	m_irCurrentStep = ogRect;
	m_fElapsedTime = 0.f;
	m_bLooped = looped;
	m_iNumberOfFrames = num;
	m_finished = false;
}

sf::IntRect Animation::GetCurrentStep()
{
	return m_irCurrentStep;
}

void Animation::Update(float deltaTime)
{
	m_fElapsedTime += deltaTime;
	if (m_fElapsedTime >= m_fUpdateInterval)
	{
		m_fElapsedTime = 0.f;
		if (m_bLooped)
		{
			if (m_iCurrentFrame >= m_iNumberOfFrames)
			{
				m_iCurrentFrame = 0;
				m_irCurrentStep = m_ogRect;
			}
		}
		else
		{
			if (m_iCurrentFrame >= m_iNumberOfFrames)
				m_finished = true;
		}
		if (!m_finished) {
			m_irCurrentStep.left = m_irCurrentStep.left + m_animStep.x;
			m_iCurrentFrame++;
		}
	}
}

bool Animation::Finished()
{
	if (!m_bLooped)
		return m_finished;
	return false;
}
