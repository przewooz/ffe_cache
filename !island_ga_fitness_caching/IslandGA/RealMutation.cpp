#include "RealMutation.h"

#include "FloatCommandParam.h"
#include "RandUtils.h"

#include <cfloat>
#include <unordered_set>

CRealMutation::CRealMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)
	: CMutation<CRealCoding>(bPopulationRequired, bBestGenotypeRequired, bBestGenotypesRequired)
{

}//CRealMutation::CRealMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)

CRealMutation::CRealMutation(float fProbability, bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)
	: CMutation<CRealCoding>(fProbability, bPopulationRequired, bBestGenotypeRequired, bBestGenotypesRequired)
{

}//CRealMutation::CRealMutation(float fProbability, bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)


CRealRandomMutation::CRealRandomMutation()
	: CRealMutation(false, false, false)
{

}//CRealRandomMutation::CRealRandomMutation()

CRealRandomMutation::CRealRandomMutation(float fProbability)
	: CRealMutation(fProbability, false, false, false)
{

}//CRealRandomMutation::CRealRandomMutation(float fProbability)

bool CRealRandomMutation::bMutate(CRealCoding *pcGenotype, uint32_t iIndex)
{
	bool b_mutate = b_apply_mutation(pcGenotype);

	if (b_mutate)
	{
		uint16_t i_rand_index = (uint16_t)RandUtils::iRandIndex((uint32_t)pcGenotype->iGetNumberOfDimensions());
		*(pcGenotype->pdGetValues() + i_rand_index) = RandUtils::dRandNumber(*(pcGenotype->pdGetMinValues() + i_rand_index),
			*(pcGenotype->pdGetMaxValues() + i_rand_index));
	}//if (b_mutate)

	return b_mutate;
}//void CRealRandomMutation::vMutate(CRealCoding *pcGenotype, uint32_t iIndex)


CRealFactorMutation::CRealFactorMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)
	: CRealMutation(bPopulationRequired, bBestGenotypeRequired, bBestGenotypesRequired)
{

}//CRealFactorMutation::CRealFactorMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)

CRealFactorMutation::CRealFactorMutation(float fProbability, double dFactor, bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)
	: CRealMutation(fProbability, bPopulationRequired, bBestGenotypeRequired, bBestGenotypesRequired)
{
	d_factor = dFactor;
}//CRealFactorMutation::CRealFactorMutation(float fProbability, double dFactor, bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)

