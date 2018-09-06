#ifndef POPULATION_STRATEGY_H
#define POPULATION_STRATEGY_H

#define POPULATION_STRATEGY_ARGUMENT_TYPE "population_strategy_type"
#define POPULATION_STRATEGY_ARGUMENT_TYPE_CLASSIC "classic"
#define POPULATION_STRATEGY_ARGUMENT_TYPE_ACTIVE "active"
#define POPULATION_STRATEGY_ARGUMENT_TYPE_SINGLE "single"
#define POPULATION_STRATEGY_ARGUMENT_TYPE_DOUBLE "double"
#define POPULATION_STRATEGY_ARGUMENT_TYPE_CONSTANT "constant"

#define POPULATION_STRATEGY_ARGUMENT_NUMBER_OF_ITERATIONS "population_strategy_number_of_iterations"

#define POPULATION_STRATEGY_ARGUMENT_NUMBER_OF_POPULATIONS "population_strategy_number_of_populations"

#include "Individual.h"
#include "Problem.h"

#include <cstdint>
#include <istream>
#include <unordered_set>

using namespace std;


enum EPopulationStrategyType
{
	POPULATION_STRATEGY_CLASSIC = 0,
	POPULATION_STRATEGY_ACTIVE = 1,
	POPULATION_STRATEGY_SINGLE = 2,
	POPULATION_STRATEGY_DOUBLE = 3,
	POPULATION_STRATEGY_CONSTANT = 4
};//enum EPopulationStrategyType


template <class TGenotype, class TFenotype>
class CPopulationStrategy
{
public:
	static uint32_t iERROR_PARENT_CPopulationStrategy;

	CPopulationStrategy();
	CPopulationStrategy(uint8_t iInitNumberOfPopulations);
	CPopulationStrategy(uint8_t iInitNumberOfPopulations, CProblem<TGenotype, TFenotype> *pcProblem);

	virtual ~CPopulationStrategy();

	virtual CError eConfigure(istream *psStream) { return CError(iERROR_PARENT_CPopulationStrategy); };

	virtual void vUpdateNumberOfPopulations(uint32_t iIterationNumber, uint8_t iNumberOfPopulations,
		CIndividual<TGenotype, TFenotype> *pcBestIndividual, CIndividual<TGenotype, TFenotype> **ppcBestPopulationsIndividuals, 
		unordered_set<uint8_t> *psPopulationToRemoveIndexes, uint8_t *piNumberOfPopulationsToCreate) = 0;

	virtual uint8_t iGetInitNumberOfPopulations() { return i_init_number_of_populations; };

protected:
	uint8_t i_init_number_of_populations;
	CProblem<TGenotype, TFenotype> *pc_problem;

private:
	void v_init(uint8_t iInitNumberOfPopulations, CProblem<TGenotype, TFenotype> *pcProblem);
};//class CPopulationStrategy


template <class TGenotype, class TFenotype>
class CConstantPopulationStrategy : public CPopulationStrategy<TGenotype, TFenotype>
{
public:
	CConstantPopulationStrategy();
	CConstantPopulationStrategy(uint8_t iInitNumberOfPopulations);

	virtual CError eConfigure(istream *psStream);

	virtual void vUpdateNumberOfPopulations(uint32_t iIterationNumber, uint8_t iNumberOfPopulations,
		CIndividual<TGenotype, TFenotype> *pcBestIndividual, CIndividual<TGenotype, TFenotype> **ppcBestPopulationsIndividuals,
		unordered_set<uint8_t> *psPopulationToRemoveIndexes, uint8_t *piNumberOfPopulationsToCreate) { };
};//class CConstantPopulationStrategy : public CPopulationStrategy<TGenotype, TFenotype>

#endif//POPULATION_STRATEGY_H