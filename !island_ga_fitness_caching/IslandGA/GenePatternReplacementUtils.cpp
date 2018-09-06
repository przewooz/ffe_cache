#include "GenePatternReplacementUtils.h"

#include "EnumCommandParam.h"

#include <atlstr.h>
#include <unordered_map>

CGenePatternReplacement * GenePatternReplacementUtils::pcGetGenePatternReplacement(istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CGenePatternReplacement *pc_gene_pattern_replacement = nullptr;

	unordered_map<CString, EGenePatternReplacementType> m_gene_pattern_replacement_types;
	m_gene_pattern_replacement_types.insert(pair<const CString, EGenePatternReplacementType>(GENE_PATTERN_REPLACEMENT_ARGUMENT_TYPE_RANDOM, GENE_PATTERN_REPLACEMENT_RANDOM));
	m_gene_pattern_replacement_types.insert(pair<const CString, EGenePatternReplacementType>(GENE_PATTERN_REPLACEMENT_ARGUMENT_TYPE_PREFER_SHORTER, GENE_PATTERN_REPLACEMENT_PREFER_SHORTER));

	CEnumCommandParam<EGenePatternReplacementType> p_type(GENE_PATTERN_REPLACEMENT_ARGUMENT_TYPE, &m_gene_pattern_replacement_types, bIsObligatory);
	EGenePatternReplacementType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch(e_type)
		{
			case GENE_PATTERN_REPLACEMENT_RANDOM:
			{
				pc_gene_pattern_replacement = new CRandomGenePatternReplacement();
				break;
			}//case GENE_PATTERN_REPLACEMENT_RANDOM
			case GENE_PATTERN_REPLACEMENT_PREFER_SHORTER:
			{
				pc_gene_pattern_replacement = new CPreferShorterGenePatternReplacement();
				break;
			}//case GENE_PATTERN_REPLACEMENT_PREFER_SHORTER
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "gene pattern replacement");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_gene_pattern_replacement->eConfigure(psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_gene_pattern_replacement;
}//CGenePatternReplacement * GenePatternReplacementUtils::pcGetGenePatternReplacement(istream *psSettings, CError *pcError, bool bIsObligatory)