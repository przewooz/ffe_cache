#ifndef GENERATION_H
#define GENERATION_H

#define GENERATION_ARGUMENT_TYPE "generation_type"
#define GENERATION_ARGUMENT_TYPE_BINARY_RANDOM "binary_random"
#define GENERATION_ARGUMENT_TYPE_BINARY_GENE_PATTERN_RANDOM "binary_gene_pattern_random"
#define GENERATION_ARGUMENT_TYPE_REAL_RANDOM "real_random"

#define GENERATION_ARGUMENT_GENE_PATTERN_EACH_DIFFERENT "generation_gene_pattern_each_different"

#define GENERATION_ARGUMENT_GENE_PATTERN_MATCH_FACTOR "generation_gene_pattern_match_factor"

#include "Error.h"
#include "GenePattern.h"
#include "GenePatternList.h"

#include <cstdint>
#include <istream>
#include <vector>

using namespace std;


enum EGenerationType
{
	GENERATION_BINARY_RANDOM = 0,
	GENERATION_BINARY_RANDOM_GENE_PATTERN = 1,
	GENERATION_REAL_RANDOM = 2
};//enum EGenerationType


template <class TGenotype>
class CGeneration
{
public:
	static uint32_t iERROR_PARENT_CGeneration;

	CGeneration();

	virtual ~CGeneration();

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings) { return CError(iERROR_PARENT_CGeneration); };

	virtual TGenotype *pcGenerate() = 0;
	virtual TGenotype *pcGenerateEmpty() = 0;

	virtual void vUpdateBestGenotypes(uint8_t iCount, TGenotype **ppcBestGenotypes);

	void vSetGenePattern(CGenePattern *pcGenePattern) { pc_gene_pattern = pcGenePattern; };

protected:
	void v_clear_best_genotypes();

	CGenePattern *pc_gene_pattern;
	vector<TGenotype*> v_best_genotypes;
};//class CGeneration

#endif//GENERATION_H