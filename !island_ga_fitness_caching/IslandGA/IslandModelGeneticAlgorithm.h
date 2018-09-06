#ifndef ISLAND_MODEL_GENETIC_ALGORITHM_H
#define ISLAND_MODEL_GENETIC_ALGORITHM_H

#include "Error.h"
#include "IslandModelOptimizer.h"
#include "Log.h"
#include "MigrationReplacement.h"
#include "MigrationSelection.h"
#include "MigrationTopology.h"
#include "MultiPopulationGeneticAlgorithm.h"
#include "Optimizer.h"
#include "Problem.h"

#include <ctime>
#include <cstdint>
#include <istream>

using namespace std;

template <class TGenotype, class TFenotype>
class CIslandModelGeneticAlgorithm : public CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>
{
public:
	CIslandModelGeneticAlgorithm(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CIslandModelGeneticAlgorithm(CIslandModelGeneticAlgorithm<TGenotype, TFenotype> *pcOther);

	virtual ~CIslandModelGeneticAlgorithm();

	virtual COptimizer<TGenotype, TFenotype> *pcCopy() { return new CIslandModelGeneticAlgorithm(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

protected:
	void v_migration(uint32_t iIterationNumber);

	virtual CIndividual<TGenotype, TFenotype> **ppc_get_best_populations_individuals_for_migration()
	{
		return b_only_last_bests ? ppc_get_last_best_populations_individuals() : ppc_get_best_populations_individuals();
	};

	void v_clear_params();

	uint32_t i_migration_frequency;
	uint32_t i_number_of_migrating_individuals;

	CMigrationTopology<TGenotype, TFenotype> *pc_migration_topology;
	CMigrationSelection<TGenotype, TFenotype> *pc_migration_selection;
	CMigrationReplacement<TGenotype, TFenotype> *pc_migration_replacement;
};//class CIslandModelGeneticAlgorithm : public CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>, public CIslandModelOptimizer<TGenotype, TFenotype>

#endif//ISLAND_MODEL_GENETIC_ALGORITHM_H