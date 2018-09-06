#include "BinaryTwoPointsCrossover.h"

#include "RandUtils.h"

#include <cmath>
#include <cstdint>

CBinaryTwoPointsCrossover::CBinaryTwoPointsCrossover()
{

}//CBinaryTwoPointsCrossover::CBinaryTwoPointsCrossover()

CBinaryTwoPointsCrossover::CBinaryTwoPointsCrossover(float fProbability)
	: CCrossover<CBinaryCoding>(fProbability)
{

}//CBinaryTwoPointsCrossover::CBinaryTwoPointsCrossover(float fProbability)

void CBinaryTwoPointsCrossover::vCrossover(CBinaryCoding *pcParent0, CBinaryCoding *pcParent1, CBinaryCoding *pcChild0, CBinaryCoding *pcChild1)
{
	uint16_t i_cut_index_0 = (uint16_t)RandUtils::iRandIndex(pcParent0->iGetNumberOfBits());
	uint16_t i_cut_index_1 = (uint16_t)RandUtils::iRandIndex(pcParent0->iGetNumberOfBits());

	uint16_t i_left_cut_index, i_right_cut_index;

	if (i_cut_index_0 < i_cut_index_1)
	{
		i_left_cut_index = i_cut_index_0;
		i_right_cut_index = i_cut_index_1;
	}//if (i_cut_index_0 < i_cut_index_1)
	else
	{
		i_left_cut_index = i_cut_index_1;
		i_right_cut_index = i_cut_index_0;
	}//else if (i_cut_index_0 < i_cut_index_1)

	for (uint16_t i = 0; i < i_left_cut_index; i++)
	{
		*(pcChild0->piGetBits() + i) = *(pcParent0->piGetBits() + i);
		*(pcChild1->piGetBits() + i) = *(pcParent1->piGetBits() + i);
	}//for (uint16_t i = 0; i < i_left_cut_index; i++)

	for (uint16_t i = i_left_cut_index; i < i_right_cut_index; i++)
	{
		*(pcChild0->piGetBits() + i) = *(pcParent1->piGetBits() + i);
		*(pcChild1->piGetBits() + i) = *(pcParent0->piGetBits() + i);
	}//for (uint16_t i = i_left_cut_index; i < i_right_cut_index; i++)

	for (uint16_t i = i_right_cut_index; i < pcParent0->iGetNumberOfBits(); i++)
	{
		*(pcChild0->piGetBits() + i) = *(pcParent0->piGetBits() + i);
		*(pcChild1->piGetBits() + i) = *(pcParent1->piGetBits() + i);
	}//for (uint16_t i = i_right_cut_index; i < pcParent0->iGetNumberOfBits(); i++)
}//void CBinaryTwoPointsCrossover::vCrossover(CBinaryCoding *pcParent0, CBinaryCoding *pcParent1, CBinaryCoding *pcChild0, CBinaryCoding *pcChild1)