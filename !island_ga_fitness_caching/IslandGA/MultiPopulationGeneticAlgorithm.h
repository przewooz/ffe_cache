#ifndef MULTI_POPULATION_GENETIC_ALGORITHM_H
#define MULTI_POPULATION_GENETIC_ALGORITHM_H

#define MGA_ARGUMENT_ONLY_LAST_BESTS "only_last_bests"

#include "Error.h"
#include "Individual.h"
#include "Log.h"
#include "MultiOptimizer.h"
#include "Optimizer.h"
#include "Problem.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;

template <class TGenotype, class TFenotype>
class CMultiPopulationGeneticAlgorithm : public CMultiOptimizer<TGenotype, TFenotype>
{
public:
	CMultiPopulationGeneticAlgorithm(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CMultiPopulationGeneticAlgorithm(CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype> *pcOther);

	virtual COptimizer<TGenotype, TFenotype> *pcCopy() { return new CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

protected:
	virtual CError e_configure_params_optimizer(istream *psSettings);

	void v_update_operators();

	CIndividual<TGenotype, TFenotype> **ppc_get_last_best_populations_individuals();
	CIndividual<TGenotype, TFenotype> **ppc_get_all_best_populations_individuals();

	virtual CIndividual<TGenotype, TFenotype> **ppc_get_best_populations_individuals_for_linkage() { return ppc_get_all_best_populations_individuals(); };
	virtual uint8_t i_get_best_populations_individuals_for_linkage_size() { return 2 * (uint8_t)v_populations.size(); };

	virtual CIndividual<TGenotype, TFenotype> **ppc_get_best_populations_individuals_for_handling_populations() 
	{ 
		return b_only_last_bests ? ppc_get_last_best_populations_individuals() : ppc_get_best_populations_individuals();
	};

	bool b_only_last_bests;
};//class CMultiPopulationGeneticAlgorithm

#endif//MULTI_POPULATION_GENETIC_ALGORITHM_H