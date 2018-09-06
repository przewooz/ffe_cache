#ifndef GENE_PATTERN_LIST_H
#define GENE_PATTERN_LIST_H

#include "GenePattern.h"
#include "GenePatternReplacement.h"
#include "RandUtils.h"

#include <cstdint>

class CGenePatternList
{
public:
	CGenePatternList(uint32_t iMaxSize, CGenePatternReplacement *pcGenePatternReplacement);
	
	~CGenePatternList();

	bool bAny() { return i_number_of_gene_patterns > 0; };

	void vAdd(CGenePattern *pcGenePattern);

	CGenePattern *pcGetRandomGenePattern();
	CGenePattern *pcGetRandomGenePattern(uint32_t *piRandomIndex);

	uint32_t iGetNumberOfGenePatterns() { return i_number_of_gene_patterns; };
	CGenePattern **ppcGetGenePatterns() { return  ppc_gene_patterns; };

private:
	uint32_t i_max_size;

	uint32_t i_number_of_gene_patterns;
	CGenePattern **ppc_gene_patterns;

	CGenePatternReplacement *pc_gene_pattern_replacement;
};//class CGenePatternList

#endif//GENE_PATTERN_LIST_H