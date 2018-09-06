#include "BinaryGeneration.h"

#include "CommandParam.h"
#include "FloatCommandParam.h"
#include "GenePattern.h"
#include "RandUtils.h"

#include <vector>

using namespace std;

CBinaryGeneration::CBinaryGeneration(uint16_t iNumberOfBits)
{
	i_number_of_bits = iNumberOfBits;
}//CBinaryGeneration::CBinaryGeneration(uint16_t iNumberOfBits)

CBinaryGeneration::CBinaryGeneration(CBinaryCoding *pcSample)
	: CBinaryGeneration(pcSample->iGetNumberOfBits())
{

}//CBinaryGeneration::CBinaryGeneration(CBinaryCoding *pcSample)


CBinaryRandomGeneration::CBinaryRandomGeneration(uint16_t iNumberOfBits)
	: CBinaryGeneration(iNumberOfBits)
{

}//CBinaryRandomGeneration::CBinaryRandomGeneration(uint16_t iNumberOfBits)

CBinaryRandomGeneration::CBinaryRandomGeneration(CBinaryCoding *pcSample)
	: CBinaryGeneration(pcSample)
{

}//CBinaryRandomGeneration::CBinaryRandomGeneration(CBinaryCoding *pcSample)

CBinaryCoding * CBinaryRandomGeneration::pcGenerate()
{
	int32_t *pi_bits = new int32_t[i_number_of_bits];

	for (uint16_t i = 0; i < i_number_of_bits; i++)
	{
		*(pi_bits + i) = RandUtils::iRandNumber(0, 1);
	}//for (uint16_t i = 0; i < i_number_of_bits; i++)

	return new CBinaryCoding(i_number_of_bits, pi_bits);
}//CBinaryCoding * CBinaryRandomGeneration::pcGenerate()


CBinaryRandomGenePatternGeneration::CBinaryRandomGenePatternGeneration(uint16_t iNumberOfBits)
	: CBinaryGeneration(iNumberOfBits), c_random_generation(iNumberOfBits)
{

}//CBinaryRandomGenePatternGeneration::CBinaryRandomGenePatternGeneration(uint16_t iNumberOfBits)

CBinaryRandomGenePatternGeneration::CBinaryRandomGenePatternGeneration(CBinaryCoding *pcSample)
	: CBinaryGeneration(pcSample), c_random_generation(pcSample)
{

}//CBinaryRandomGenePatternGeneration::CBinaryRandomGenePatternGeneration(CBinaryCoding *pcSample)

CBinaryRandomGenePatternGeneration::CBinaryRandomGenePatternGeneration(uint16_t iNumberOfBits, CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent, float fGenePatternMatchFactor)
	: CBinaryGeneration(iNumberOfBits), c_random_generation(iNumberOfBits)
{
	v_init(plGenePatterns, bGenePatternEachDifferent, fGenePatternMatchFactor);
}//CBinaryRandomGenePatternGeneration::CBinaryRandomGenePatternGeneration(uint16_t iNumberOfBits, CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent, float fGenePatternMatchFactor)

CBinaryRandomGenePatternGeneration::CBinaryRandomGenePatternGeneration(CBinaryCoding *pcSample, CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent, float fGenePatternMatchFactor)
	: CBinaryGeneration(pcSample), c_random_generation(pcSample)
{
	v_init(plGenePatterns, bGenePatternEachDifferent, fGenePatternMatchFactor);
}//CBinaryRandomGenePatternGeneration::CBinaryRandomGenePatternGeneration(CBinaryCoding *pcSample, CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent, float fGenePatternMatchFactor)

