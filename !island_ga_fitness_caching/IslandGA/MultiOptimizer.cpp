#include "MultiOptimizer.h"

#include "BinaryCoding.h"
#include "OptimizerUtils.h"
#include "PopulationStrategyUtils.h"
#include "RealCoding.h"
#include "UIntCommandParam.h"

template <class TGenotype, class TFenotype>
CMultiOptimizer<TGenotype, TFenotype>::CMultiOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: COptimizer<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed)
{
	pc_population_strategy = nullptr;
	pc_params_optimizer = nullptr;
}//CMultiOptimizer<TGenotype, TFenotype>::CMultiOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

template <class TGenotype, class TFenotype>
CMultiOptimizer<TGenotype, TFenotype>::CMultiOptimizer(CMultiOptimizer<TGenotype, TFenotype> *pcOther)
	: COptimizer<TGenotype, TFenotype>(pcOther)
{
	i_linkage_iterations_from_last_best = pcOther->i_linkage_iterations_from_last_best;

	pc_population_strategy = pcOther->pc_population_strategy;
	pc_params_optimizer = pcOther->pc_params_optimizer;
}//CMultiOptimizer<TGenotype, TFenotype>::CMutliOptimizer(CMultiOptimizer<TGenotype, TFenotype> *pcOther)

template  <class TGenotype, class TFenotype>
CMultiOptimizer<TGenotype, TFenotype>::~CMultiOptimizer()
{
	v_clear_populations();
	v_clear_params();
}//CMultiOptimizer<TGenotype, TFenotype>::~CMultiOptimizer()

template <class TGenotype, class TFenotype>
CError CMultiOptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	v_clear_params();

	CError c_error = COptimizer<TGenotype, TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		c_error = e_configure_params_optimizer(psSettings);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_linkage_iterations_from_last_best(MULTI_OPTIMIZER_ARGUMENT_LINKAGE_ITERATIONS_FROM_LAST_BEST);
		i_linkage_iterations_from_last_best = p_linkage_iterations_from_last_best.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		pc_population_strategy = PopulationStrategyUtils::pcGetPopulationStrategy(pc_problem, psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CMultiOptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
void CMultiOptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)
{
	COptimizer<TGenotype, TFenotype>::vInitialize(tStartTime);

	uint8_t i_init_number_of_populations = pc_population_strategy->iGetInitNumberOfPopulations();

	v_clear_populations();

	v_populations.reserve(i_init_number_of_populations);

	COptimizer<TGenotype, TFenotype> *pc_optimizer;

	for (uint8_t i = 0; i < i_init_number_of_populations; i++)
	{
		pc_optimizer = pc_params_optimizer->pcCopy();
		pc_optimizer->vInitialize(tStartTime);

		v_populations.push_back(pc_optimizer);
		v_populations_iterations_from_last_update.push_back(0);
	}//for (uint8_t i = 0; i < i_init_number_of_populations; i++)

	b_update_best_individual(0, tStartTime);
}//void CMultiOptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)

template <class TGenotype, class TFenotype>
bool CMultiOptimizer<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	v_run_populations_iteration(iIterationNumber, tStartTime);
	v_linkage(iIterationNumber);
	v_handle_populations(iIterationNumber, tStartTime);

	bool b_updated = b_update_best_individual(iIterationNumber, tStartTime);

	CString s_log;
	s_log.Format("iteration: %d; time: %u; number of populations: %d; best: %f", iIterationNumber, (uint32_t)(time(nullptr) - tStartTime),
		v_populations.size(), pc_best_individual->dGetFitnessValue());

	pc_log->vPrintLine(s_log, true);
	pc_log->vPrintEmptyLine(true);

	return b_updated;
}//bool CMultiOptimizer<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

template <class TGenotype, class TFenotype>
CError CMultiOptimizer<TGenotype, TFenotype>::e_configure_params_optimizer(istream *psSettings)
{
	CError c_error;

	pc_params_optimizer = OptimizerUtils::pcGetOptimizer(pc_problem, pc_log, i_random_seed, psSettings, &c_error);

	if (!c_error && pc_params_optimizer && !pc_params_optimizer->plGetGenePatterns())
	{
		pc_params_optimizer->vSetGenePatterns(pl_gene_patterns);
	}//if (!c_error && pc_params_optimizer && !pc_params_optimizer->plGetGenePatterns())

	return c_error;
}//CError CMultiOptimizer<TGenotype, TFenotype>::e_configure_params_optimizer(istream *psSettings)

