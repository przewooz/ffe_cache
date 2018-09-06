#ifndef GENE_PATTERN_REPLACEMENT_H
#define GENE_PATTERN_REPLACEMENT_H

#define GENE_PATTERN_REPLACEMENT_ARGUMENT_TYPE "gene_pattern_replacement_type"
#define GENE_PATTERN_REPLACEMENT_ARGUMENT_TYPE_RANDOM "random"
#define GENE_PATTERN_REPLACEMENT_ARGUMENT_TYPE_PREFER_SHORTER "prefer_shorter"

#define GENE_PATTERN_REPLACEMENT_ARGUMENT_NUMBER_OF_SAMPLES "gene_pattern_replacement_number_of_samples"

#include "Error.h"
#include "GenePattern.h"

#include <cstdint>
#include <istream>

using namespace std;


enum EGenePatternReplacementType
{
	GENE_PATTERN_REPLACEMENT_RANDOM = 0,
	GENE_PATTERN_REPLACEMENT_PREFER_SHORTER = 1
};//enum EGenePatternReplacementType


class CGenePatternReplacement
{
public:
	static uint32_t iERROR_PARENT_CGenePatternReplacement;

	virtual ~CGenePatternReplacement();

	virtual CError eConfigure(istream *psSettings) { return CError(iERROR_PARENT_CGenePatternReplacement); };

	virtual void vReplace(CGenePattern *pcGenePatternToAdd, uint32_t iNumberOfGenePatterns, CGenePattern **ppcGenePatterns) = 0;
};//class CGenePatternReplacement


class CRandomGenePatternReplacement : public CGenePatternReplacement
{
public:
	virtual void vReplace(CGenePattern *pcGenePatternToAdd, uint32_t iNumberOfGenePatterns, CGenePattern **ppcGenePatterns);
};//class CRandomGenePatternReplacement : public CGenePatternReplacement


class CPreferShorterGenePatternReplacement : public CGenePatternReplacement
{
public:
	virtual CError eConfigure(istream *psSettings);

	virtual void vReplace(CGenePattern *pcGenePatternToAdd, uint32_t iNumberOfGenePatterns, CGenePattern **ppcGenePatterns);

private:
	uint32_t i_number_of_samples;
};//class CPreferShorterGenePatternReplacement : public CGenePatternReplacement

#endif//GENE_PATTERN_REPLACEMENT_H