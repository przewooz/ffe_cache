#ifndef BINARY_UNIFORM_CROSSOVER_H
#define BINARY_UNIFORM_CROSSOVER_H

#include "BinaryCoding.h"
#include "Crossover.h"

class CBinaryUniformCrossover : public CCrossover<CBinaryCoding>
{
public:
	CBinaryUniformCrossover();
	CBinaryUniformCrossover(float fProbability);

	virtual void vCrossover(CBinaryCoding *pcParent0, CBinaryCoding *pcParent1, CBinaryCoding *pcChild0, CBinaryCoding *pcChild1);
};//class CBinaryUniformCrossover

#endif//BINARY_UNIFORM_CROSSOVER_H