#include "BinaryDifferentialLinkage.h"

#include <cstdint>
#include <vector>

using namespace std;

#include <iostream>

CGenePattern * CBinaryDifferentialLinkage::pcGetGenePattern(CBinaryCoding *pcGenotype0, CBinaryCoding *pcGenotype1)
{
	CGenePattern *pc_pattern = new CGenePattern();

	//cout << "BEGIN" << endl;

	//cout << pcGenotype0 << endl << endl;
	//cout << pcGenotype1 << endl << endl;

	for (uint16_t i = 0; i < pcGenotype0->iGetNumberOfBits(); i++)
	{
		if (*(pcGenotype0->piGetBits() + i) != *(pcGenotype1->piGetBits() + i))
		{
			pc_pattern->vAdd(i);
			//cout << i << " ";
		}//if (*(pcGenotype0->piGetBits() + i) != *(pcGenotype1->piGetBits() + i))
	}//for (uint16_t i = 0; i < pcGenotype0->iGetNumberOfBits(); i++)

	//cout << endl;

	//cout << endl;

	//cout << "END" << endl;

	//cout << endl;

	return pc_pattern;
}//CGenePattern * CBinaryDifferentialLinkage::pcGetGenePattern(CBinaryCoding *pcGenotype0, CBinaryCoding *pcGenotype1)