#include "SudokuWindow.h"

#include <iostream>
#include "Line.h"

SudokuWindow::SudokuWindow(){
	this->m_sudokuSquares.resize(81);
	this->m_clickedSquare = nullptr;
	this->m_gameFinished = false;
	createButtons();
	eventHandler();
}

void SudokuWindow::eventHandler(){
	Line vertices[] = {
	Line(sf::Vector2f(150, 0), sf::Vector2f(150, 500)),
	Line(sf::Vector2f(300, 0), sf::Vector2f(300, 500)),
	Line(sf::Vector2f(0, 150), sf::Vector2f(500, 150)),
	Line(sf::Vector2f(0, 300), sf::Vector2f(500, 300))
	};
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8.0;
	sf::RenderWindow window(sf::VideoMode(550, 480), "Sudoku v1.0", sf::Style::Close, settings);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left)
					onMouseButtonClicked(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
			}
			if (event.type == sf::Event::TextEntered)
				onKeyButtonClicked(event.text);
		}
		window.clear();
		for (int i = 0; i < 81; ++i) {
			this->m_sudokuSquares[i]->draw(&window, sf::RenderStates::Default);
		}
		window.draw(vertices[0]);
		window.draw(vertices[1]);
		window.draw(vertices[2]);
		window.draw(vertices[3]);
		window.display();
	}
}


void SudokuWindow::onMouseButtonClicked(const sf::Vector2f& mousePosition) {
	int rowClicked = -1, colClicked = -1, valueClicked = -1;
	for (int i = 0; i < 81; ++i) {
		if (this->m_sudokuSquares[i]->getGlobalBounds().contains(mousePosition)) {
			rowClicked = i / 9; colClicked = i % 9; valueClicked = this->m_sudokuSquares[i]->getValue();
			this->m_clickedSquare = this->m_sudokuSquares[i];
			break;
		}
	}

	update(rowClicked, colClicked, valueClicked);
}

void SudokuWindow::onKeyButtonClicked(sf::Event::TextEvent text){
	if (m_gameFinished)
		return;
	if (this->m_clickedSquare == nullptr)
		return;

	int unicodeValue = text.unicode;
	if (unicodeValue < 49 || unicodeValue > 58)
		return;
	if (!this->m_clickedSquare->getValueConstant()) {
		this->m_clickedSquare->setValue(unicodeValue - '0');
		this->m_generator.setValueAt(this->m_clickedSquare->getRow(), this->m_clickedSquare->getCol(), unicodeValue - '0');
	}
	update(m_clickedSquare->getRow(), m_clickedSquare->getCol(), m_clickedSquare->getValue());
}

void SudokuWindow::update(int rowClicked, int colClicked, int valueClicked) {
	if (rowClicked == -1 || colClicked == -1)
		return;
	int gridY = rowClicked - rowClicked % 3, gridX = colClicked - colClicked % 3;
	for (int i = 0; i < 81; ++i) {
		int value = this->m_sudokuSquares[i]->getValue();
		int row = i / 9, col = i % 9;

		// Button that has been pressed
		if (row == rowClicked && col == colClicked) {
			this->m_sudokuSquares[i]->update(SudokuSquare::CLICKED);
			continue;
		}
		// Button in a same row, column or grid that has the same value as the clicked one
		if ((row == rowClicked || col == colClicked || (col - col % 3 == gridX && row - row % 3 == gridY)) && value == valueClicked && value != 0)
			this->m_sudokuSquares[i]->update(SudokuSquare::INCORRECT);
		else if (value == valueClicked && value > 0) // Button with the same value that isn't in the same row, column or grid
			this->m_sudokuSquares[i]->update(SudokuSquare::CLICKED);
		else if (row == rowClicked || col == colClicked || (col - col % 3 == gridX && row - row % 3 == gridY)) 
			this->m_sudokuSquares[i]->update(SudokuSquare::HOVERED);
		else this->m_sudokuSquares[i]->update(SudokuSquare::IDLE);
	}
	if(m_generator.isGameFinished()){
		m_gameFinished = true;
		std::cout << "Game finished\n";
	}
	
}

void SudokuWindow::createButtons(){
	sf::Font font;
	if(!font.loadFromFile("./Fonts/calibri.ttf")){
		std::cerr << "Couldn't open a file with font. Exiting.\n";
		getchar();
		exit(1);
	}
	auto board = m_generator.getBoard();
	for (int i = 0; i < 81; ++i) {
		this->m_sudokuSquares[i] = new SudokuSquare(i / 9, i % 9, (i % 9) * 50.0f, (i / 9) * 50.0f, 50, 50, font, "");
		int value = board[i / 9][i % 9];
		if(value != 0)
			this->m_sudokuSquares[i]->setValue(value);
		this->m_sudokuSquares[i]->setValueConstant(value != 0);
	}
}
