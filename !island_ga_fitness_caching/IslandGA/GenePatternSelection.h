#ifndef GENE_PATTERN_SELECTION_H
#define GENE_PATTERN_SELECTION_H

#define GENE_PATTERN_SELECTION_ARGUMENT_TYPE "gene_pattern_selection_type"
#define GENE_PATTERN_SELECTION_ARGUMENT_TYPE_RANDOM "random"
#define GENE_PATTERN_SELECTION_ARGUMENT_TYPE_PREFER_SHORTER "prefer_shorter"

#define GENE_PATTERN_SELECTION_ARGUMENT_NUMBER_OF_SAMPLES "gene_pattern_selection_number_of_samples"

#include "Error.h"
#include "GenePattern.h"
#include "GenePatternList.h"

#include <cstdint>
#include <iostream>

using namespace std;


namespace GenePatternSelection
{
	enum EGenePatternSelectionType
	{
		GENE_PATTERN_SELECTION_RANDOM = 0,
		GENE_PATTERN_SELECTION_PREFER_SHORTER = 1
	};//enum EGenePatternSelectionType


	class CGenePatternSelection
	{
	public:
		virtual ~CGenePatternSelection() { };

		virtual CError eConfigure(istream *psSettings) { return CError(); };

		virtual CGenePattern *pcSelectGenePattern(CGenePatternList *plGenePatterns) = 0;
	};//class CGenePatternSelection


	class CRandomGenePatternSelection : public CGenePatternSelection
	{
		virtual CGenePattern *pcSelectGenePattern(CGenePatternList *plGenePatterns) { return plGenePatterns->pcGetRandomGenePattern(); };
	};//class CRandomGenePatternSelection : public CGenePatternSelection


	class CPreferShorterGenePatternSelection : public CGenePatternSelection
	{
		virtual CError eConfigure(istream *psSettings);

		virtual CGenePattern *pcSelectGenePattern(CGenePatternList *plGenePatterns);

	private:
		uint32_t i_number_of_samples;
	};//class CPreferShorterGenePatternSelection : public CGenePatternSelection


	CGenePatternSelection *pcGetGenePatternSelection(istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace GenePatternSelection

#endif//GENE_PATTERN_SELECTION_H