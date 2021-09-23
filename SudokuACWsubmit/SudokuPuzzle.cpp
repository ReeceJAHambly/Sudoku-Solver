#include "SudokuPuzzle.h"
#include <Windows.h>
#include <unordered_set>
#include <chrono>

using namespace std;

//Default constructor + destructor, sets the candidate list comparsion count to 0
SudokuPuzzle::SudokuPuzzle(void) : clCompareCount(0)
{
}

SudokuPuzzle::~SudokuPuzzle(void) {
}


void SudokuPuzzle::IncrementCandidateComparsionCount()
{
	++clCompareCount;
}


//Solve method
void SudokuPuzzle::Solve(char filenameIn[])
{

	readPuzzle(filenameIn);
	const auto startTime = chrono::high_resolution_clock::now();

	// Add code to solve the puzzle
	//Number of times the problem is ran through
	int numberOfRuns = 0;

	candidateListStart();
	do
	{
		numberOfRuns++;
	} while (hiddenSingles());

	// Get end time
	const auto endTime = chrono::high_resolution_clock::now();
	const auto duration = (endTime - startTime).count();
	double converter = 1000000; // Used to convert nanoseconds to milliseconds
	cout << "----------Software Performance----------" << endl;
	cout << "iterations " << numberOfRuns * 3 << endl; // *3 as the grid is ran through that many time
	cout << "Solve time: " << duration << endl;
	cout << "Solve time (ms): " << duration / converter << endl;
	cout << clCompareCount << endl;
	cout << "----------------------------------------" << endl;
	Output();  // Output the solved puzzle
}

//Finds naked singles by creating candidate lists 
void SudokuPuzzle::candidateListStart() const
{
	//for each row
	for (int row = 0; row < 9; row++)
	{
		//for each column
		for (int column = 0; column < 9; column++)
		{
			//initialising presentValue and candidateList sets
			unordered_set<int> knownValues;
			unordered_set<int> candidateList;

			//creating new cell for given column and row
			CellObject* cell = boardRows[row].getCell(column);

			//getting a cells block from row and column
			int block = ((row / 3) * 3) + column / 3;

			//creating candidateList for cells that have not been populated
			if (cell->getValue() == 0)
			{
				for (int i = 0; i < 9; i++)
				{
					//Adding values to list of present values
					knownValues.insert((boardBlocks[block].getCell(i))->getValue());
					knownValues.insert((boardColumns[column].getCell(i))->getValue());
					knownValues.insert((boardRows[row].getCell(i))->getValue());
				}
				for (int i = 0; i <= 9; i++)
				{
					//adding all values not in present values to candidate list
					if (knownValues.find(i) == knownValues.end())
					{
						candidateList.insert(i);
					}
				}
				//setting candidate list of specified cell
				cell->setCandidateList(candidateList);
			}
		}
	}
}

