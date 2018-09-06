#include "BinaryUniformCrossover.h"

#include "RandUtils.h"

#include <cstdint>

CBinaryUniformCrossover::CBinaryUniformCrossover()
{

}//CBinaryUniformCrossover::CBinaryUniformCrossover()

CBinaryUniformCrossover::CBinaryUniformCrossover(float fProbability)
	: CCrossover<CBinaryCoding>(fProbability)
{

}//CBinaryUniformCrossover::CBinaryUniformCrossover(float fProbability)

void CBinaryUniformCrossover::vCrossover(CBinaryCoding *pcParent0, CBinaryCoding *pcParent1, CBinaryCoding *pcChild0, CBinaryCoding *pcChild1)
{
	for (uint16_t i = 0; i < pcParent0->iGetNumberOfBits(); i++)
	{
		if (RandUtils::bSatisfyProbability(0.5))
		{
			*(pcChild0->piGetBits() + i) = *(pcParent0->piGetBits() + i);
			*(pcChild1->piGetBits() + i) = *(pcParent1->piGetBits() + i);
		}//if (RandUtils::bSatisfyProbability(0.5))
		else
		{
			*(pcChild0->piGetBits() + i) = *(pcParent1->piGetBits() + i);
			*(pcChild1->piGetBits() + i) = *(pcParent0->piGetBits() + i);
		}//else if (RandUtils::bSatisfyProbability(0.5))
	}//for (uint16_t i = 0; i < pcParent0->iGetNumberOfBits(); i++)
}//void CBinaryUniformCrossover::vCrossover(CBinaryCoding *pcParent0, CBinaryCoding *pcParent1, CBinaryCoding *pcChild0, CBinaryCoding *pcChild1)