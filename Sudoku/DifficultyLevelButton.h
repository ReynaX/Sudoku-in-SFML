#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

class DifficultyLevelButton final : public sf::CircleShape
{
	enum class DifficultyLevel { EASY = 0, MEDIUM, HARD };
public:
	DifficultyLevelButton(float x, float y, float radius, sf::Color backgroundColor, const sf::Font& font, const sf::Texture &texture, int difficultyLevel);
	void draw(sf::RenderTarget* target, sf::RenderStates states = sf::RenderStates::Default) const;
	void update(bool isChecked);
	bool isChecked();
private:
	DifficultyLevel m_difficultyLevel;
	sf::Texture m_texture;
	bool m_isChecked;
	sf::Text m_text;
	sf::Font m_font;
	sf::Color m_color;
};

