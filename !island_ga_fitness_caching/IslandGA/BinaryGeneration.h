#ifndef BINARY_GENERATION_H
#define BINARY_GENERATION_H

#include "BinaryCoding.h"
#include "Error.h"
#include "GenePatternList.h"
#include "Generation.h"

#include <cstdint>
#include <istream>

using namespace std;

class CBinaryGeneration : public CGeneration<CBinaryCoding>
{
public:
	CBinaryGeneration(uint16_t iNumberOfBits);
	CBinaryGeneration(CBinaryCoding *pcSample);

	virtual CBinaryCoding *pcGenerate() { return pcGenerateEmpty(); };
	virtual CBinaryCoding *pcGenerateEmpty() { return new CBinaryCoding(i_number_of_bits); };

protected:
	uint16_t i_number_of_bits;
};//class CBinaryGeneration

class CBinaryRandomGeneration : public CBinaryGeneration
{
public:
	CBinaryRandomGeneration(uint16_t iNumberOfBits);
	CBinaryRandomGeneration(CBinaryCoding *pcSample);

	virtual CBinaryCoding *pcGenerate();

	virtual void vUpdateBestGenotypes(uint8_t iCount, CBinaryCoding **ppcBestGenotypes) { };
};//class CBinaryRandomGeneration

class CBinaryRandomGenePatternGeneration : public CBinaryGeneration
{
public:
	CBinaryRandomGenePatternGeneration(uint16_t iNumberOfBits);
	CBinaryRandomGenePatternGeneration(CBinaryCoding *pcSample);
	CBinaryRandomGenePatternGeneration(uint16_t iNumberOfBits, CGenePatternList *plGenePatterns,
		bool bGenePatternEachDifferent, float fGenePatternMatchFactor);
	CBinaryRandomGenePatternGeneration(CBinaryCoding *pcSample, CGenePatternList *plGenePatterns,
		bool bGenePatternEachDifferent, float fGenePatternMatchFactor);

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings);

	virtual CBinaryCoding *pcGenerate();

private:
	void v_init(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent, float fGenePatternMatchFactor);

	CGenePatternList *pl_gene_patterns;

	bool b_gene_pattern_each_different;
	float f_gene_pattern_match_factor;

	CBinaryRandomGeneration c_random_generation;
};//class CBinaryRandomGenePatternGeneration

#endif//BINARY_GENERATION_H