template <class TGenotype, class TFenotype>
bool CMultiOptimizer<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime)
{
	COptimizer<TGenotype, TFenotype> *pc_optimizer;
	CIndividual<TGenotype, TFenotype> *pc_optimizer_best_individual;

	bool b_updated = false;

	for (uint8_t i = 0; i < (uint8_t)v_populations.size(); i++)
	{
		pc_optimizer = v_populations.at(i);
		pc_optimizer_best_individual = pc_optimizer->pcGetBestIndividual();

		if (pc_optimizer_best_individual)
		{
			if (!pc_best_individual || pc_problem->bIsBetterIndividual(pc_optimizer_best_individual, pc_best_individual))
			{
				delete pc_best_individual;
				pc_best_individual = new CIndividual<TGenotype, TFenotype>(pc_optimizer_best_individual);

				b_updated = true;
			}//if (!pc_best_individual || pc_problem->bIsBetterIndividual(pc_optimizer_best_individual, pc_best_individual))
		}//if (pc_optimizer_best_individual)
	}//for (uint8_t i = 0; i < (uint8_t)v_populations.size(); i++)

	if (b_updated)
	{
		t_best_time = time(nullptr) - tStartTime;
		i_best_ffe = pc_problem->pcGetEvaluation()->iGetFFE();

		CString s_log;
		s_log.Format("new best found: %f", pc_optimizer_best_individual->dGetFitnessValue());

		pc_log->vPrintEmptyLine(true);
		pc_log->vPrintLine(s_log, true);
		pc_log->vPrintEmptyLine(true);
		pc_log->vPrintLine(pc_optimizer_best_individual->pcGetFenotype()->sToString(), false);
		pc_log->vPrintEmptyLine(false);
	}//if (b_updated)

	return b_updated;
}//bool CMultiOptimizer<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime)

template <class TGenotype, class TFenotype>
void CMultiOptimizer<TGenotype, TFenotype>::v_clear_populations()
{
	for (uint8_t i = 0; i < (uint8_t)v_populations.size(); i++)
	{
		delete v_populations.at(i);
	}//for (uint8_t i = 0; i < (uint8_t)v_populations.size(); i++)

	v_populations.clear();
	v_populations_iterations_from_last_update.clear();
}//void CMultiOptimizer<TGenotype, TFenotype>::v_clear_populations()

template <class TGenotype, class TFenotype>
void CMultiOptimizer<TGenotype, TFenotype>::v_clear_params()
{
	if (b_own_params)
	{
		delete pc_population_strategy;
		pc_population_strategy = nullptr;

		delete pc_params_optimizer;
		pc_params_optimizer = nullptr;
	}//if (b_own_params)
}//void CMultiOptimizer<TGenotype, TFenotype>::v_clear_params()

template <class TGenotype, class TFenotype>
void CMultiOptimizer<TGenotype, TFenotype>::v_run_populations_iteration(uint32_t iIterationNumber, time_t tStartTime)
{
	COptimizer<TGenotype, TFenotype> *pc_optimizer;

	for (uint8_t i = 0; i < (uint8_t)v_populations.size(); i++)
	{
		pc_optimizer = v_populations.at(i);

		if (!pc_optimizer->bRunIteration(iIterationNumber, tStartTime))
		{
			v_populations_iterations_from_last_update.at(i)++;
		}//if (!pc_optimizer->bRunIteration(iIterationNumber, tStartTime))
	}//for (uint8_t i = 0; i < (uint8_t)v_populations.size(); i++)
}//void CMultiOptimizer<TGenotype, TFenotype>::v_run_populations_iteration(uint32_t iIterationNumber, time_t tStartTime)

