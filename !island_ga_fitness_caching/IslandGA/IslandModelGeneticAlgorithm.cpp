#include "IslandModelGeneticAlgorithm.h"

#include "BinaryCoding.h"
#include "GeneticAlgorithm.h"
#include "MigrationReplacementUtils.h"
#include "MigrationSelectionUtils.h"
#include "MigrationTopologyUtils.h"
#include "PopulationOptimizer.h"
#include "PopulationStrategyUtils.h"
#include "RealCoding.h"
#include "UIntCommandParam.h"

template <class TGenotype, class TFenotype>
CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::CIslandModelGeneticAlgorithm(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed)
{
	pc_migration_topology = nullptr;
	pc_migration_selection = nullptr;
	pc_migration_replacement = nullptr;
}//CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::CIslandModelGeneticAlgorithm(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

template <class TGenotype, class TFenotype>
CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::CIslandModelGeneticAlgorithm(CIslandModelGeneticAlgorithm<TGenotype, TFenotype> *pcOther)
	: CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>(pcOther)
{
	i_migration_frequency = pcOther->i_migration_frequency;
	i_number_of_migrating_individuals = pcOther->i_number_of_migrating_individuals;
	pc_migration_topology = pcOther->pc_migration_topology;
	pc_migration_selection = pcOther->pc_migration_selection;
	pc_migration_replacement = pcOther->pc_migration_replacement;
}//CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::CIslandModelGeneticAlgorithm(CIslandModelGeneticAlgorithm<TGenotype, TFenotype> *pcOther)

template <class TGenotype, class TFenotype>
CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::~CIslandModelGeneticAlgorithm()
{
	v_clear_params();
}//CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::~CIslandModelGeneticAlgorithm()

template <class TGenotype, class TFenotype>
CError CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	v_clear_params();

	CError c_error = CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_migration_frequency(ISLAND_MODEL_OPTIMIZER_ARGUMENT_MIGRATION_FREQUENCY, 1, UINT32_MAX);
		i_migration_frequency = p_migration_frequency.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_number_of_migrating_individuals(ISLAND_MODEL_OPTIMIZER_ARGUMENT_NUMBER_OF_MIGRATING_INDIVIDUALS);
		i_number_of_migrating_individuals = p_number_of_migrating_individuals.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		pc_migration_topology = MigrationTopologyUtils::pcGetMigrationTopology(pc_problem, pl_gene_patterns, psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		pc_migration_selection = MigrationSelectionUtils::pcGetMigrationSelection(pc_problem, pl_gene_patterns, psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		pc_migration_replacement = MigrationReplacementUtils::pcGetMigrationReplacement(pc_problem, pl_gene_patterns, psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
bool CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	bool b_updated = CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::bRunIteration(iIterationNumber, tStartTime);
	v_migration(iIterationNumber);

	return b_updated;
}//bool CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

template<class TGenotype, class TFenotype>
void CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::v_migration(uint32_t iIterationNumber)
{
	if (iIterationNumber > 0 && iIterationNumber % i_migration_frequency == 0)
	{
		uint8_t i_number_of_populations = (uint8_t)v_populations.size();

		if (i_number_of_populations >= 2)
		{
			uint8_t *pi_migrations_from = new uint8_t[i_number_of_populations];
			uint8_t *pi_migrations_to = new uint8_t[i_number_of_populations];

			CIndividual<TGenotype, TFenotype> **ppc_best_populations_individuals = ppc_get_best_populations_individuals_for_migration();

			pc_migration_topology->vGetMigrations(i_number_of_populations, ppc_best_populations_individuals, pi_migrations_from, pi_migrations_to);

			CIndividual<TGenotype, TFenotype> ***pppc_migrating_individuals = new CIndividual<TGenotype, TFenotype>**[i_number_of_populations];

			CGeneticAlgorithm<TGenotype, TFenotype> *pc_population_from;

			for (uint32_t i = 0; i < i_number_of_populations; i++)
			{
				*(pppc_migrating_individuals + i) = new CIndividual<TGenotype, TFenotype>*[i_number_of_migrating_individuals];

				pc_population_from = (CGeneticAlgorithm<TGenotype, TFenotype>*)v_populations.at(*(pi_migrations_from + i));

				pc_migration_selection->vSelect(pc_population_from->iGetPopulationSize(), pc_population_from->ppcGetPopulation(),
					i_number_of_migrating_individuals, *(pppc_migrating_individuals + i));
			}//for (uint32_t i = 0; i < i_number_of_populations; i++)

			CGeneticAlgorithm<TGenotype, TFenotype> *pc_population_to;

			for (uint32_t i = 0; i < i_number_of_populations; i++)
			{
				pc_population_to = (CGeneticAlgorithm<TGenotype, TFenotype>*)v_populations.at(*(pi_migrations_to + i));

				pc_migration_replacement->vReplace(pc_population_to->iGetPopulationSize(), pc_population_to->ppcGetPopulation(),
					i_number_of_migrating_individuals, *(pppc_migrating_individuals + i));

				delete *(pppc_migrating_individuals + i);
			}//for (uint32_t i = 0; i < i_number_of_populations; i++)

			pc_migration_replacement->vUpdateParams();

			delete pi_migrations_from;
			delete pi_migrations_to;

			delete ppc_best_populations_individuals;

			delete pppc_migrating_individuals;
		}//if (i_number_of_populations >= 2)
	}//if (iIterationNumber > 0 && iIterationNumber % i_migration_frequency == 0)
}//void CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::v_migration(uint32_t iIterationNumber)

template <class TGenotype, class TFenotype>
void CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::v_clear_params()
{
	if (b_own_params)
	{
		delete pc_migration_topology;
		pc_migration_topology = nullptr;

		delete pc_migration_selection;
		pc_migration_selection = nullptr;

		delete pc_migration_replacement;
		pc_migration_replacement = nullptr;
	}//if (b_own_params)
}//void CIslandModelGeneticAlgorithm<TGenotype, TFenotype>::v_clear_params()

template class CIslandModelGeneticAlgorithm<CBinaryCoding, CBinaryCoding>;
template class CIslandModelGeneticAlgorithm<CRealCoding, CRealCoding>;