CError CBinaryRandomGenePatternGeneration::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)
{
	CError c_error = CBinaryGeneration::eConfigure(plGenePatterns, psSettings);

	if (!c_error)
	{
		if (plGenePatterns)
		{
			pl_gene_patterns = plGenePatterns;
		}//if (plGenePatterns)
		else
		{
			c_error.vSetError(CError::iERROR_CODE_SYSTEM_MISSING_ARGUMENT, "gene patterns");
		}//else if (plGenePatterns)
	}//if (!c_error)

	if (!c_error)
	{
		CBoolCommandParam p_gene_pattern_each_different(GENERATION_ARGUMENT_GENE_PATTERN_EACH_DIFFERENT);
		b_gene_pattern_each_different = p_gene_pattern_each_different.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CFloatCommandParam p_gene_pattern_match_factor(GENERATION_ARGUMENT_GENE_PATTERN_MATCH_FACTOR, DEF_PROBABILITY_MIN, DEF_PROBABILITY_MAX);
		f_gene_pattern_match_factor = p_gene_pattern_match_factor.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;;
}//CError CBinaryRandomGenePatternGeneration::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)

#include <iostream>
using namespace std;

CBinaryCoding* CBinaryRandomGenePatternGeneration::pcGenerate()
{
	CBinaryCoding *pc_binary_coding = c_random_generation.pcGenerate();

	if (!v_best_genotypes.empty() && pl_gene_patterns->bAny())
	{
		CGenePattern *pc_gene_pattern = b_gene_pattern_each_different ? pl_gene_patterns->pcGetRandomGenePattern() : this->pc_gene_pattern;

		uint16_t i_gene_index;
		uint32_t i_number_of_zeros;

		//cout << endl << "pattern" << endl;

		//for (uint16_t i = 0; i < pc_gene_pattern->iGetSize(); i++)
		//{
		//	cout << *(pc_gene_pattern->piGetPattern() + i) << " ";
		//}

		//cout << endl << endl;

		//for (uint32_t j = 0; j < (uint32_t)v_best_genotypes.size(); j++)
		//{
		//	cout << j << ": " << v_best_genotypes.at(j) << endl;
		//}//for (uint32_t j = 0; j < (uint32_t)v_best_genotypes.size(); j++)

		//cout << endl;

		//cout << "before" << endl;

		//cout << pc_binary_coding << endl << endl;

		if (RandUtils::bSatisfyProbability(f_gene_pattern_match_factor))
		{
			/*cout << "first" << endl << endl;*/

			for (uint16_t i = 0; i < pc_gene_pattern->iGetSize(); i++)
			{
				i_gene_index = *(pc_gene_pattern->piGetPattern() + i);
				i_number_of_zeros = 0;

				for (uint32_t j = 0; j < (uint32_t)v_best_genotypes.size(); j++)
				{
					if (*(v_best_genotypes.at(j)->piGetBits() + i_gene_index) == 0)
					{
						i_number_of_zeros++;
					}//if (*(v_best_genotypes.at(i)->piGetBits() + i_gene_index) == 0)
				}//for (uint32_t j = 0; j < (uint32_t)v_best_genotypes.size(); j++)

				if (RandUtils::bSatisfyProbability(1.0f - (float)i_number_of_zeros / (float)v_best_genotypes.size()))
				{
					*(pc_binary_coding->piGetBits() + i_gene_index) = 0;
				}//if (RandUtils::bSatisfyProbability(1.0f - (float)i_number_of_zeros / (float)v_best_genotypes.size()))
				else
				{
					*(pc_binary_coding->piGetBits() + i_gene_index) = 1;
				}//else if (RandUtils::bSatisfyProbability(1.0f - (float)i_number_of_zeros / (float)v_best_genotypes.size()))
			}//for (uint16_t i = 0; i < pc_gene_pattern->iGetSize(); i++)
		}//if (RandUtils::bSatisfyProbability(f_gene_pattern_match_factor))
		else
		{
			/*cout << "second" << endl << endl;
*/
			uint16_t i_gene_pattern_gene_index = 0;

			for (uint16_t i = 0; i < i_number_of_bits; i++)
			{
				if (i < *(pc_gene_pattern->piGetPattern() + i_gene_pattern_gene_index))
				{
					i_gene_index = i;
					i_number_of_zeros = 0;

					for (uint32_t j = 0; j < (uint32_t)v_best_genotypes.size(); j++)
					{
						if (*(v_best_genotypes.at(j)->piGetBits() + i_gene_index) == 0)
						{
							i_number_of_zeros++;
						}//if (*(v_best_genotypes.at(i)->piGetBits() + i_gene_index) == 0)
					}//for (uint32_t j = 0; j < (uint32_t)v_best_genotypes.size(); j++)

					if (RandUtils::bSatisfyProbability(1.0f - (float)i_number_of_zeros / (float)v_best_genotypes.size()))
					{
						*(pc_binary_coding->piGetBits() + i_gene_index) = 0;
					}//if (RandUtils::bSatisfyProbability(1.0f - (float)i_number_of_zeros / (float)v_best_genotypes.size()))
					else
					{
						*(pc_binary_coding->piGetBits() + i_gene_index) = 1;
					}//else if (RandUtils::bSatisfyProbability(1.0f - (float)i_number_of_zeros / (float)v_best_genotypes.size()))
				}//if (i < *(pc_gene_pattern->piGetPattern() + i_gene_pattern_gene_index))
				else
				{
					i_gene_pattern_gene_index++;
				}//else if (i < *(pc_gene_pattern->piGetPattern() + i_gene_pattern_gene_index))
			}//for (uint16_t i = 0; i < i_number_of_bits; i++)
		}//else if (RandUtils::bSatisfyProbability(f_gene_pattern_match_factor))

			//cout << "new" << endl;
			//cout << pc_binary_coding << endl << endl;
	}//if (!v_best_genotypes.empty() && pl_gene_patterns->bAny())

	return pc_binary_coding;
}//CBinaryCoding* CBinaryRandomGenePatternGeneration::pcGenerate()

void CBinaryRandomGenePatternGeneration::v_init(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent, float fGenePatternMatchFactor)
{
	pl_gene_patterns = plGenePatterns;

	b_gene_pattern_each_different = bGenePatternEachDifferent;
	f_gene_pattern_match_factor = fGenePatternMatchFactor;
}//void CBinaryRandomGenePatternGeneration::v_init(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent, float fGenePatternMatchFactor)