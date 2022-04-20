#pragma once
#include <random>
#include <vector>
class SudokuGenerator{

public:
	SudokuGenerator();
	std::vector<std::vector<int>> getBoard();
private:
	void removeNumbersFromBoard();
	int solveSudoku(int row, int col, int solutions);
	bool isSafe(int value, int row, int col);
	std::vector<int> generateRandomPermutation();

	std::vector<std::vector<int>> m_board;
	std::vector<std::vector<int>> m_solvedBoard;
	int m_missingValues;
	std::random_device m_rd;
};

