#include "RealCrossover.h"

#include "FloatCommandParam.h"
#include "RandUtils.h"

#include <cstdint>

CRealLinearCrossover::CRealLinearCrossover()
{

}//CRealLinearCrossover::CRealLinearCrossover()

CRealLinearCrossover::CRealLinearCrossover(float fProbability)
	: CCrossover<CRealCoding>(fProbability)
{

}//CRealLinearCrossover::CRealLinearCrossover(float fProbability)

void CRealLinearCrossover::vCrossover(CRealCoding *pcParent0, CRealCoding *pcParent1, CRealCoding *pcChild0, CRealCoding *pcChild1)
{
	double d_ratio = (double)RandUtils::fRandProbability();

	for (uint16_t i = 0; i < pcParent0->iGetNumberOfDimensions(); i++)
	{
		*(pcChild0->pdGetValues() + i) = d_ratio * *(pcParent0->pdGetValues() + i) + (1 - d_ratio) * *(pcParent1->pdGetValues() + i);
		*(pcChild1->pdGetValues() + i) = d_ratio * *(pcParent1->pdGetValues() + i) + (1 - d_ratio) * *(pcParent0->pdGetValues() + i);
	}//for (uint16_t i = 0; i < pcParent0->iGetNumberOfDimensions(); i++)
}//void CRealLinearCrossover::vCrossover(CRealCoding *pcParent0, CRealCoding *pcParent1, CRealCoding *pcChild0, CRealCoding *pcChild1)


CRealBinCrossover::CRealBinCrossover()
{

}//CRealBinCrossover::CRealBinCrossover()

CRealBinCrossover::CRealBinCrossover(float fProbability, float fCrossoverConstant)
	: CCrossover<CRealCoding>(fProbability)
{
	f_crossover_constant = fCrossoverConstant;
}//CRealLinearCrossover::CRealLinearCrossover(float fProbability, float fCrossoverConstant)

CError CRealBinCrossover::eConfigure(istream *psSettings)
{
	CError c_error = CCrossover<CRealCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		CFloatCommandParam p_crossover_constant(CROSSOVER_ARGUMENT_CONSTANT, DEF_PROBABILITY_MIN, DEF_PROBABILITY_MAX);
		f_crossover_constant = p_crossover_constant.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CRealBinCrossover::eConfigure(istream *psSettings)

void CRealBinCrossover::vCrossover(CRealCoding *pcParent0, CRealCoding *pcParent1, CRealCoding *pcChild0, CRealCoding *pcChild1)
{
	uint16_t i_number_of_dimensions = pcParent0->iGetNumberOfDimensions();
	uint16_t i_random_index = (uint16_t)RandUtils::iRandIndex(i_number_of_dimensions);

	*(pcChild0->pdGetValues() + i_random_index) = *(pcParent1->pdGetValues() + i_random_index);

	uint16_t i_index;

	for (uint16_t i = 1; i < i_number_of_dimensions; i++)
	{
		i_index = (i_random_index + i) % i_number_of_dimensions;

		if (RandUtils::fRandProbability() <= f_crossover_constant)
		{
			*(pcChild0->pdGetValues() + i_index) = *(pcParent1->pdGetValues() + i_index);
		}//if (RandUtils::fRandProbability() <= f_crossover_constant)
		else
		{
			*(pcChild0->pdGetValues() + i_index) = *(pcParent0->pdGetValues() + i_index);
		}//else if (RandUtils::fRandProbability() <= f_crossover_constant)
	}//for (uint16_t i = 1; i < i_number_of_dimensions; i++)
}//void CRealBinCrossover::vCrossover(CRealCoding *pcParent0, CRealCoding *pcParent1, CRealCoding *pcChild0, CRealCoding *pcChild1)