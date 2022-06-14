#include "SudokuWindow.h"

#include <iostream>

#include "DifficultyLevelButton.h"
#include "Line.h"
#include "MenuButton.h"
#include "SudokuGenerator.h"

SudokuWindow::SudokuWindow(){
	m_generator = new SudokuGenerator(1);
	m_sudokuSquares.resize(81);
	m_clickedSquare = nullptr;
	m_gameFinished = false;

	m_clock = new sf::Clock();
	m_clockText.setFont(m_font);
	m_clockText.setString("00:00");
	m_clockText.setFillColor(sf::Color::Black);
	m_clockText.setCharacterSize(24);

	m_clockText.setPosition(
		WindowConfig::height - m_clockText.getString().getSize() / 2.f * m_clockText.getCharacterSize(), 0
	);

	m_difficultyText.setFont(m_font);
	m_difficultyText.setFillColor(sf::Color::Black);
	m_difficultyText.setCharacterSize(24);

	m_difficultyText.setPosition(
		0, 0
	);

	m_difficultySelected = MEDIUM;
	
	m_animationFinished = false;
	m_animationStage = 0;
	createButtons();
	eventHandler();
}

SudokuWindow::~SudokuWindow(){
	for (auto &button : m_menuButtons)
		delete button;
	for (auto &button : m_sudokuSquares)
		delete button;
	for (auto &button : m_difficultybuttons)
		delete button;

	delete m_generator;
	delete m_clock;
}

void SudokuWindow::eventHandler(){
	uint32_t cSize = m_difficultyText.getCharacterSize() + m_difficultyText.getCharacterSize() / 2;
	Line vertices[] = {
	Line(sf::Vector2f(WindowConfig::height / 3.f, 0 + cSize), sf::Vector2f(WindowConfig::height / 3.f, WindowConfig::height + cSize)),
	Line(sf::Vector2f(WindowConfig::height * 2.f / 3.f, 0 + cSize), sf::Vector2f(WindowConfig::height * 2.f / 3.f, WindowConfig::height + cSize)),
	Line(sf::Vector2f(0, WindowConfig::height / 3.f + cSize), sf::Vector2f(WindowConfig::height, WindowConfig::height / 3.f + cSize)),
	Line(sf::Vector2f(0, WindowConfig::height * 2.f / 3.f + cSize), sf::Vector2f(WindowConfig::height, WindowConfig::height * 2.f / 3.f + cSize))
	};
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8.0;
	sf::RenderWindow window(sf::VideoMode(WindowConfig::width , WindowConfig::height + cSize), "Sudoku v1.0", sf::Style::Close, settings);
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
		if (m_gameFinished && !m_animationFinished) {
			onAnimationStageChanged();
		}
		window.clear(sf::Color(255, 255, 255, 1));
		for (int i = 0; i < 81; ++i) {
			m_sudokuSquares[i]->draw(&window, sf::RenderStates::Default);
		}

		for (auto &v : vertices)
			window.draw(v);
		for (auto &button : m_menuButtons)
			button->draw(&window);
		for (auto &button : m_difficultybuttons)
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
			return;
		}
	}
	int buttonChecked = -1;
	for (int i = 0; i < m_difficultybuttons.size(); ++i) {
		if(m_difficultybuttons[i]->getGlobalBounds().contains(mousePosition)){
			buttonChecked = i;
			m_difficultySelected = static_cast<Difficulty>(buttonChecked);
			break;
		}
	}

	if (buttonChecked == -1)
		return;
	
	for(int i = 0; i < m_difficultybuttons.size(); ++i){
		m_difficultybuttons[i]->update(buttonChecked == i);
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
	if(m_clickedSquare != nullptr && !m_gameFinished){
		if(!m_clickedSquare->getValueConstant()){
			m_clickedSquare->setValue(0);
			m_generator->setValueAt(m_clickedSquare->getRow(), m_clickedSquare->getCol(), 0);
			update(m_clickedSquare->getRow(), m_clickedSquare->getCol(), 0);
		}
	}
}