//Finds hidden singles
bool SudokuPuzzle::hiddenSingles(CellGroup *context, TripleContext contextIdentifier)
{
	bool updated = false;
	unordered_set<int> candidateList;

	for (int contextIndex = 0; contextIndex < 9; contextIndex++)
	{
		for (int index = 0; index < 9; index++)
		{
			CellObject* cell = context[contextIndex].getCell(index);

			int occurances[10] = { 0 };

			if (cell->getValue() == 0)
			{
				for (int i = 0; i < 9; i++)
				{
					CellObject *workingCell = context[contextIndex].getCell(i);

					candidateList = workingCell->getCandidateList();

					IncrementCandidateComparsionCount();

					//Setting the naked singles
					if (candidateList.size() == 1)
					{
						IncrementCandidateComparsionCount();
						workingCell->setValue(static_cast<int>(*candidateList.begin()));
						candidateListUpdate(contextIdentifier, contextIndex, i, workingCell->getValue());
					}

					else
					{
						for (auto f : candidateList)
						{
							IncrementCandidateComparsionCount();
							//++occurances[f];
						}
					}
				}

#ifdef _DEBUG					
				for (int i = 1; i <= 9; i++)
				{
					cout << "occurances: " << i << occurances[i] << endl;
				}
#endif // _DEBUG


				for (int valueIndex = 1; valueIndex < 10; valueIndex++)
				{
					for (int i = 0; i < 9; i++)
					{
						unordered_set<int> workingCandidateList = context[contextIndex].getCell(i)->getCandidateList();

						IncrementCandidateComparsionCount();
						if (workingCandidateList.find(valueIndex) != workingCandidateList.end())
						{
#ifdef _DEBUG	
							int row = 0;
							int	column = 0;
							int block = 0;

							if (contextIdentifier == TripleContext::TripleRow)
							{
								row = contextIndex;
								column = i;
							}
							else if (contextIdentifier == TripleContext::TripleColumn)
							{
								row = i;
								column = contextIndex;
							}
							else if (contextIdentifier == TripleContext::TripleBlock)
							{
								block = contextIndex;

								int rowOffset = (block / 3) * 3;
								int colOffset = (block % 3) * 3;
								int localRow = i / 3;
								int localCol = i % 3;

								row = rowOffset + localRow;
								column = colOffset + localCol;
							}
							cout << "setting (row: " << row << ", column: " << column << "): " << valueIndex << endl;
#endif // _DEBUG

							context[contextIndex].getCell(i)->setValue(valueIndex);
							context[contextIndex].getCell(i)->setCandidateList(unordered_set<int>());

							candidateListUpdate(contextIdentifier, contextIndex, i, valueIndex);

							updated = true;
						}
					}
				}
			}
		}
	}

	if (updated)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Hidden singles overload
bool SudokuPuzzle::hiddenSingles()
{
	//Searches the triple context for hidden singles
	bool updatedRow = hiddenSingles(boardRows, TripleContext::TripleRow);
	bool updatedColumn = hiddenSingles(boardColumns, TripleContext::TripleColumn);
	bool updatedBlock = hiddenSingles(boardBlocks, TripleContext::TripleBlock);


	return updatedRow || updatedColumn || updatedBlock;
}

//Updates the candidate lists for a given cells row, block and column
void SudokuPuzzle::candidateListUpdate(TripleContext const &context, int contextIndex, int index, int value)
{
	int row = 0;
	int	column = 0;
	int	block = 0;

	if (context == TripleContext::TripleRow)
	{
		row = contextIndex;
		column = index;
		block = ((row / 3) * 3) + column / 3;

	}
	else if (context == TripleContext::TripleColumn)
	{
		row = index;
		column = contextIndex;
		block = ((row / 3) * 3) + column / 3;
	}
	else if (context == TripleContext::TripleBlock)
	{
		block = contextIndex;

		int rowOffset = (block / 3) * 3;
		int colOffset = (block % 3) * 3;
		int localRow = index / 3;
		int localCol = index % 3;

		row = rowOffset + localRow;
		column = colOffset + localCol;
	}

	for (int i = 0; i < 9; i++)
	{
		unordered_set<int> workingRowCandidateList = boardRows[row].getCell(i)->getCandidateList();
		unordered_set<int> workingColumnCandidateList = boardColumns[column].getCell(i)->getCandidateList();
		unordered_set<int> workingBlockCandidateList = boardBlocks[block].getCell(i)->getCandidateList();

		if (workingRowCandidateList.find(value) != workingRowCandidateList.end())
		{
			workingRowCandidateList.erase(value);
			boardRows[row].getCell(i)->setCandidateList(workingRowCandidateList);
		}

		if (workingColumnCandidateList.find(value) != workingColumnCandidateList.end())
		{
			workingColumnCandidateList.erase(value);
			boardColumns[column].getCell(i)->setCandidateList(workingColumnCandidateList);
		}

		if (workingBlockCandidateList.find(value) != workingBlockCandidateList.end())
		{
			workingBlockCandidateList.erase(value);
			boardBlocks[block].getCell(i)->setCandidateList(workingBlockCandidateList);
		}
	}
}

//Output solved puzzle
void SudokuPuzzle::Output() const
{
	ofstream fout("sudoku_solution.txt"); // DO NOT CHANGE THE NAME OF THIS FILE
	if (fout.is_open())
	{
		//looping through all cells
		for (int y = 0; y < 9; ++y)
		{
			for (int x = 0; x < 9; ++x)
			{
				//getting value of cell
				CellObject* cell = boardRows[y].getCell(x);
				int value = cell->getValue();

				//outputing solved values of solved puzzle
				fout << value << " ";
			}

			fout << endl;
		}
		fout.close();
	}
}

//Function used to read in the puzzle file
void SudokuPuzzle::readPuzzle(char filenameIn[])
{
	//creates an ifstream with the filename given to the function 
	ifstream fin(filenameIn);
	//initialises the cell value to be zero
	int value = 0;
	//unordered set for the candidate lists so numbers can be added and removed
	unordered_set<int> candidateList;
	//variable to describe if the value is known or not
	bool known;

	//number of known values at the start of the problem
	int KnownValueCounter = 0;

	//for each row of the puzzle
	for (int row = 0; row < 9; ++row)
	{
		//for each column of the puzzle 
		for (int column = 0; column < 9; ++column)
		{
			//reading in each value 
			fin >> value;
			known = false;

			if (value != 0)
			{
				//Increases the counter as the value is known
				KnownValueCounter++;

				//sets known to true if the value isn't zero
				known = true;
			}

			//Creates the cell with a value, if its known and an empty candidate list
			CellObject* cell = new CellObject(value, known, candidateList);
			//Sets the cell of the cellgroup boardRows to the newly created cell
			boardRows[row].setCell(column, cell);
			//Sets the cell of the cellgroup boardColumns to the newly created cell
			boardColumns[column].setCell(row, cell);
		}
	}



	//variables used for indexing the blocks
	int cellIndex = 0;
	int blockIndex = 0;

	//for each block row, moves 3 along the puzzle
	for (int blockY = 0; blockY <= 6; blockY += 3)
	{
		//for each block column, moves 3 along the puzzle
		for (int blockX = 0; blockX <= 6; blockX += 3)
		{
			//for each cell row, each block is 3x3
			for (int cellY = 0; cellY < 3; cellY++)
			{
				//for each cell column, each block is 3x3
				for (int cellX = 0; cellX < 3; cellX++)
				{
					//creating new cell from the given block and cell
					CellObject* cell = boardRows[blockY + cellY].getCell(blockX + cellX);
					//adding cell to gridBlocks
					boardBlocks[blockIndex].setCell(cellIndex, cell);
					++cellIndex;
				}
			}
			cellIndex = 0;
			++blockIndex;
		}
	}
	//Closes the file
	fin.close();

	//Outputs the the amount of starting values
	cout << "Number of starting values : " << KnownValueCounter;
	cout << endl;
}