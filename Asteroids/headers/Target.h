#pragma once
#include "Entity.h"

class Target : public Entity
{
public:
	Target(const sf::Vector2f& position, sf::Texture* targetTexture, const float& lifeTime);
	void Update(const float& deltaTime, const sf::Vector2i& screenSize) override;	
	~Target();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;
private:
	sf::Sprite m_body;
	float m_fTotalTime;
	float m_fTargetScale;
	float m_fCurrentScale;
	float m_fIncreaseRate;
	float m_fIntervalDur;
	float m_fLifeTime;
};

