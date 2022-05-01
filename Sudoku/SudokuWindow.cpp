#include "SudokuWindow.h"

#include <iostream>
#include "Line.h"

SudokuWindow::SudokuWindow(){
	m_generator = new SudokuGenerator();
	m_sudokuSquares.resize(81);
	m_clickedSquare = nullptr;
	m_gameFinished = false;
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
	sf::RenderWindow window(sf::VideoMode(625, 480), "Sudoku v1.0", sf::Style::Close, settings);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			
			if (event.type == sf::Event::MouseMoved)
				onMouseMoved(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
			
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left)
					onMouseButtonClicked(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
			}

			if (event.type == sf::Event::KeyPressed){
				if (event.key.code == sf::Keyboard::BackSpace) {
					onBackspaceButtonClicked();
					continue;
				}
			}
			if (event.type == sf::Event::TextEntered)
				onKeyButtonClicked(event.text);
		}
		window.clear();
		for (int i = 0; i < 81; ++i) {
			m_sudokuSquares[i]->draw(&window, sf::RenderStates::Default);
		}
		window.draw(vertices[0]);
		window.draw(vertices[1]);
		window.draw(vertices[2]);
		window.draw(vertices[3]);
		m_menuButtons[0]->draw(&window);
		m_menuButtons[1]->draw(&window);
		m_menuButtons[2]->draw(&window);
		window.display();
	}
}

void SudokuWindow::onMouseButtonClicked(const sf::Vector2f& mousePosition) {
	int rowClicked = -1, colClicked = -1, valueClicked = -1;
	for (int i = 0; i < 81; ++i) {
		if (m_sudokuSquares[i]->getGlobalBounds().contains(mousePosition)) {
			rowClicked = i / 9; colClicked = i % 9; valueClicked = m_sudokuSquares[i]->getValue();
			m_clickedSquare = m_sudokuSquares[i];
			break;
		}
	}
	if (rowClicked != -1 && colClicked != -1) {
		update(rowClicked, colClicked, valueClicked);
		return;
	}
	
	for(auto& button: m_menuButtons){
		if(button->getGlobalBounds().contains(mousePosition)){
			switch(button->getFunction()){
			case MenuButton::NEW_GAME: {
				m_generator = new SudokuGenerator();
				// Reassign values of sudoku squares
				auto board = m_generator->getBoard();
				for (int i = 0; i < 81; ++i) {
					int value = board[i / 9][i % 9];
					m_sudokuSquares[i]->setValue(value);
					m_sudokuSquares[i]->setValueConstant(value != 0);
				}
				break;
			}
			case MenuButton::SOLVE_GAME:{
				m_generator->solve();
				auto board = m_generator->getBoard();
				for (int i = 0; i < 81; ++i) {
					int value = board[i / 9][i % 9];
					m_sudokuSquares[i]->setValue(value);
					m_sudokuSquares[i]->setValueConstant(value != 0);
				}
				break;
			}
			case MenuButton::HINT:{
				if (m_clickedSquare != nullptr) {
					m_generator->solveAt(m_clickedSquare->getRow(), m_clickedSquare->getCol());
					int valueAt = m_generator->getBoard()[m_clickedSquare->getRow()][m_clickedSquare->getCol()];
					m_sudokuSquares[m_clickedSquare->getRow()][m_clickedSquare->getCol()].setValue(valueAt);
					update(m_clickedSquare->getRow(), m_clickedSquare->getCol(), valueAt);
				}
				break;
			}
			}
			break;
		}
	}
	//update(0, 0, )
}

void SudokuWindow::onMouseMoved(const sf::Vector2f& mousePosition){
	for(auto& button: m_menuButtons){
		if (button->getGlobalBounds().contains(mousePosition))
			button->update(true);
		else button->update(false);
	}
}

void SudokuWindow::onKeyButtonClicked(sf::Event::TextEvent text){
	if (m_gameFinished)
		return;
	if (m_clickedSquare == nullptr)
		return;

	int unicodeValue = text.unicode;

	if (unicodeValue < 49 || unicodeValue > 58) // digit
		return;
	if (!m_clickedSquare->getValueConstant()) {
		m_clickedSquare->setValue(unicodeValue - '0');
		m_generator->setValueAt(m_clickedSquare->getRow(), m_clickedSquare->getCol(), unicodeValue - '0');
	}
	update(m_clickedSquare->getRow(), m_clickedSquare->getCol(), m_clickedSquare->getValue());
}

void SudokuWindow::onBackspaceButtonClicked(){
	if(m_clickedSquare != nullptr){
		if(!m_clickedSquare->getValueConstant()){
			m_clickedSquare->setValue(0);
			m_generator->setValueAt(m_clickedSquare->getRow(), m_clickedSquare->getCol(), 0);
			update(m_clickedSquare->getRow(), m_clickedSquare->getCol(), 0);
		}
	}
}

void SudokuWindow::update(int rowClicked, int colClicked, int valueClicked) {
	if (rowClicked == -1 || colClicked == -1)
		return;
	int gridY = rowClicked - rowClicked % 3, gridX = colClicked - colClicked % 3;
	for (int i = 0; i < 81; ++i) {
		int value = m_sudokuSquares[i]->getValue();
		int row = i / 9, col = i % 9;

		// Button that has been pressed
		if (row == rowClicked && col == colClicked) {
			m_sudokuSquares[i]->update(SudokuSquare::CLICKED);
			continue;
		}
		// Button in a same row, column or grid that has the same value as the clicked one
		if ((row == rowClicked || col == colClicked || (col - col % 3 == gridX && row - row % 3 == gridY)) && value == valueClicked && value != 0)
			m_sudokuSquares[i]->update(SudokuSquare::INCORRECT);
		else if (value == valueClicked && value > 0) // Button with the same value that isn't in the same row, column or grid
			m_sudokuSquares[i]->update(SudokuSquare::CLICKED);
		else if (row == rowClicked || col == colClicked || (col - col % 3 == gridX && row - row % 3 == gridY)) 
			m_sudokuSquares[i]->update(SudokuSquare::HOVERED);
		else m_sudokuSquares[i]->update(SudokuSquare::IDLE);
	}
	if(m_generator->isGameFinished()){
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
	// Create and fill sudoku squares
	auto board = m_generator->getBoard();
	for (int i = 0; i < 81; ++i) {
		m_sudokuSquares[i] = new SudokuSquare(i / 9, i % 9, (i % 9) * 50.0f, (i / 9) * 50.0f, 50, 50, font, "");
		int value = board[i / 9][i % 9];
		m_sudokuSquares[i]->setValue(value);
		m_sudokuSquares[i]->setValueConstant(value != 0);
	}

	// Create and assign functions to menu buttons
	m_menuButtons.push_back(new MenuButton(475, 25 + 40 * 0, 125, 40, font, "New Game", MenuButton::NEW_GAME));
	m_menuButtons.push_back(new MenuButton(475, 25 + 40 * 1, 125, 40, font, "Solve", MenuButton::SOLVE_GAME));
	m_menuButtons.push_back(new MenuButton(475, 25 + 40 * 2, 125, 40, font, "Hint", MenuButton::HINT));
}
