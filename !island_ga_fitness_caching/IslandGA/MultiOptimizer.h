#ifndef MULTI_OPTIMIZER_H
#define MULTI_OPTIMIZER_H

#define MULTI_OPTIMIZER_ARGUMENT_LINKAGE_ITERATIONS_FROM_LAST_BEST "linkage_iterations_from_last_best"

#include "Error.h"
#include "Individual.h"
#include "Log.h"
#include "Optimizer.h"
#include "PopulationStrategy.h"
#include "Problem.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;

template <class TGenotype, class TFenotype>
class CMultiOptimizer : public COptimizer<TGenotype, TFenotype>
{
public:
	CMultiOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CMultiOptimizer(CMultiOptimizer<TGenotype, TFenotype> *pcOther);
	
	virtual ~CMultiOptimizer();

	virtual COptimizer<TGenotype, TFenotype> *pcCopy() { return new CMultiOptimizer<TGenotype, TFenotype>(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

protected:
	virtual CError e_configure_params_optimizer(istream *psSettings);

	using COptimizer<TGenotype, TFenotype>::b_update_best_individual;
	virtual bool b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime);

	void v_clear_populations();
	void v_clear_params();

	void v_run_populations_iteration(uint32_t iIterationNumber, time_t tStartTime);
	void v_linkage(uint32_t iIterationNumber);
	void v_handle_populations(uint32_t iIterationNumber, time_t tStartTime);

	CIndividual<TGenotype, TFenotype> **ppc_get_best_populations_individuals();

	virtual CIndividual<TGenotype, TFenotype> **ppc_get_best_populations_individuals_for_linkage() { return ppc_get_best_populations_individuals(); };
	virtual uint8_t i_get_best_populations_individuals_for_linkage_size() { return (uint8_t)v_populations.size(); };

	virtual CIndividual<TGenotype, TFenotype> **ppc_get_best_populations_individuals_for_handling_populations()  { return ppc_get_best_populations_individuals(); };

	vector<COptimizer<TGenotype, TFenotype>*> v_populations;
	vector<uint32_t> v_populations_iterations_from_last_update;

	uint32_t i_linkage_iterations_from_last_best;

	CPopulationStrategy<TGenotype, TFenotype> *pc_population_strategy;

	COptimizer<TGenotype, TFenotype> *pc_params_optimizer;
};//class CMultiOptimizer : public COptimizer<TGenotype, TFenotype>

#endif//MULTI_OPTIMIZER_H