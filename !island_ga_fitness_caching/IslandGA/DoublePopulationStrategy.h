#ifndef DOUBLE_POPULATION_STRATEGY_H
#define DOUBLE_POPULATION_STRATEGY_H

#define DEF_DOUBLE_STRATEGY_INIT_NUMBER_OF_POPULATIONS 2

#include "Individual.h"
#include "PopulationStrategy.h"
#include "Problem.h"

#include <cstdint>
#include <unordered_set>

using namespace std;

template <class TGenotype, class TFenotype>
class CDoublePopulationStrategy : public CPopulationStrategy<TGenotype, TFenotype>
{
public:
	CDoublePopulationStrategy();

	virtual void vUpdateNumberOfPopulations(uint32_t iIterationNumber, uint8_t iNumberOfPopulations,
		CIndividual<TGenotype, TFenotype> *pcBestIndividual, CIndividual<TGenotype, TFenotype> **ppcBestPopulationsIndividuals,
		unordered_set<uint8_t> *psPopulationToRemoveIndexes, uint8_t *piNumberOfPopulationsToCreate) { };
};//class CDoublePopulationStrategy

#endif//DOUBLE_POPULATION_STRATEGY_H