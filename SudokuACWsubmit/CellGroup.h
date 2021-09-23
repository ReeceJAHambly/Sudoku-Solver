#pragma once
#include "CellObject.h"

//CellGroup class
class CellGroup
{
public:
	//Cellgroup default constructor + destructor
	CellGroup(void);
	~CellGroup(void);

	//cell set and get
	void setCell(int index, CellObject *cell);
	CellObject* getCell(int index) const;

private:
	CellObject* pointerArray[9];
};
