#ifndef MUTATION_H
#define MUTATION_H

#define MUTATION_ARGUMENT_TYPE "mutation_type"
#define MUTATION_ARGUMENT_TYPE_BINARY_NEGATION "binary_negation"
#define MUTATION_ARGUMENT_TYPE_BINARY_GENE_PATTERN_BEST_SWAP "binary_gene_pattern_best_swap"
#define MUTATION_ARGUMENT_TYPE_REAL_RANDOM "real_random"
#define MUTATION_ARGUMENT_TYPE_REAL_TARGET_TO_BEST "real_target_to_best"
#define MUTATION_ARGUMENT_TYPE_REAL_RAND_1 "real_rand_1"
#define MUTATION_ARGUMENT_TYPE_REAL_NRAND_1 "real_nrand_1"

#define MUTATION_ARGUMENT_PROBABILITY "mutation_probability"
#define MUTATION_ARGUMENT_FACTOR "mutation_factor"

#include "BinaryCoding.h"
#include "Error.h"
#include "GenePatternList.h"
#include "RandUtils.h"

#include <cstdint>
#include <istream>
#include <vector>

using namespace std;


enum EMutationType
{
	MUTATION_BINARY_NEGATION = 0,
	MUTATION_BINARY_GENE_PATTERN_BEST_SWAP = 1,
	MUTATION_REAL_RANDOM = 2,
	MUTATION_REAL_TARGET_TO_BEST = 3,
	MUTATION_REAL_RAND_1 = 4,
	MUTATION_REAL_NRAND_1 = 5
};//enum EMutationType


template <class TGenotype>
class CMutation
{
public:
	CMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired);
	CMutation(float fProbability, bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired);
	
	virtual ~CMutation();

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings);

	virtual bool bMutate(TGenotype *pcGenotype, uint32_t iIndex) = 0;

	virtual void vUpdatePopulation(uint32_t iSize, TGenotype **ppcPopulation);
	virtual void vUpdateBestGenotype(TGenotype *pcBestGenotype);
	virtual void vUpdateBestGenotypes(uint8_t iCount, TGenotype **ppcBestGenotypes);

	bool bPopulationRequired() { return b_population_required; };
	bool bBestGenotypeRequired() { return b_best_genotype_required; };
	bool bBestGenotypesRequired() { return b_best_genotypes_required; };

protected:
	virtual bool b_apply_mutation(TGenotype *pcGenotype) { return RandUtils::bSatisfyProbability(f_probability); };

	void v_clear_population();
	void v_clear_best_genotypes();

	float f_probability;

	bool b_population_required;
	bool b_best_genotype_required;
	bool b_best_genotypes_required;

	uint32_t i_population_size;
	TGenotype **ppc_population;

	TGenotype *pc_best_genotype;

	vector<TGenotype*> v_best_genotypes;

private:
	void v_init(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired);
};//class CMutation

class CBinaryGenePatternBestSwapMutation : public CMutation<CBinaryCoding>
{
public:
	CBinaryGenePatternBestSwapMutation();
	CBinaryGenePatternBestSwapMutation(float fProbability, CGenePatternList *plGenePatterns);

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings);

	virtual bool bMutate(CBinaryCoding *pcGenotype, uint32_t iIndex);

private:
	CGenePatternList *pl_gene_patterns;
};//class CBinaryGenePatternBestSwapMutation

#endif//MUTATION_H