#ifndef REAL_CROSSOVER_H
#define REAL_CROSSOVER_H

#include "Crossover.h"
#include "Error.h"
#include "RealCoding.h"

#include <istream>

using namespace std;

class CRealLinearCrossover : public CCrossover<CRealCoding>
{
public:
	CRealLinearCrossover();
	CRealLinearCrossover(float fProbability);

	virtual void vCrossover(CRealCoding *pcParent0, CRealCoding *pcParent1, CRealCoding *pcChild0, CRealCoding *pcChild1);
};//class CRealLinearCrossover


class CRealBinCrossover : public CCrossover<CRealCoding>
{
public:
	CRealBinCrossover();
	CRealBinCrossover(float fProbability, float fCrossoverConstant);

	virtual CError eConfigure(istream *psSettings);

	virtual void vCrossover(CRealCoding *pcParent0, CRealCoding *pcParent1, CRealCoding *pcChild0, CRealCoding *pcChild1);

private:
	float f_crossover_constant;
};//class CRealBinCrossover : public CCrossover<CRealCoding>

#endif//REAL_CROSSOVER_H