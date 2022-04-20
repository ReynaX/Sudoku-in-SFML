#pragma once

#include <SFML/Graphics.hpp>
class SudokuSquare : public sf::RectangleShape
{
public:
	enum SquareState {IDLE, HOVERED, CLICKED, INCORRECT};
	SudokuSquare(int rowOnBoard, int colOnBoard, float x, float y, float width, float height, const sf::Font &font, const std::string &text);

	void draw(sf::RenderTarget *target, sf::RenderStates states);

	void setValue(int value);
	int getValue();
	void setValueConstant(bool valueConstant);
	void update(SquareState newState);
	int getRow() { return m_row;  }
	int getCol() { return m_col; }
private:
	int m_row, m_col;
	sf::Text m_text;
	sf::Font m_font;
	sf::Color m_idleColor;
	sf::Color m_clickedColor;
	sf::Color m_hoverColor;
	sf::Color m_incorrectColor;
	sf::Color m_activeColor;
	SquareState m_state;
	int m_value;
	bool m_isValueConstant;
};

