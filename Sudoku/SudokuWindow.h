#pragma once
#include "MenuButton.h"
#include "SudokuGenerator.h"
#include "SudokuSquare.h"

class SudokuWindow{
public:
	SudokuWindow();
	void eventHandler();
private:
	void createButtons();
	void onMouseButtonClicked(const sf::Vector2f& mousePosition);
	void onMouseMoved(const sf::Vector2f& mousePosition);
	void onKeyButtonClicked(sf::Event::TextEvent text);
	void onBackspaceButtonClicked();
	void onNewGameButtonClicked();
	void onSolveButtonClicked();
	void onHintButtonClicked();
	void updateClock();
	
	void update(int rowClicked, int colClicked, int valueClicked);
	
	SudokuGenerator *m_generator;
	std::vector<SudokuSquare*> m_sudokuSquares;
	std::vector<MenuButton*> m_menuButtons;
	SudokuSquare* m_clickedSquare;
	bool m_gameFinished;
	sf::Font m_font;
	sf::Text m_clockText;
	sf::Text m_difficultyText;
	sf::Clock *m_clock;
};

