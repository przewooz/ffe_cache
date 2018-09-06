#ifndef BINARY_TWO_POINTS_CROSSOVER_H
#define BINARY_TWO_POINTS_CROSSOVER_H

#include "BinaryCoding.h"
#include "Crossover.h"

class CBinaryTwoPointsCrossover : public CCrossover<CBinaryCoding>
{
public:
	CBinaryTwoPointsCrossover();
	CBinaryTwoPointsCrossover(float fProbability);

	virtual void vCrossover(CBinaryCoding *pcParent0, CBinaryCoding *pcParent1, CBinaryCoding *pcChild0, CBinaryCoding *pcChild1);
};//class CBinaryTwoPointsCrossover

#endif//BINARY_TWO_POINTS_CROSSOVER_H