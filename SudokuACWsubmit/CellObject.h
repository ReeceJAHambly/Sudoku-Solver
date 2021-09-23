#pragma once
#include <unordered_set>

//Cell class
class CellObject
{
public:
	//default constructor + destructor
	CellObject(void);
	~CellObject(void);

	//constructor
	CellObject(int value, bool given, std::unordered_set<int> const &candidateList);

	//value get and set
	void setValue(int inValue);
	int getValue() const;

	//given get and set
	void setKnown(bool inGiven);
	bool getKnown() const;

	//candidateList get and set
	std::unordered_set<int> const &getCandidateList() const;
	void setCandidateList(std::unordered_set<int> const &inCandidateList);

private:
	std::unordered_set<int> mCandidateList;
	int mValue;
	bool mKnown;
};