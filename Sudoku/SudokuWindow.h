#pragma once
#include "SudokuGenerator.h"
#include "SudokuSquare.h"

class SudokuWindow{
public:
	SudokuWindow();
	void eventHandler();
private:
	void createButtons();
	void onMouseButtonClicked(const sf::Vector2f& mousePosition);
	void onKeyButtonClicked(sf::Event::TextEvent text);
	void onBackspaceButtonClicked();
	void update(int rowClicked, int colClicked, int valueClicked);
	
	SudokuGenerator m_generator;
	std::vector<SudokuSquare*> m_sudokuSquares;
	SudokuSquare* m_clickedSquare;
	bool m_gameFinished;
};