template <class TGenotype, class TFenotype>
void CMultiOptimizer<TGenotype, TFenotype>::v_linkage(uint32_t iIterationNumber)
{
	if (pc_linkage && pl_gene_patterns)
	{
		if (iIterationNumber > 0 && iIterationNumber % i_linkage_frequency == 0)
		{
			uint8_t i_number_of_populations = (uint8_t)v_populations.size();

			if (i_number_of_populations >= 2)
			{
				CIndividual<TGenotype, TFenotype> **ppc_best_populations_individuals = ppc_get_best_populations_individuals_for_linkage();
				uint8_t i_individuals_per_population = i_get_best_populations_individuals_for_linkage_size() / (uint8_t)v_populations.size();

				CIndividual<TGenotype, TFenotype> *pc_best_population_individual_0;
				CIndividual<TGenotype, TFenotype> *pc_best_population_individual_1;

				CGenePattern *pc_gene_pattern;

				for (uint8_t i = 0; i < i_number_of_populations * i_individuals_per_population; i++)
				{
					if (v_populations_iterations_from_last_update.at(i / i_individuals_per_population) >= i_linkage_iterations_from_last_best)
					{
						pc_best_population_individual_0 = *(ppc_best_populations_individuals + i);

						for (uint8_t j = i + 1; j < i_number_of_populations * i_individuals_per_population; j++)
						{
							if (v_populations_iterations_from_last_update.at(j / i_individuals_per_population) >= i_linkage_iterations_from_last_best)
							{
								pc_best_population_individual_1 = *(ppc_best_populations_individuals + j);

								pc_gene_pattern = pc_linkage->pcGetGenePattern(pc_best_population_individual_0->pcGetGenotype(),
									pc_best_population_individual_1->pcGetGenotype());

								if (pc_gene_pattern->iGetSize() >= i_gene_pattern_min_length)
								{
									pl_gene_patterns->vAdd(pc_gene_pattern);
								}//if (pc_gene_pattern->iGetSize() >= i_gene_pattern_min_length)
								else
								{
									delete pc_gene_pattern;
								}//else if (pc_gene_pattern->iGetSize() > 0)
							}//if (v_populations_iterations_from_last_update.at(j / i_individuals_per_population) >= i_linkage_iterations_from_last_best)
						}//for (uint8_t j = i + 1; j < i_number_of_populations * i_individuals_per_population; j++)
					}//if (v_populations_iterations_from_last_update.at(i / i_individuals_per_population) >= i_linkage_iterations_from_last_best)
				}//for (uint8_t i = 0; i < i_number_of_populations * i_individuals_per_population; i++)

				delete ppc_best_populations_individuals;
			}//if (i_number_of_populations >= 2)
		}//if (iIterationNumber > 0 && iIterationNumber % i_linkage_frequency == 0)
	}//if (pc_linkage && pl_gene_patterns)
}//void CMultiOptimizer<TGenotype, TFenotype>::v_linkage(uint32_t iIterationNumber)

template <class TGenotype, class TFenotype>
void CMultiOptimizer<TGenotype, TFenotype>::v_handle_populations(uint32_t iIterationNumber, time_t tStartTime)
{
	uint8_t i_number_of_populations = (uint8_t)v_populations.size();

	CIndividual<TGenotype, TFenotype> **ppc_best_populations_individuals = ppc_get_best_populations_individuals_for_handling_populations();

	unordered_set<uint8_t> s_population_to_remove_indexes(i_number_of_populations);
	uint8_t i_number_of_populations_to_create = 0;

	pc_population_strategy->vUpdateNumberOfPopulations(iIterationNumber, i_number_of_populations, pc_best_individual,
		ppc_best_populations_individuals, &s_population_to_remove_indexes, &i_number_of_populations_to_create);

	for (int16_t i = (int16_t)v_populations.size() - 1; i >= 0; i--)
	{
		if (s_population_to_remove_indexes.count((uint8_t)i) > 0)
		{
			delete v_populations.at(i);
			v_populations.erase(v_populations.begin() + i);
			v_populations_iterations_from_last_update.erase(v_populations_iterations_from_last_update.begin() + i);
		}//if (s_population_to_remove_indexes.count((uint8_t)i) > 0)
	}//for (int16_t i = (int16_t)v_populations.size() - 1; i >= 0; i--)

	COptimizer<TGenotype, TFenotype> *pc_optimizer;

	for (uint8_t i = 0; i < i_number_of_populations_to_create; i++)
	{
		pc_optimizer = pc_params_optimizer->pcCopy();
		pc_optimizer->vInitialize(tStartTime);

		v_populations.push_back(pc_optimizer);
		v_populations_iterations_from_last_update.push_back(0);
	}//for (uint8_t i = 0; i < i_number_of_populations_to_create; i++)

	delete ppc_best_populations_individuals;

	s_population_to_remove_indexes.clear();
}//void CMultiOptimizer<TGenotype, TFenotype>::v_handle_populations(uint32_t iIterationNumber, time_t tStartTime)

template<class TGenotype, class TFenotype>
CIndividual<TGenotype, TFenotype> ** CMultiOptimizer<TGenotype, TFenotype>::ppc_get_best_populations_individuals()
{
	uint8_t i_number_of_populations = (uint8_t)v_populations.size();

	CIndividual<TGenotype, TFenotype> **ppc_best_populations_individuals = new CIndividual<TGenotype, TFenotype>*[i_number_of_populations];

	COptimizer<TGenotype, TFenotype> *pc_optimizer;

	for (uint8_t i = 0; i < i_number_of_populations; i++)
	{
		pc_optimizer = v_populations.at(i);
		*(ppc_best_populations_individuals + i) = pc_optimizer->pcGetBestIndividual();
	}//for (uint8_t i = 0; i < i_number_of_populations; i++)

	return ppc_best_populations_individuals;
}//CIndividual<TGenotype, TFenotype> ** CMultiOptimizer<TGenotype, TFenotype>::ppc_get_best_populations_individuals()

template class CMultiOptimizer<CBinaryCoding, CBinaryCoding>;
template class CMultiOptimizer<CRealCoding, CRealCoding>;