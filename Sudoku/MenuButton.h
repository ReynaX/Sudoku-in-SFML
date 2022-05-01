#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuButton final : public sf::RectangleShape
{
public:
	enum ButtonFunction{NEW_GAME, HINT, SOLVE_GAME};
	MenuButton(float x, float y, float width, float height, const sf::Font &font, const std::string &text, ButtonFunction function);
	
	ButtonFunction getFunction();
	void draw(sf::RenderTarget* target, sf::RenderStates states = sf::RenderStates::Default) const;
	void update(bool isHovered);
private:
	sf::Font m_font;
	sf::Text m_text;
	ButtonFunction m_buttonFunction;
	
	sf::Color m_activeColor;
	sf::Color m_idleColor;
	sf::Color m_hoverColor;
};

