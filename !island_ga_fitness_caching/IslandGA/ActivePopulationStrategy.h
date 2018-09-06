#ifndef ACTIVE_POPULATION_STRATEGY_H
#define ACTIVE_POPULATION_STRATEGY_H

#define DEF_ACTIVE_STRATEGY_INIT_NUMBER_OF_POPULATIONS 1

#include "Individual.h"
#include "ClassicPopulationStrategy.h"
#include "Problem.h"

#include <cstdint>
#include <unordered_set>

using namespace std;

template <class TGenotype, class TFenotype>
class CActivePopulationStrategy : public CClassicPopulationStrategy<TGenotype, TFenotype>
{
public:
	CActivePopulationStrategy(CProblem<TGenotype, TFenotype> *pcProblem);
	CActivePopulationStrategy(uint32_t iNumberOfIterationsWithoutProgress, CProblem<TGenotype, TFenotype> *pcProblem);

	virtual ~CActivePopulationStrategy();

	virtual void vUpdateNumberOfPopulations(uint32_t iIterationNumber, uint8_t iNumberOfPopulations,
		CIndividual<TGenotype, TFenotype> *pcBestIndividual, CIndividual<TGenotype, TFenotype> **ppcBestPopulationsIndividuals, 
		unordered_set<uint8_t> *psPopulationToRemoveIndexes, uint8_t *piNumberOfPopulationsToCreate);

private:
	void v_init();

	CIndividual<TGenotype, TFenotype> *pc_best_individual;
};//class CActivePopulationStrategy

#endif//ACTIVE_POPULATION_STRATEGY_H