#include "BinaryOnePointCrossover.h"

#include "RandUtils.h"

#include <cstdint>

CBinaryOnePointCrossover::CBinaryOnePointCrossover()
{

}//CBinaryOnePointCrossover::CBinaryOnePointCrossover()

CBinaryOnePointCrossover::CBinaryOnePointCrossover(float fProbability)
	: CCrossover<CBinaryCoding>(fProbability)
{

}//CBinaryOnePointCrossover::CBinaryOnePointCrossover(float fProbability)

void CBinaryOnePointCrossover::vCrossover(CBinaryCoding *pcParent0, CBinaryCoding *pcParent1, CBinaryCoding *pcChild0, CBinaryCoding *pcChild1)
{
	uint16_t i_cut_index = (uint16_t)RandUtils::iRandIndex(pcParent0->iGetNumberOfBits());

	for (uint16_t i = 0; i < i_cut_index; i++)
	{
		*(pcChild0->piGetBits() + i) = *(pcParent0->piGetBits() + i);
		*(pcChild1->piGetBits() + i) = *(pcParent1->piGetBits() + i);
	}//for (uint16_t i = 0; i < i_cut_index; i++)

	for (uint16_t i = i_cut_index; i < pcParent0->iGetNumberOfBits(); i++)
	{
		*(pcChild0->piGetBits() + i) = *(pcParent1->piGetBits() + i);
		*(pcChild1->piGetBits() + i) = *(pcParent0->piGetBits() + i);
	}//for (uint16_t i = i_cut_index; i < pcParent0->iGetNumberOfBits(); i++)
}//void CBinaryOnePointCrossover::vCrossover(CBinaryCoding *pcParent0, CBinaryCoding *pcParent1, CBinaryCoding *pcChild0, CBinaryCoding *pcChild1)