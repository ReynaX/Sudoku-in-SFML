#include "SudokuSquare.h"

SudokuSquare::SudokuSquare(int rowOnBoard, int colOnBoard, float x, float y, float width, float height, const sf::Font &font, const std::string &text){
	m_row = rowOnBoard;
	m_col = colOnBoard;
	
	setSize(sf::Vector2f(width, height));
	setPosition(sf::Vector2f(x, y));
	setOutlineThickness(1);
	setOutlineColor(sf::Color::Black);

	m_font = font;
	m_text.setFont(m_font);
	m_text.setString(text);
	m_text.setFillColor(sf::Color::Black);
	m_text.setCharacterSize(24);
	
	m_text.setPosition(
		getPosition().x + getGlobalBounds().width / 3.f  - m_text.getGlobalBounds().width / 2.f,
		getPosition().y + getGlobalBounds().height / 3.f - m_text.getGlobalBounds().height / 2.f
	);
	
	m_idleColor = sf::Color::White;
	m_hoverColor = sf::Color(224, 255, 255);
	m_clickedColor = sf::Color(135, 206, 235);
	m_incorrectColor = sf::Color(220,20,6);
	m_activeColor = m_idleColor;
	setFillColor(m_activeColor);
	
	m_value = 0;
	m_isValueConstant = false;
	m_state = IDLE;
}

void SudokuSquare::draw(sf::RenderTarget *target, sf::RenderStates states) const{
	target->draw(*this, states);
	target->draw(m_text);
}

void SudokuSquare::setValue(int value){
	m_value = value;
	if(value != 0)
		m_text.setString(std::to_string(value));
	else m_text.setString("");
}

int SudokuSquare::getValue(){
	return m_value;
}

void SudokuSquare::setValueConstant(bool valueConstant){
	m_isValueConstant = valueConstant;
}

bool SudokuSquare::getValueConstant(){
	return m_isValueConstant;
}

void SudokuSquare::update(SquareState newState){
	switch(newState){
	case IDLE:
		m_activeColor = m_idleColor; break;
	case HOVERED:
		m_activeColor = m_hoverColor; break;
	case CLICKED:
		m_activeColor = m_clickedColor; break;
	case INCORRECT:
		m_activeColor = m_incorrectColor; break;
	}
	m_state = newState;
	setFillColor(m_activeColor);
}

