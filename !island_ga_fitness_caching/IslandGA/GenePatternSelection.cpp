#include "GenePatternSelection.h"

#include "EnumCommandParam.h"
#include "RandUtils.h"
#include "StringUtils.h"
#include "UIntCommandParam.h"

#include <atlstr.h>
#include <unordered_map>

using namespace GenePatternSelection;


CError CPreferShorterGenePatternSelection::eConfigure(istream *psSettings)
{
	CError c_error = CGenePatternSelection::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_number_of_samples(GENE_PATTERN_SELECTION_ARGUMENT_NUMBER_OF_SAMPLES, 2, UINT32_MAX);
		i_number_of_samples = p_number_of_samples.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CPreferShorterGenePatternSelection::eConfigure(istream *psSettings)

CGenePattern * CPreferShorterGenePatternSelection::pcSelectGenePattern(CGenePatternList *plGenePatterns)
{
	unordered_set<uint32_t> s_selected_gene_patterns_indexes(i_number_of_samples);

	uint32_t i_shortest_gene_pattern_index = RandUtils::iRandIndex(plGenePatterns->iGetNumberOfGenePatterns());
	s_selected_gene_patterns_indexes.insert(i_shortest_gene_pattern_index);

	CGenePattern *pc_random_gene_pattern = *(plGenePatterns->ppcGetGenePatterns() + i_shortest_gene_pattern_index);

	uint16_t i_shortest_gene_pattern_size = pc_random_gene_pattern->iGetSize();

	uint32_t i_random_index;

	for (uint32_t i = 0; i < i_number_of_samples - 1; i++)
	{
		i_random_index = RandUtils::iRandUniqueIndex(plGenePatterns->iGetNumberOfGenePatterns(), &s_selected_gene_patterns_indexes);
		s_selected_gene_patterns_indexes.insert(i_random_index);

		pc_random_gene_pattern = *(plGenePatterns->ppcGetGenePatterns() + i_random_index);

		if (pc_random_gene_pattern->iGetSize() < i_shortest_gene_pattern_size)
		{
			i_shortest_gene_pattern_index = i_random_index;
			i_shortest_gene_pattern_size = pc_random_gene_pattern->iGetSize();
		}//if (pc_random_gene_pattern->iGetSize() < i_shortest_gene_pattern_size)
	}//for (uint32_t i = 0; i < i_number_of_samples - 1; i++)

	return *(plGenePatterns->ppcGetGenePatterns() + i_shortest_gene_pattern_index);
}//CGenePattern * CPreferShorterGenePatternSelection::pcSelectGenePattern(CGenePatternList *plGenePatterns)


CGenePatternSelection * GenePatternSelection::pcGetGenePatternSelection(istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CGenePatternSelection *pc_gene_pattern_selection = nullptr;

	unordered_map<CString, EGenePatternSelectionType> m_gene_pattern_selection_types;
	m_gene_pattern_selection_types.insert(pair<const CString, EGenePatternSelectionType>(GENE_PATTERN_SELECTION_ARGUMENT_TYPE_RANDOM, GENE_PATTERN_SELECTION_RANDOM));
	m_gene_pattern_selection_types.insert(pair<const CString, EGenePatternSelectionType>(GENE_PATTERN_SELECTION_ARGUMENT_TYPE_PREFER_SHORTER, GENE_PATTERN_SELECTION_PREFER_SHORTER));

	CEnumCommandParam<EGenePatternSelectionType> p_type(GENE_PATTERN_SELECTION_ARGUMENT_TYPE, &m_gene_pattern_selection_types, bIsObligatory);
	EGenePatternSelectionType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case GENE_PATTERN_SELECTION_RANDOM:
			{
				pc_gene_pattern_selection = new CRandomGenePatternSelection();
				break;
			}//case GENE_PATTERN_SELECTION_RANDOM
			case GENE_PATTERN_SELECTION_PREFER_SHORTER:
			{
				pc_gene_pattern_selection = new CPreferShorterGenePatternSelection();
				break;
			}//case GENE_PATTERN_SELECTION_PREFER_SHORTER
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "gene pattern selection");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_gene_pattern_selection->eConfigure(psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_gene_pattern_selection;
}//CGenePatternSelection * GenePatternSelection::pcGetGenePatternSelection(istream *psSettings, CError *pcError, bool bIsObligatory)