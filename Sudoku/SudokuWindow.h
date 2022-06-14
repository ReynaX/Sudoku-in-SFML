#pragma once
#include "SudokuSquare.h"
class SudokuGenerator;
class DifficultyLevelButton;
class MenuButton;
/**
 * Dimensions for the main window
 * Size of sudoku board is height x height
 * Size of the window height is slightly bigger as its added to size of text on top of sudoko board
 */
namespace WindowConfig {
	const unsigned int width = 625;
	const unsigned int height = 450;
}

class SudokuWindow{
public:
	SudokuWindow();
	~SudokuWindow();
	/// Handles events(mouse, keyboard events)
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
	/// Update clock on screen
	void updateClock();
	/**
	 * Change state of square at given position.
	 * Method changes state of all same value squares and square in the same row, column and grid.
	 * Checks if sudoku has been solved.
	 *
	 * @param rowClicked row of square clicked on
	 * @param colClicked column of square clicked on
	 * @param valueClicked value that has been clicked on
	 */
	void update(int rowClicked, int colClicked, int valueClicked);
	 /// Makes a pseudo animation by changing states of squares in the right intervals.
	void onAnimationStageChanged();
	
	void updateAnimation(int offset, enum SudokuSquare::SquareState state);
	
	SudokuGenerator *m_generator;
	std::vector<SudokuSquare*> m_sudokuSquares; // Vector with squares
	std::vector<MenuButton*> m_menuButtons; // Vector with menu button(new game button, hint button, solution button)
	std::vector<DifficultyLevelButton*> m_difficultybuttons; // Vector with buttons to choose level difficulty
	SudokuSquare* m_clickedSquare; // Currently clicked square
	
	bool m_gameFinished; // Informs whether sudoku has been solved
	bool m_animationFinished; // Informs whether animation has finished
	int m_animationStage; // Indicates current phase of ending animation

	enum Difficulty{EASY = 0, MEDIUM, HARD};
	
	Difficulty m_difficultySelected; // difficulty selected

	sf::Font m_font;
	sf::Text m_clockText;
	sf::Text m_difficultyText;
	sf::Clock *m_clock; // Clock of the game
};

