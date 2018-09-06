#ifndef CLASSIC_POPULATION_STRATEGY_H
#define CLASSIC_POPULATION_STRATEGY_H

#define DEF_CLASSIC_STRATEGY_INIT_NUMBER_OF_POPULATIONS 2
#define DEF_CLASSIC_STRATEGY_NUMBER_OF_POPULATIONS_TO_CREATE 1

#include "Error.h"
#include "Individual.h"
#include "PopulationStrategy.h"
#include "Problem.h"

#include <cstdint>
#include <istream>
#include <unordered_set>

using namespace std;

template <class TGenotype, class TFenotype>
class CClassicPopulationStrategy : public CPopulationStrategy<TGenotype, TFenotype>
{
public:
	CClassicPopulationStrategy(CProblem<TGenotype, TFenotype> *pcProblem);
	CClassicPopulationStrategy(uint32_t iMaxNumberOfIterationsWithoutProgress, CProblem<TGenotype, TFenotype> *pcProblem);

	virtual CError eConfigure(istream *psSettings);

	virtual void vUpdateNumberOfPopulations(uint32_t iIterationNumber, uint8_t iNumberOfPopulations,
		CIndividual<TGenotype, TFenotype> *pcBestIndividual, CIndividual<TGenotype, TFenotype> **ppcBestPopulationsIndividuals, 
		unordered_set<uint8_t> *psPopulationToRemoveIndexes, uint8_t *piNumberOfPopulationsToCreate);

protected:
	uint32_t i_max_number_of_iterations_without_progress;
	uint32_t i_last_progress_iteration_number;

	double d_last_avg_fitness_value;

private:
	void v_init();
};//class CClassicPopulationStrategy

#endif//CLASSIC_POPULATION_STRATEGY_H