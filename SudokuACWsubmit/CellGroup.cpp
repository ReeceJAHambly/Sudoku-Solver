#include "CellObject.h"

//cell constructor
CellObject::CellObject(int value, bool given, std::unordered_set<int> const &candidateList) :
	mCandidateList(candidateList), mValue(value), mKnown(given) {}

//Default constructor + destructor
CellObject::CellObject() : mCandidateList(std::unordered_set<int>()), mValue(0), mKnown(false) {}
CellObject::~CellObject() {}

//value get and set
void CellObject::setValue(int inValue)
{
	mValue = inValue;
}
int CellObject::getValue() const
{
	return mValue;
}

//given get and set
void CellObject::setKnown(bool inGiven)
{
	mKnown = inGiven;
}
bool CellObject::getKnown() const
{
	return mKnown;
}

//candidatelist get and set
void CellObject::setCandidateList(std::unordered_set<int> const &inCandidateList)
{
	mCandidateList = inCandidateList;
}
const std::unordered_set<int> &CellObject::getCandidateList() const
{
	return mCandidateList;
}