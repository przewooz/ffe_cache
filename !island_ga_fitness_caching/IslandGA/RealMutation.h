#ifndef REAL_MUTATION_H
#define REAL_MUTATION_H

#define REAL_MUTATION_RAND_1_NUMBER_OF_RANDOM_INDEXES 3
#define REAL_MUTATION_NRAND_1_NUMBER_OF_RANDOM_INDEXES 2

#include "Error.h"
#include "GenePatternList.h"
#include "Mutation.h"
#include "RealCoding.h"

#include <cstdint>
#include <istream>

using namespace std;

class CRealMutation : public CMutation<CRealCoding>
{
public:
	CRealMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired);
	CRealMutation(float fProbability, bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired);

	virtual bool bMutate(CRealCoding *pcGenotype, uint32_t iIndex) = 0;
};//class CRealMutation : public CMutation<CRealCoding>


class CRealRandomMutation : public CRealMutation
{
public:
	CRealRandomMutation();
	CRealRandomMutation(float fProbability);

	virtual bool bMutate(CRealCoding *pcGenotype, uint32_t iIndex);
};//class CRealRandomMutation : public CRealMutation


class CRealFactorMutation : public CRealMutation
{
public:
	CRealFactorMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired);
	CRealFactorMutation(float fProbability, double dFactor, bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired);

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings);

	virtual bool bMutate(CRealCoding *pcGenotype, uint32_t iIndex) = 0;

protected:
	double d_factor;
};//class CRealFactorMutation : public CRealMutation


class CRealTargetToBestMutation : public CRealFactorMutation
{
public:
	CRealTargetToBestMutation();
	CRealTargetToBestMutation(float fProbability, double dFactor);

	virtual bool bMutate(CRealCoding *pcGenotype, uint32_t iIndex);
};//class CRealTargetToBestMutation : public CRealFactorMutation


class CRealRand1Mutation : public CRealFactorMutation
{
public:
	CRealRand1Mutation();
	CRealRand1Mutation(float fProbability, double dFactor);

	virtual bool bMutate(CRealCoding *pcGenotype, uint32_t iIndex);
};//class CRealRand1Mutation : public CRealFactorMutation


class CRealNRand1Mutation : public CRealFactorMutation
{
public:
	CRealNRand1Mutation();
	CRealNRand1Mutation(float fProbability, double dFactor);

	virtual bool bMutate(CRealCoding *pcGenotype, uint32_t iIndex);

private:
	uint32_t i_get_nearest_index(CRealCoding *pcGenotype, uint32_t iIndex);
};//class CRealNRand1Mutation : public CRealFactorMutation

#endif//REAL_MUTATION_H