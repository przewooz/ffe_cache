#include "IslandModelP3.h"

#include "BinaryDifferentialLinkage.h"
#include "CommandParam.h"
#include "IslandModelOptimizer.h"
#include "GenePattern.h"
#include "MigrationReplacementUtils.h"
#include "MigrationTopologyUtils.h"
#include "P3.h"
#include "PopulationStrategyUtils.h"
#include "UIntCommandParam.h"

#include <atlstr.h>
#include <vector>

CIslandModelP3::CIslandModelP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CMultiOptimizer<CBinaryCoding, CBinaryCoding>(pcProblem, pcLog, iRandomSeed)
{
	pc_migration_topology = nullptr;
	pc_migration_replacement = nullptr;
}//CIslandModelP3::CIslandModelP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

CIslandModelP3::CIslandModelP3(CIslandModelP3 *pcOther)
	: CMultiOptimizer<CBinaryCoding, CBinaryCoding>(pcOther)
{
	i_migration_frequency = pcOther->i_migration_frequency;

	pc_migration_topology = pcOther->pc_migration_topology;
	pc_migration_replacement = pcOther->pc_migration_replacement;

	b_get_difference = pcOther->b_get_difference;
	b_hill_climb = pcOther->b_hill_climb;
	b_climb_only_if_added = pcOther->b_climb_only_if_added;
	b_add_to_top = pcOther->b_add_to_top;
}//CIslandModelP3::CIslandModelP3(CIslandModelP3 *pcOther)

CIslandModelP3::~CIslandModelP3()
{
	v_clear_params();
}//CIslandModelP3::~CIslandModelP3()

