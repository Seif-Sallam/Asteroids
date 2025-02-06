#include <SFML/Graphics.hpp>

class KeySprite : public sf::Drawable
{
public:
    KeySprite(sf::Font& font, const std::string& key, const std::string& description, const sf::Vector2f& position, const sf::Vector2f& size)
    {
        m_keyText.setFont(font);
        m_description.setFont(font);
        m_keyText.setString(key);
        m_description.setString(description);
        m_keyText.setCharacterSize(20);
        m_description.setCharacterSize(20);
        m_keyText.setFillColor(sf::Color::White);
        m_description.setFillColor(sf::Color::White);

        m_shape.setSize(size);
        m_shape.setFillColor(sf::Color(0, 0, 0, 125));
        m_shape.setOutlineColor(sf::Color::White);
        m_shape.setOutlineThickness(1.0f);
        m_shape.setPosition(position);

        m_keyText.setPosition(position.x + 5.0f, position.y );
        m_description.setPosition(position.x + size.x + 16.0f, position.y + 4.0f);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(m_shape, states);
        target.draw(m_keyText, states);
        target.draw(m_description, states);
    }
private:
    sf::Text m_keyText;
    sf::Text m_description;
    sf::RectangleShape m_shape;
};
