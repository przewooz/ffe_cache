#include "GenePatternReplacement.h"

#include "RandUtils.h"
#include "UIntCommandParam.h"

#include <unordered_set>


uint32_t CGenePatternReplacement::iERROR_PARENT_CGenePatternReplacement = CError::iADD_ERROR_PARENT("CGenePatternReplacement");
CGenePatternReplacement::~CGenePatternReplacement()
{

}//CGenePatternReplacement::~CGenePatternReplacement()


void CRandomGenePatternReplacement::vReplace(CGenePattern *pcGenePatternToAdd, uint32_t iNumberOfGenePatterns, CGenePattern **ppcGenePatterns)
{
	uint32_t i_random_index = RandUtils::iRandIndex(iNumberOfGenePatterns);

	delete *(ppcGenePatterns + i_random_index);

	*(ppcGenePatterns + i_random_index) = pcGenePatternToAdd;
}//void CRandomGenePatternReplacement::vReplace(CGenePattern *pcGenePatternToAdd, uint32_t iNumberOfGenePatterns, CGenePattern **pcGenePatterns)


CError CPreferShorterGenePatternReplacement::eConfigure(istream *psSettings)
{
	CError c_error = CGenePatternReplacement::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_number_of_samples(GENE_PATTERN_REPLACEMENT_ARGUMENT_NUMBER_OF_SAMPLES, 2, UINT32_MAX);
		i_number_of_samples = p_number_of_samples.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CPreferShorterGenePatternReplacement::eConfigure(istream *psSettings)

#include <iostream>

void CPreferShorterGenePatternReplacement::vReplace(CGenePattern *pcGenePatternToAdd, uint32_t iNumberOfGenePatterns, CGenePattern **ppcGenePatterns)
{
	unordered_set<uint32_t> s_selected_gene_patterns_indexes(i_number_of_samples);

	uint32_t i_longest_gene_pattern_index = RandUtils::iRandIndex(iNumberOfGenePatterns);
	s_selected_gene_patterns_indexes.insert(i_longest_gene_pattern_index);

	CGenePattern *pc_random_gene_pattern = *(ppcGenePatterns + i_longest_gene_pattern_index);

	uint16_t i_longest_gene_pattern_size = pc_random_gene_pattern->iGetSize();

	uint32_t i_random_index;

	for (uint32_t i = 0; i < i_number_of_samples - 1; i++)
	{
		i_random_index = RandUtils::iRandUniqueIndex(iNumberOfGenePatterns, &s_selected_gene_patterns_indexes);
		s_selected_gene_patterns_indexes.insert(i_random_index);

		pc_random_gene_pattern = *(ppcGenePatterns + i_random_index);

		if (pc_random_gene_pattern->iGetSize() > i_longest_gene_pattern_size)
		{
			i_longest_gene_pattern_index = i_random_index;
			i_longest_gene_pattern_size = pc_random_gene_pattern->iGetSize();
		}//if (pc_random_gene_pattern->iGetSize() > i_longest_gene_pattern_size)
	}//for (uint32_t i = 0; i < i_number_of_samples - 1; i++)

	delete *(ppcGenePatterns + i_longest_gene_pattern_index);

	*(ppcGenePatterns + i_longest_gene_pattern_index) = pcGenePatternToAdd;
}//void CPreferShorterGenePatternReplacement::vReplace(CGenePattern *pcGenePatternToAdd, uint32_t iNumberOfGenePatterns, CGenePattern **ppcGenePatterns)