CError CIslandModelP3::eConfigure(istream *psSettings)
{
	v_clear_params();

	CError c_error = CMultiOptimizer<CBinaryCoding, CBinaryCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_migration_frequency(ISLAND_MODEL_OPTIMIZER_ARGUMENT_MIGRATION_FREQUENCY, 1, UINT32_MAX);
		i_migration_frequency = p_migration_frequency.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_number_of_exchanges(ISLAND_MODEL_P3_ARGUMENT_NUMBER_OF_EXCHANGES, 1, UINT8_MAX);
		i_number_of_exchanges = p_number_of_exchanges.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		pc_migration_topology = MigrationTopologyUtils::pcGetMigrationTopology(pc_problem, pl_gene_patterns, psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		pc_migration_replacement = MigrationReplacementUtils::pcGetMigrationReplacement(pc_problem, pl_gene_patterns, psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CBoolCommandParam p_get_difference("get_difference");
		b_get_difference = p_get_difference.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CBoolCommandParam p_hill_climb("hill_climb");
		b_hill_climb = p_hill_climb.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CBoolCommandParam p_climb_only_if_added("climb_only_if_added");
		b_climb_only_if_added = p_climb_only_if_added.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CBoolCommandParam p_add_to_top(ISLAND_MODEL_P3_ARGUMENT_ADD_TO_TOP);
		b_add_to_top = p_add_to_top.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CIslandModelP3::eConfigure(istream *psSettings)

void CIslandModelP3::vInitialize(time_t tStartTime)
{
	CMultiOptimizer::vInitialize(tStartTime);

	i_number_of_migrations = 0;
	i_number_of_effective_migrations = 0;
	i_number_of_effective_with_non_empty_gene_pattern_migrations = 0;
	i_gene_patterns_length_sum = 0;
}//void CIslandModelP3::vInitialize(time_t tStartTime)

bool CIslandModelP3::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	bool b_updated = CMultiOptimizer::bRunIteration(iIterationNumber, tStartTime);
	v_migration(iIterationNumber);

	return b_updated;
}//bool CIslandModelP3::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

void CIslandModelP3::vRun()
{
	CMultiOptimizer::vRun();

	CString s_migration_summary_log;
	s_migration_summary_log.Format("number of migrations: %d; number of effective migrations: %d (%f); number of effective with non empty gene pattern migrations: %d (%f); average gene pattern length: %f",
		i_number_of_migrations, i_number_of_effective_migrations, (float)i_number_of_effective_migrations / (float)i_number_of_migrations,
		i_number_of_effective_with_non_empty_gene_pattern_migrations, (float)i_number_of_effective_with_non_empty_gene_pattern_migrations / (float)i_number_of_effective_migrations,
		(float)i_gene_patterns_length_sum / (float)i_number_of_effective_migrations);
	pc_log->vPrintLine(s_migration_summary_log, true);
}//void CIslandModelP3::vRun()

CError CIslandModelP3::e_configure_params_optimizer(istream *psSettings)
{
	pc_params_optimizer = new CP3(pc_problem, pc_log, i_random_seed);
	return pc_params_optimizer->eConfigure(psSettings);
}//CError CIslandModelP3::e_configure_params_optimizer(istream *psSettings)

void CIslandModelP3::v_migration(uint32_t iIterationNumber)
{
	if (iIterationNumber > 0 && iIterationNumber % i_migration_frequency == 0)
	{
		uint8_t i_number_of_populations = (uint8_t)v_populations.size();

		if (i_number_of_populations >= 2)
		{
			CBinaryDifferentialLinkage c_binary_differential_linkage;

			uint8_t *pi_migrations_from = new uint8_t[i_number_of_populations];
			uint8_t *pi_migrations_to = new uint8_t[i_number_of_populations];

			CIndividual<CBinaryCoding, CBinaryCoding> **ppc_best_populations_individuals = ppc_get_best_populations_individuals();

			pc_migration_topology->vGetMigrations(i_number_of_populations, ppc_best_populations_individuals, pi_migrations_from, pi_migrations_to);

			CIndividual<CBinaryCoding, CBinaryCoding> *pc_migrating_individual_from, *pc_migrating_individual_to;

			CIndividual<CBinaryCoding, CBinaryCoding> *pc_migrating_individual_to_copy_to_gene_pattern;

			uint8_t i_from, i_to;

			CP3 *pc_p3_from, *pc_p3_to;

			for (uint8_t i = 0; i < i_number_of_populations; i++)
			{
				i_from = *(pi_migrations_from + i);
				i_to = *(pi_migrations_to + i);

				pc_p3_from = (CP3*)v_populations.at(i_from);
				pc_p3_to = (CP3*)v_populations.at(i_to);

				for (uint8_t j = 0; j < i_number_of_exchanges; j++)
				{
					//CString s_linkage_report_before = pc_p3_to->s_create_linkage_report();

					pc_migrating_individual_from = new CIndividual<CBinaryCoding, CBinaryCoding>(*(ppc_best_populations_individuals + i_from));
					pc_migrating_individual_to = new CIndividual<CBinaryCoding, CBinaryCoding>(*(ppc_best_populations_individuals + i_to));

					if (b_get_difference)
					{
						pc_migrating_individual_to_copy_to_gene_pattern = new CIndividual<CBinaryCoding, CBinaryCoding>(pc_migrating_individual_to);
					}//if (b_get_difference)

					pc_migration_replacement->vReplace(1, &pc_migrating_individual_to, 1, &pc_migrating_individual_from);

					//vector<bool> v_p3_individual;
					//v_p3_individual.reserve(pc_migrating_individual_to->pcGetGenotype()->iGetNumberOfBits());
					CLtgaIndividual  c_new_ind;
					c_new_ind.v_genotype.reserve(pc_migrating_individual_to->pcGetGenotype()->iGetNumberOfBits());

					//CGenePattern *pc_gene_pattern = c_binary_differential_linkage.pcGetGenePattern(pc_migrating_individual_to->pcGetGenotype(),
					//	pc_migrating_individual_to_copy_to_gene_pattern->pcGetGenotype());

					for (uint16_t k = 0; k < pc_migrating_individual_to->pcGetGenotype()->iGetNumberOfBits(); k++)
					{
						if (b_get_difference)
						{
							if (*(pc_migrating_individual_to->pcGetGenotype()->piGetBits() + k) != *(pc_migrating_individual_to_copy_to_gene_pattern->pcGetGenotype()->piGetBits() + k))
							{
								c_new_ind.v_genotype.push_back(*(pc_migrating_individual_to->pcGetGenotype()->piGetBits() + k) == 1);
							}//if (*(pc_migrating_individual_to->pcGetGenotype()->piGetBits() + k) != *(pc_migrating_individual_to_copy_to_gene_pattern->pcGetGenotype()->piGetBits() + k))
							else
							{
								c_new_ind.v_genotype.push_back(RandUtils::iRandNumber(0, 1) == 1);
							}//else if (*(pc_migrating_individual_to->pcGetGenotype()->piGetBits() + k) != *(pc_migrating_individual_to_copy_to_gene_pattern->pcGetGenotype()->piGetBits() + k))
						}//if (b_get_difference)
						else
						{
							c_new_ind.v_genotype.push_back(*(pc_migrating_individual_to->pcGetGenotype()->piGetBits() + k) == 1);
						}//else if (b_get_difference)
					}//for (uint16_t k = 0; k < pc_migrating_individual_to->pcGetGenotype()->iGetNumberOfBits(); k++)

					//float f_p3_individual_fitness_value = pc_p3_to->pc_recorder->evaluate(v_p3_individual);

					bool b_effective;

					if (b_add_to_top)
					{
						b_effective = pc_p3_to->pc_p3->climb(c_new_ind, pc_p3_to->pc_p3->pops.size() - 1, b_hill_climb, b_climb_only_if_added);
					}//if (b_add_to_top)
					else
					{
						b_effective = pc_p3_to->pc_p3->climb(c_new_ind, 0, b_hill_climb, b_climb_only_if_added);
					}//else if (b_add_to_top)

					i_number_of_migrations++;

					if (b_effective)
					{
						//CString s_from_to;
						//s_from_to.Format("FROM: %d (best at %d); TO: %d (best at %d)", i_from, (uint32_t)pc_p3_from->tGetBestTime(), i_to, (uint32_t)pc_p3_to->tGetBestTime());

						//CString s_iteration;
						//s_iteration.Format("ITERATION: %d", iIterationNumber);

						//pc_log->vPrintLine("MIGRATION_START");
						//pc_log->vPrintEmptyLine();
						//pc_log->vPrintLine(s_iteration);
						//pc_log->vPrintEmptyLine();
						//pc_log->vPrintLine(s_from_to);
						//pc_log->vPrintEmptyLine();
						//pc_log->vPrintLine("GENOTYPE BEFORE");
						//pc_log->vPrintLine(pc_migrating_individual_to_copy_to_gene_pattern->pcGetGenotype()->sToString(10));
						//pc_log->vPrintEmptyLine();
						//pc_log->vPrintLine("GENOTYPE AFTER");
						//pc_log->vPrintLine(pc_migrating_individual_to->pcGetGenotype()->sToString(10));
						//pc_log->vPrintEmptyLine();
						//pc_log->vPrintLine("GENE PATTERN");
						//pc_log->vPrintLine(pc_gene_pattern->sToString());
						//pc_log->vPrintEmptyLine();
						//pc_log->vPrintLine("LINKAGE BEFORE");
						//pc_log->vPrintLine(s_linkage_report_before);
						//pc_log->vPrintEmptyLine();
						//pc_log->vPrintLine("LINKAGE AFTER");
						//pc_log->vPrintLine(pc_p3_to->s_create_linkage_report());
						//pc_log->vPrintEmptyLine();

						//if (pl_gene_patterns)
						//{
						//	pc_log->vPrintLine("GENE PATTERN POOL");

						//	for (uint32_t i = 0; i < pl_gene_patterns->iGetNumberOfGenePatterns(); i++)
						//	{
						//		pc_log->vPrintLine((*(pl_gene_patterns->ppcGetGenePatterns() + i))->sToString());
						//	}//for (uint32_t i = 0; i < pl_gene_patterns->iGetNumberOfGenePatterns(); i++)

						//	pc_log->vPrintEmptyLine();
						//}//if (pl_gene_patterns)

						//pc_log->vPrintLine("MIGRATION_END");
						//pc_log->vPrintEmptyLine();

						//if (pc_gene_pattern->iGetSize() > 0)
						//{
						//	i_number_of_effective_with_non_empty_gene_pattern_migrations++;
						//}//if (pc_gene_pattern->iGetSize() > 0)

						//i_gene_patterns_length_sum += pc_gene_pattern->iGetSize();

						i_number_of_effective_migrations++;
					}//if (b_effective)

					delete pc_migrating_individual_to;

					if (b_get_difference)
					{
						delete pc_migrating_individual_to_copy_to_gene_pattern;
					}//if (b_get_difference)
					//delete pc_gene_pattern;
				}//for (uint8_t j = 0; j < i_number_of_exchanges; j++)
			}//for (uint32_t i = 0; i < i_number_of_populations; i++)

			pc_migration_replacement->vUpdateParams();

			delete pi_migrations_from;
			delete pi_migrations_to;

			delete ppc_best_populations_individuals;
		}//if (i_number_of_populations >= 2)
	}//if (iIterationNumber > 0 && iIterationNumber % i_migration_frequency == 0)
}//void CIslandModelP3::v_migration(uint32_t iIterationNumber)

void CIslandModelP3::v_clear_params()
{
	if (b_own_params)
	{
		delete pc_migration_topology;
		pc_migration_topology = nullptr;

		delete pc_migration_replacement;
		pc_migration_replacement = nullptr;
	}//if (b_own_params)
}//void CIslandModelP3::v_clear_params()