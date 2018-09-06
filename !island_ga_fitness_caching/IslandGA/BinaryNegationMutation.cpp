#include "BinaryNegationMutation.h"

CBinaryNegationMutation::CBinaryNegationMutation()
	: CMutation<CBinaryCoding>(false, false, false)
{

}//CBinaryNegationMutation::CBinaryNegationMutation()

CBinaryNegationMutation::CBinaryNegationMutation(float fProbability)
	: CMutation<CBinaryCoding>(fProbability, false, false, false)
{

}//CBinaryNegationMutation::CBinaryNegationMutation(float fProbability)

bool CBinaryNegationMutation::bMutate(CBinaryCoding *pcGenotype, uint32_t iIndex)
{
	bool b_mutated = false;

	for (uint16_t i = 0; i < pcGenotype->iGetNumberOfBits(); i++)
	{
		if (b_apply_mutation(pcGenotype))
		{
			*(pcGenotype->piGetBits() + i) ^= 1;
			b_mutated = true;
		}//if (b_apply_mutation(pcGenotype))
	}//for (uint16_t i = 0; i < pcGenotype->iGetNumberOfBits(); i++)

	return b_mutated;
}//bool CBinaryNegationMutation::bMutate(CBinaryCoding *pcGenotype, uint32_t iIndex)