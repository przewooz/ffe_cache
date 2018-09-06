#ifndef ISLAND_MODEL_OPTIMIZER
#define ISLAND_MODEL_OPTIMIZER

#define ISLAND_MODEL_OPTIMIZER_ARGUMENT_MIGRATION_FREQUENCY "migration_frequency"

#define ISLAND_MODEL_OPTIMIZER_ARGUMENT_NUMBER_OF_MIGRATING_INDIVIDUALS "number_of_migrating_individuals"

#include "Error.h"
#include "Log.h"
#include "MigrationReplacement.h"
#include "MigrationSelection.h"
#include "MigrationTopology.h"
#include "MultiOptimizer.h"
#include "Optimizer.h"
#include "Problem.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;

template <class TGenotype, class TFenotype>
class CIslandModelOptimizer : public CMultiOptimizer<TGenotype, TFenotype>
{
public:
	CIslandModelOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CIslandModelOptimizer(CIslandModelOptimizer<TGenotype, TFenotype> *pcOther);

	virtual ~CIslandModelOptimizer();

	virtual COptimizer<TGenotype, TFenotype> *pcCopy() { return new CIslandModelOptimizer(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

protected:
	void v_migration(uint32_t iIterationNumber);

	virtual CIndividual<TGenotype, TFenotype> **ppc_get_best_populations_individuals_for_migration() { return ppc_get_best_populations_individuals(); };

	void v_clear_params();

	uint32_t i_migration_frequency;
	uint32_t i_number_of_migrating_individuals;

	CMigrationTopology<TGenotype, TFenotype> *pc_migration_topology;
	CMigrationSelection<TGenotype, TFenotype> *pc_migration_selection;
	CMigrationReplacement<TGenotype, TFenotype> *pc_migration_replacement;
};//class CIslandModelOptimizer : public CMultiOptimizer<TGenotype, TFenotype>

#endif//ISLAND_MODEL_OPTIMIZER