CError CRealFactorMutation::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)
{
	CError c_error = CRealMutation::eConfigure(plGenePatterns, psSettings);

	if (!c_error)
	{
		CFloatCommandParam p_factor(MUTATION_ARGUMENT_FACTOR, DEF_PROBABILITY_MIN, DEF_PROBABILITY_MAX);
		d_factor = (double)p_factor.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CRealFactorMutation::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)


CRealTargetToBestMutation::CRealTargetToBestMutation()
	: CRealFactorMutation(false, true, false)
{

}//CRealTargetToBestMutation::CRealTargetToBestMutation()

CRealTargetToBestMutation::CRealTargetToBestMutation(float fProbability, double dFactor)
	: CRealFactorMutation(fProbability, dFactor, false, true, false)
{

}//CRealTargetToBestMutation::CRealTargetToBestMutation(float fProbability, double dFactor)

bool CRealTargetToBestMutation::bMutate(CRealCoding *pcGenotype, uint32_t iIndex)
{
	bool b_mutated = b_apply_mutation(pcGenotype);

	if (b_mutated)
	{
		for (uint16_t i = 0; i < pcGenotype->iGetNumberOfDimensions(); i++)
		{
			*(pcGenotype->pdGetValues() + i) += d_factor * (*(pc_best_genotype->pdGetValues() + i) - *(pcGenotype->pdGetValues() + i));
		}//for (uint16_t i = 0; i < pcGenotype->iGetNumberOfDimensions(); i++)
	}//if (b_mutated)

	return b_mutated;
}//bool CRealTargetToBestMutation::bMutate(CRealCoding *pcGenotype, uint32_t iIndex)


CRealRand1Mutation::CRealRand1Mutation()
	: CRealFactorMutation(true, false, false)
{

}//CRealRand1Mutation::CRealRand1Mutation()

CRealRand1Mutation::CRealRand1Mutation(float fProbability, double dFactor)
	: CRealFactorMutation(fProbability, dFactor, true, false, false)
{

}//CRealRand1Mutation::CRealRand1Mutation(float fProbability, double dFactor)

bool CRealRand1Mutation::bMutate(CRealCoding *pcGenotype, uint32_t iIndex)
{
	bool b_mutated = b_apply_mutation(pcGenotype);

	if (b_mutated)
	{
		unordered_set<uint32_t> s_indexes(REAL_MUTATION_RAND_1_NUMBER_OF_RANDOM_INDEXES + 1);
		s_indexes.insert(iIndex);

		uint32_t pi_rand_indexes[REAL_MUTATION_RAND_1_NUMBER_OF_RANDOM_INDEXES];

		for (uint32_t i = 0; i < REAL_MUTATION_RAND_1_NUMBER_OF_RANDOM_INDEXES; i++)
		{
			*(pi_rand_indexes + i) = RandUtils::iRandUniqueIndex(i_population_size, &s_indexes);
			s_indexes.insert(*(pi_rand_indexes + i));
		}//for (uint32_t i = 0; i < REAL_MUTATION_RAND_1_NUMBER_OF_RANDOM_INDEXES; i++)

		CRealCoding *pc_rand_genotype_0 = *(ppc_population + *(pi_rand_indexes + 0));
		CRealCoding *pc_rand_genotype_1 = *(ppc_population + *(pi_rand_indexes + 1));
		CRealCoding *pc_rand_genotype_2 = *(ppc_population + *(pi_rand_indexes + 2));

		for (uint16_t i = 0; i < pcGenotype->iGetNumberOfDimensions(); i++)
		{			
			*(pcGenotype->pdGetValues() + i) = (*(pc_rand_genotype_0->pdGetValues() + i)) + 
				d_factor * (*(pc_rand_genotype_1->pdGetValues() + i) - *(pc_rand_genotype_2->pdGetValues() + i));
		}//for (uint16_t i = 0; i < pcGenotype->iGetNumberOfDimensions(); i++)

		pcGenotype->vRepair();
	}//if (b_mutated)

	return b_mutated;
}//bool CRealRand1Mutation::bMutate(CRealCoding *pcGenotype, uint32_t iIndex)


CRealNRand1Mutation::CRealNRand1Mutation()
	: CRealFactorMutation(true, false, false)
{

}//CRealNRand1Mutation::CRealNRand1Mutation()

CRealNRand1Mutation::CRealNRand1Mutation(float fProbability, double dFactor)
	: CRealFactorMutation(fProbability, dFactor, true, false, false)
{

}//CRealNRand1Mutation::CRealNRand1Mutation(float fProbability, double dFactor)

bool CRealNRand1Mutation::bMutate(CRealCoding *pcGenotype, uint32_t iIndex)
{
	bool b_mutated = b_apply_mutation(pcGenotype);

	if (b_mutated)
	{
		unordered_set<uint32_t> s_indexes(REAL_MUTATION_NRAND_1_NUMBER_OF_RANDOM_INDEXES + 1);
		s_indexes.insert(iIndex);

		uint32_t pi_rand_indexes[REAL_MUTATION_NRAND_1_NUMBER_OF_RANDOM_INDEXES];

		for (uint32_t i = 0; i < REAL_MUTATION_NRAND_1_NUMBER_OF_RANDOM_INDEXES; i++)
		{
			*(pi_rand_indexes + i) = RandUtils::iRandUniqueIndex(i_population_size, &s_indexes);
			s_indexes.insert(*(pi_rand_indexes + i));
		}//for (uint32_t i = 0; i < REAL_MUTATION_RAND_1_NUMBER_OF_RANDOM_INDEXES; i++)

		CRealCoding *pc_nearest_genotype = *(ppc_population + i_get_nearest_index(pcGenotype, iIndex));
		CRealCoding *pc_rand_genotype_0 = *(ppc_population + *(pi_rand_indexes + 0));
		CRealCoding *pc_rand_genotype_1 = *(ppc_population + *(pi_rand_indexes + 1));

		for (uint16_t i = 0; i < pcGenotype->iGetNumberOfDimensions(); i++)
		{
			*(pcGenotype->pdGetValues() + i) = (*(pc_nearest_genotype->pdGetValues() + i)) +
				d_factor * (*(pc_rand_genotype_0->pdGetValues() + i) - *(pc_rand_genotype_1->pdGetValues() + i));
		}//for (uint16_t i = 0; i < pcGenotype->iGetNumberOfDimensions(); i++)

		pcGenotype->vRepair();
	}//if (b_mutated)

	return b_mutated;
}//bool CRealNRand1Mutation::bMutate(CRealCoding *pcGenotype, uint32_t iIndex)

uint32_t CRealNRand1Mutation::i_get_nearest_index(CRealCoding *pcGenotype, uint32_t iIndex)
{
	uint32_t i_nearest_index = iIndex;
	double d_min_distance = DBL_MAX;

	CRealCoding *pc_other;

	for (uint32_t i = 0; i < i_population_size; i++)
	{
		pc_other = *(ppc_population + i);

		if (i != iIndex)
		{
			if (!pcGenotype->bExceededMaxDistance(pc_other, d_min_distance, &d_min_distance))
			{
				i_nearest_index = i;
			}//if (!pcGenotype->bExceededMaxDistance(pc_other, d_min_distance, &d_min_distance))	 
		}//if (i != iIndex)
	}//for (uint32_t i = 1; i < i_population_size; i++)

	return i_nearest_index;
}//uint32_t CRealNRand1Mutation::i_get_nearest_index(CRealCoding *pcGenotype)