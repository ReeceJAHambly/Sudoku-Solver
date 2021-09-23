#pragma once

#include <iostream>
#include <fstream>
#include "CellGroup.h"
using namespace std;

class SudokuPuzzle
{
public:
	SudokuPuzzle(void);
	~SudokuPuzzle(void);

	enum class TripleContext {
		TripleBlock,
		TripleColumn,
		TripleRow
	};


	void Solve(char filenameIn[]);



private:
	CellGroup boardRows[9];
	CellGroup boardColumns[9];
	CellGroup boardBlocks[9];

	void Output() const;
	void readPuzzle(char filenameIn[]);

	int clCompareCount;

	void candidateListStart() const;
	void candidateListUpdate(TripleContext const &context, int contextIndex, int index, int value);
	void IncrementCandidateComparsionCount();
	bool hiddenSingles(CellGroup *context, TripleContext contextIdentifier);
	bool hiddenSingles();
};

