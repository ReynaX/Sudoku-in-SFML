#include "SudokuWindow.h"

#include <iostream>
#include "Line.h"

SudokuWindow::SudokuWindow(){
	m_generator = new SudokuGenerator();
	m_sudokuSquares.resize(81);
	m_clickedSquare = nullptr;
	m_gameFinished = false;

	m_clock = new sf::Clock();
	m_clockText.setFont(m_font);
	m_clockText.setString("00:00");
	m_clockText.setFillColor(sf::Color::Black);
	m_clockText.setCharacterSize(24);

	m_clockText.setPosition(
		450 - m_clockText.getGlobalBounds().width, 0
	);

	m_difficultyText.setFont(m_font);
	m_difficultyText.setString("Difficulty: Easy");
	m_difficultyText.setFillColor(sf::Color::Black);
	m_difficultyText.setCharacterSize(24);

	m_difficultyText.setPosition(
		0, 0
	);
	createButtons();
	eventHandler();
}

void SudokuWindow::eventHandler(){
	Line vertices[] = {
	Line(sf::Vector2f(150, 0 + m_difficultyText.getGlobalBounds().height), sf::Vector2f(150, 450 + m_difficultyText.getGlobalBounds().height)),
	Line(sf::Vector2f(300, 0 + m_difficultyText.getGlobalBounds().height), sf::Vector2f(300, 450 + m_difficultyText.getGlobalBounds().height)),
	Line(sf::Vector2f(0, 150 + m_difficultyText.getGlobalBounds().height), sf::Vector2f(450, 150 + m_difficultyText.getGlobalBounds().height)),
	Line(sf::Vector2f(0, 300 + m_difficultyText.getGlobalBounds().height), sf::Vector2f(450, 300 + m_difficultyText.getGlobalBounds().height))
	};
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8.0;
	sf::RenderWindow window(sf::VideoMode(625 , 450 + m_clockText.getGlobalBounds().height), "Sudoku v1.0", sf::Style::Close, settings);
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
		window.clear(sf::Color(255, 255, 255, 1));
			for (int i = 0; i < 81; ++i) {
				m_sudokuSquares[i]->draw(&window, sf::RenderStates::Default);
			}

			for (auto &v : vertices)
				window.draw(v);
		for (auto &button : m_menuButtons)
			button->draw(&window);
		updateClock();
		window.draw(m_clockText);
		window.draw(m_difficultyText);
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
					onNewGameButtonClicked();
					break;
				}
				case MenuButton::SOLVE_GAME:{
					onSolveButtonClicked();
					break;
				}
				case MenuButton::HINT:{
					onHintButtonClicked();
					break;
				}
			}
			break;
		}
	}
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

void SudokuWindow::onNewGameButtonClicked(){
	m_generator = new SudokuGenerator();
	// Reassign values of sudoku squares
	auto board = m_generator->getBoard();
	for (int i = 0; i < 81; ++i) {
		int value = board[i / 9][i % 9];
		m_sudokuSquares[i]->setValue(value);
		m_sudokuSquares[i]->setValueConstant(value != 0);
	}
	m_clickedSquare = nullptr;

	for(auto& square: m_sudokuSquares){
		square->update(SudokuSquare::IDLE);
	}
}

void SudokuWindow::onSolveButtonClicked(){
	m_generator->solve();
	auto board = m_generator->getBoard();
	for (int i = 0; i < 81; ++i) {
		int value = board[i / 9][i % 9];
		m_sudokuSquares[i]->setValue(value);
		m_sudokuSquares[i]->setValueConstant(value != 0);
	}
	if (m_clickedSquare)
		update(m_clickedSquare->getRow(), m_clickedSquare->getCol(), m_clickedSquare->getValue());
	m_gameFinished = true;
}
void SudokuWindow::onHintButtonClicked(){
	if (m_clickedSquare != nullptr) {
		int row = m_clickedSquare->getRow(), col = m_clickedSquare->getCol();
		m_generator->solveAt(row, col);
		int valueAt = m_generator->getBoard()[row][col];
		m_sudokuSquares[row * 9 + col]->setValue(valueAt);
		m_sudokuSquares[row * 9 + col]->setValueConstant(true);
		update(m_clickedSquare->getRow(), m_clickedSquare->getCol(), m_clickedSquare->getValue());
	}
}

void SudokuWindow::updateClock(){
	if (m_gameFinished)
		return;
	
	sf::Int32 miliseconds = m_clock->getElapsedTime().asMilliseconds();
	sf::Int32 seconds = (miliseconds / 1000) % 60;
	sf::Int32 minutes = (miliseconds / 1000) / 60;
	m_clockText.setString(((minutes < 10) ? "0" : "") + std::to_string(minutes) + ":" +
						  ((seconds < 10) ? "0" : "") + std::to_string(seconds));
}

void SudokuWindow::update(int rowClicked, int colClicked, int valueClicked) {
	if (m_gameFinished)
		return;
	
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
	if(!m_font.loadFromFile("./Fonts/calibri.ttf")){
		std::cerr << "Couldn't open a file with font. Exiting.\n";
		getchar();
		exit(1);
	}
	// Create and fill sudoku squares
	auto board = m_generator->getBoard();
	for (int i = 0; i < 81; ++i) {
		float f = m_difficultyText.getGlobalBounds().height;
		//std::cout << f << '\n';
		m_sudokuSquares[i] = new SudokuSquare(i / 9, i % 9, (i % 9) * 50.0f , (i / 9) * 50.0f + f + f / 3.f, 50, 50, m_font, "");
		int value = board[i / 9][i % 9];
		m_sudokuSquares[i]->setValue(value);
		m_sudokuSquares[i]->setValueConstant(value != 0);
	}

	// Create and assign functions to menu buttons
	m_menuButtons.push_back(new MenuButton(475, 25 + 40 * 0, 125, 40, m_font, "New Game", MenuButton::NEW_GAME));
	m_menuButtons.push_back(new MenuButton(475, 25 + 40 * 1, 125, 40, m_font, "Solve", MenuButton::SOLVE_GAME));
	m_menuButtons.push_back(new MenuButton(475, 25 + 40 * 2, 125, 40, m_font, "Hint", MenuButton::HINT));
}
