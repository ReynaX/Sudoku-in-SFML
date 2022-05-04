#pragma once

#include <SFML/Graphics.hpp>

class SudokuSquare final : public sf::RectangleShape
{
public:
	enum SquareState { IDLE, HOVERED, CLICKED, INCORRECT };
	/**
	 * @param rowOnBoard Y-position on the board
	 * @param colOnBoard X-position on the board
	 * @param x window position
	 * @param y window position
	 * @param width width of square in pixels
	 * @param height height of square in pixels
	 * @param font font needed to generate text within a square
	 * @param text value to be stored
	 */
	SudokuSquare(int rowOnBoard, int colOnBoard, float x, float y, float width, float height, const sf::Font& font, const std::string& text);

	/**
	 * Draws this object with given states at given target
	 *
	 * @param target pointer to draw at
	 * @param states states for print object at <code>RenderTarget</code>
	 */
	void draw(sf::RenderTarget* target, sf::RenderStates states = sf::RenderStates::Default) const;

	void setValue(int value);
	int getValue();

	void setValueConstant(bool valueConstant);
	bool getValueConstant();

	int getRow(){ return m_row; }
	int getCol(){ return m_col; }
	/// Changes state of this object
	void update(SquareState newState);
private:
	int m_row, m_col; /// Position on the board
	sf::Text m_text; /// Text that contains the numeric value of square
	sf::Font m_font;
	
	sf::Color m_idleColor;
	sf::Color m_clickedColor;
	sf::Color m_hoverColor;
	sf::Color m_incorrectColor;
	sf::Color m_activeColor;
	
	SquareState m_state; /// Current state of a square
	int m_value; /// Numeric value of number within this square or 0 if there is no number
	bool m_isValueConstant; /// True if value within this square can be modified
};