void SudokuWindow::onNewGameButtonClicked(){
	m_generator = new SudokuGenerator(m_difficultySelected);
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
	m_gameFinished = false;
	m_animationStage = 0;
	m_animationFinished = false;
	m_clock->restart();
	std::string difficultyLevelText = (m_difficultySelected == 0 ? "Easy" : (m_difficultySelected == 1 ? "Medium" : "Hard"));
	m_difficultyText.setString("Difficulty: " + difficultyLevelText);
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
	if (rowClicked == -1 || colClicked == -1)
		return;

	if (m_gameFinished && !m_animationFinished)
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
		m_animationFinished = false;
		std::cout << "Game finished\n";
	}
}

void SudokuWindow::onAnimationStageChanged(){
	if (m_animationStage == 0) {
		++m_animationStage;
		m_sudokuSquares[4 * 9 + 9]->update(SudokuSquare::CLICKED);
		m_clock->restart();
		return;
	}
	if (m_animationStage > 4 && m_clock->getElapsedTime().asSeconds() >= 0.5f) {
		m_animationFinished = true;
		int offset = m_animationStage - 1;
		updateAnimation(offset, SudokuSquare::IDLE);
		return;
	}
	if(m_clock->getElapsedTime().asSeconds() >= 0.5f){
		int offset = m_animationStage - 1;
		updateAnimation(offset, SudokuSquare::IDLE);

		offset = m_animationStage;
		updateAnimation(offset, SudokuSquare::CLICKED);
		m_clock->restart();
		++m_animationStage;
	}
}

void SudokuWindow::updateAnimation(int offset, SudokuSquare::SquareState state){
	for (int i = -offset; i <= offset && offset != 0; i += offset * 2) {
		int centerX = 4, centerY = 4;
		for (int j = centerX - abs(offset); j <= abs(offset) + centerX; ++j)
			m_sudokuSquares[(centerY + i) * 9 + j]->update(state);

		for (int j = centerY - abs(offset); j <= abs(offset) + centerY; ++j)
			m_sudokuSquares[j * 9 + (centerX + i)]->update(state);
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
	uint32_t cSize = m_difficultyText.getCharacterSize() + m_difficultyText.getCharacterSize() / 2;
	for (int i = 0; i < 81; ++i) {
		//std::cout << f << '\n';
		m_sudokuSquares[i] = new SudokuSquare(i / 9, i % 9, (i % 9) * (WindowConfig::height / 9.f) , i / 9 * (WindowConfig::height / 9.f) + cSize,
			(WindowConfig::height / 9.f), (WindowConfig::height / 9.f), m_font, "");
		int value = board[i / 9][i % 9];
		m_sudokuSquares[i]->setValue(value);
		m_sudokuSquares[i]->setValueConstant(value != 0);
	}

	// Create and assign functions to menu buttons
	m_menuButtons.push_back(new MenuButton(475, 25 + 40 * 0 + cSize, 125, 40, m_font, "New Game", MenuButton::NEW_GAME));
	m_menuButtons.push_back(new MenuButton(475, 25 + 40 * 1 + cSize, 125, 40, m_font, "Solve", MenuButton::SOLVE_GAME));
	m_menuButtons.push_back(new MenuButton(475, 25 + 40 * 2 + cSize, 125, 40, m_font, "Hint", MenuButton::HINT));


	// Create difficulty level buttons
	std::vector<sf::Texture*> textures(3);
	std::string level[] = { "easy", "medium", "hard" };
	float radius = 16;
	
	for(int i = 0; i < 3; ++i){
		textures[i] = new sf::Texture();
		if(!textures[i]->loadFromFile("Resources/" + level[i] + "_level_icon.png")){
			std::cerr << "Can't read texture from file: " + level[i] + "_level_icon" << '\n';
			getchar();
			exit(1);
		}
		m_difficultybuttons.emplace_back(new DifficultyLevelButton(450 + radius * 2 * i + 20 *(i + 1), 185, radius, m_font, textures[i], i));
	}

	// Set default difficulty level to medium
	m_difficultybuttons[1]->update(true);
	m_difficultySelected = MEDIUM;
	m_difficultyText.setString("Difficulty: Medium");
}
