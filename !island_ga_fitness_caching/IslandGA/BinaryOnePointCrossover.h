#ifndef BINARY_ONE_POINT_CROSSOVER_H
#define BINARY_ONE_POINT_CROSSOVER_H

#include "BinaryCoding.h"
#include "Crossover.h"

class CBinaryOnePointCrossover : public CCrossover<CBinaryCoding>
{
public:
	CBinaryOnePointCrossover();
	CBinaryOnePointCrossover(float fProbability);

	virtual void vCrossover(CBinaryCoding *pcParent0, CBinaryCoding *pcParent1, CBinaryCoding *pcChild0, CBinaryCoding *pcChild1);
};//class CBinaryOnePointCrossover

#endif//BINARY_ONE_POINT_CROSSOVER_H