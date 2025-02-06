#include "Target.h"

Target::Target(const sf::Vector2f& position, sf::Texture* m_targetTexture, const float& lifeTime)
{
	this->position = position;
	m_body.setTexture(*m_targetTexture);
	m_body.setOrigin(32.0f, 32.0f);
	m_body.setPosition(position);
	m_fCurrentScale = 1.0f;
	m_fTargetScale = 0.7f;
	m_fTotalTime = 0.0f;
	m_fIntervalDur = 0.01f;
	m_fLifeTime = lifeTime;
	m_fIncreaseRate = 0.1f;
}

void Target::Update(const float& deltaTime, const sf::Vector2i& screenSize)
{
	m_fTotalTime += deltaTime;
	if (m_fTotalTime < m_fLifeTime) {
		if (m_fTotalTime >= m_fIntervalDur)
		{
			m_fIntervalDur += m_fIncreaseRate;
			if (m_fCurrentScale - m_fTargetScale > 0) {
				m_fCurrentScale -= 0.01;
				if (m_fCurrentScale <= m_fTargetScale) {
					m_fCurrentScale = 0.7f;
					m_fTargetScale = 1.0f;
				}
			}
			else if (m_fCurrentScale - m_fTargetScale < 0)
			{
				m_fCurrentScale += 0.01;
				if (m_fCurrentScale >= m_fTargetScale)
				{
					m_fCurrentScale = 1.0f;
					m_fTargetScale = 0.7f;
				}
			}
			m_body.setScale(m_fCurrentScale, m_fCurrentScale);
		}
	}
	else
	{
		alive = false;
	}
}

Target::~Target()
{
}

void Target::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(m_body, state);
}
