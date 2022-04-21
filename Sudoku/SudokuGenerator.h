#pragma once
#include <random>
#include <vector>
class SudokuGenerator{
public:
	SudokuGenerator();
	std::vector<std::vector<int>> getBoard();
	bool isGameFinished();
	void setValueAt(int row, int col, int value);
private:
	bool removeNumbersFromBoard();
	int solveSudoku(int row, int col, int solutions);
	bool isSafe(int value, int row, int col);
	std::vector<int> generateRandomPermutation();

	std::vector<std::vector<int>> m_board;
	std::vector<std::vector<int>> m_solvedBoard;
	int m_missingValues;

	// Random permutation generator
	std::random_device m_rd;
	std::mt19937 m_mt{m_rd()};
	std::uniform_int_distribution<int> m_dist{0,8};
};

