#include "SudokuSquare.h"

SudokuSquare::SudokuSquare(int rowOnBoard, int colOnBoard, float x, float y, float width, float height, const sf::Font &font, const std::string &text){
	this->m_row = rowOnBoard;
	this->m_col = colOnBoard;
	
	this->setSize(sf::Vector2f(width, height));
	this->setPosition(sf::Vector2f(x, y));
	this->setOutlineThickness(1);
	this->setOutlineColor(sf::Color::Black);

	this->m_font = font;
	this->m_text.setFont(m_font);
	this->m_text.setString(text);
	this->m_text.setFillColor(sf::Color::Black);
	this->m_text.setCharacterSize(24);
	
	this->m_text.setPosition(
		this->getPosition().x + this->getGlobalBounds().width / 3.f  - m_text.getGlobalBounds().width / 2.f,
		this->getPosition().y + this->getGlobalBounds().height / 3.f - m_text.getGlobalBounds().height / 2.f
	);
	
	this->m_idleColor = sf::Color::White;
	this->m_hoverColor = sf::Color(224, 255, 255);
	this->m_clickedColor = sf::Color(135, 206, 235);
	this->m_incorrectColor = sf::Color(220,20,6);
	this->m_activeColor = m_idleColor;
	this->setFillColor(m_activeColor);
	
	this->m_value = 0;
	this->m_isValueConstant = false;
	this->m_state = IDLE;
}

void SudokuSquare::draw(sf::RenderTarget *target, sf::RenderStates states) {
	target->draw(*this);
	target->draw(this->m_text);
}

void SudokuSquare::setValue(int value){
	if (m_isValueConstant) return;
	this->m_value = value;
	this->m_text.setString(std::to_string(value));
}

int SudokuSquare::getValue(){
	return m_value;
}

void SudokuSquare::setValueConstant(bool valueConstant){
	m_isValueConstant = valueConstant;
}

void SudokuSquare::update(SquareState newState){
	switch(newState){
	case IDLE:
		this->m_activeColor = m_idleColor; break;
	case HOVERED:
		this->m_activeColor = m_hoverColor; break;
	case CLICKED:
		this->m_activeColor = m_clickedColor; break;
	case INCORRECT:
		this->m_activeColor = m_incorrectColor; break;
	}
	this->m_state = newState;
	this->setFillColor(m_activeColor);
}

