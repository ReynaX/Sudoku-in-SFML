#pragma once
#include <random>
#include <vector>
//TODO: Implement difficulty level
class SudokuGenerator{
public:
	SudokuGenerator(int difficultyLevel);
	
	std::vector<std::vector<int>> getBoard();
	bool isGameFinished();
	/// Set given value at given row and col
	void setValueAt(int row, int col, int value);
	/// Solve the board
	void solve();
	/// Set correct value at given row and col
	void solveAt(int row, int col);
private:
	/**
	 * Using backtracking removes values from squares so that sudoku remains valid(has only 1 solution)
	 *
	 * @param removeOrder numbers between 1-81 that will define order of removing numbers
	 * @param index points at current position in <code>removeOrder</code> vector
	 *
	 * @return if it's possible to remove given number of values from board in given order
	 */
	bool removeNumbersFromBoard(const std::vector<int>& removeOrder, int index);
	/**
	 * Using backtracking finds number of possible solutions.
	 * Method operates no original board, so making a copy of board before calling the method is recommended.
	 * 
	 * @param row row to start from
	 * @param col column to start from
	 * @param solutions number of currently found solutions
	 * 
	 * @return true, if it's possible to remove given number of values from board in given order
	 */
	int solveSudoku(int row = 0, int col = 0, int solutions = 0);
	/**
	 * Checks if inserting value at given position is valid
	 * 
	 * @param value number to be inserted
	 * @param row row of position to check
	 * @param col column of position to check
	 *
	 * @return true, if inserting value at given position doesn't break the rules of sudoku
	 */
	bool isSafe(int value, int row, int col);
	/**
	 * Generates random permutation of given size with values between <1; <code>vectorSize</code>>
	 *
	 * @param vectorSize maximum value of generated permutation
	 *
	 * @return generated permutation
	 */
	std::vector<int> generateRandomPermutation(int vectorSize);

	std::vector<std::vector<int>> m_board; // Matrix that contains current sudoku board
	std::vector<std::vector<int>> m_solvedBoard; // Matrix that contains solved sudoku board
	
	int m_missingValues; // Number of empty squares to generate in sudoku board
	int m_emptySquares;

	std::random_device m_rd;
	std::mt19937 m_mt{m_rd()};
	std::uniform_int_distribution<int> m_dist{0,8};
};

