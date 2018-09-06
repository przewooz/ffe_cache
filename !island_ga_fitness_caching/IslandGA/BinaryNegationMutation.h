#ifndef BINARY_NEGATION_MUTATION_H
#define BINARY_NEGATION_MUTATION_H

#include "BinaryCoding.h"
#include "Mutation.h"

#include <cstdint>

using namespace std;

class CBinaryNegationMutation : public CMutation<CBinaryCoding>
{
public:
	CBinaryNegationMutation();
	CBinaryNegationMutation(float fProbability);

	virtual bool bMutate(CBinaryCoding *pcGenotype, uint32_t iIndex);
};//class CBinaryNegationMutation

#endif//BINARY_NEGATION_MUTATION_H