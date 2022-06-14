#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class DifficultyLevelButton final : public sf::CircleShape
{
	enum class DifficultyLevel { EASY = 0, MEDIUM, HARD };
public:
	DifficultyLevelButton(float x, float y, float radius, const sf::Font& font, sf::Texture *texture, int difficultyLevel);
	~DifficultyLevelButton() override;
	
	void draw(sf::RenderTarget* target, sf::RenderStates states = sf::RenderStates::Default) const;
	void update(bool isChecked);
	bool isChecked();
private:
	DifficultyLevel m_difficultyLevel;
	sf::Texture *m_texture;
	bool m_isChecked;
	sf::Text m_text;
	sf::Font m_font;
};

