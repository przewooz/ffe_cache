#ifndef ISLAND_P3
#define ISLAND_P3

#define ISLAND_MODEL_P3_ARGUMENT_NUMBER_OF_EXCHANGES "migration_number_of_exchanges"

#define ISLAND_MODEL_P3_ARGUMENT_ADD_TO_TOP "add_to_top"

#include "BinaryCoding.h"
#include "Error.h"
#include "Log.h"
#include "MigrationReplacement.h"
#include "MigrationTopology.h"
#include "MultiOptimizer.h"
#include "Optimizer.h"
#include "Problem.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;

class CMultiDifferentWithGlobalP3;

class CIslandModelP3 : public CMultiOptimizer<CBinaryCoding, CBinaryCoding>
{
friend class CMultiDifferentWithGlobalP3;

public:
	CIslandModelP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CIslandModelP3(CIslandModelP3 *pcOther);

	virtual ~CIslandModelP3();

	virtual CError eConfigure(istream *psSettings);

	virtual COptimizer<CBinaryCoding, CBinaryCoding> *pcCopy() { return new CIslandModelP3(this); };

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

	virtual void vRun();

protected:
	virtual CError e_configure_params_optimizer(istream *psSettings);

	void v_migration(uint32_t iIterationNumber);

	void v_clear_params();

	uint32_t i_migration_frequency;
	uint8_t i_number_of_exchanges;

	CMigrationTopology<CBinaryCoding, CBinaryCoding> *pc_migration_topology;
	CMigrationReplacement<CBinaryCoding, CBinaryCoding> *pc_migration_replacement;

	bool b_get_difference;
	bool b_hill_climb;
	bool b_climb_only_if_added;
	bool b_add_to_top;

	uint32_t i_number_of_migrations;
	uint32_t i_number_of_effective_migrations;
	uint32_t i_number_of_effective_with_non_empty_gene_pattern_migrations;
	uint32_t i_gene_patterns_length_sum;
};//class CIslandModelP3 : public CMultiOptimizer<CBinaryCoding, CBinaryCoding>

#endif//ISLAND_P3