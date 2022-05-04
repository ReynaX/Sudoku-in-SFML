#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuButton final : public sf::RectangleShape
{
public:
	/// Function this button serves
	enum ButtonFunction{NEW_GAME, HINT, SOLVE_GAME};
	/**
	 * @param x window position
	 * @param y window position
	 * @param width width of square in pixels
	 * @param height height of square in pixels
	 * @param font font needed to generate text within a square
	 * @param text value to be stored
	 * @param function function this button serves
	*/
	MenuButton(float x, float y, float width, float height, const sf::Font &font, const std::string &text, ButtonFunction function);
	
	ButtonFunction getFunction();
	/**
	 * Draws this object with given states at given target
	 *
	 * @param target pointer to draw at
	 * @param states states for print object at <code>RenderTarget</code>
	 */
	void draw(sf::RenderTarget* target, sf::RenderStates states = sf::RenderStates::Default) const;
	/// Changes state of this object
	void update(bool isHovered);
private:
	sf::Font m_font;
	sf::Text m_text;
	ButtonFunction m_buttonFunction;
	
	sf::Color m_activeColor;
	sf::Color m_idleColor;
	sf::Color m_hoverColor;
};

