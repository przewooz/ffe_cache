#include "GeneticAlgorithm.h"

#include "BinaryCoding.h"
#include "CrossoverUtils.h"
#include "Evaluation.h"
#include "MutationUtils.h"
#include "RealCoding.h"
#include "SelectionUtils.h"
#include "UIntCommandParam.h"

#include <algorithm>
#include <atlstr.h>

template <class TGenotype, class TFenotype>
CGeneticAlgorithm<TGenotype, TFenotype>::CGeneticAlgorithm(CProblem<TFenotype, TGenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CPopulationOptimizer<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed)
{
	pc_last_best_individual = nullptr;

	pc_selection = nullptr;
	pc_crossover = nullptr;
	pc_mutation = nullptr;
}//CGeneticAlgorithm<TGenotype, TFenotype>::CGeneticAlgorithm(CProblem<TFenotype, TGenotype> *pcProblem, CLog *pcLog)

template <class TGenotype, class TFenotype>
CGeneticAlgorithm<TGenotype, TFenotype>::CGeneticAlgorithm(CGeneticAlgorithm<TGenotype, TFenotype> *pcOther)
	: CPopulationOptimizer<TGenotype, TFenotype>(pcOther)
{
	pc_last_best_individual = nullptr;

	i_last_best_reset_frequency = pcOther->i_last_best_reset_frequency;

	pc_selection = pcOther->pc_selection;
	pc_crossover = pcOther->pc_crossover;
	pc_mutation = pcOther->pc_mutation;
}//CGeneticAlgorithm<TGenotype, TFenotype>::CGeneticAlgorithm(CGeneticAlgorithm<TGenotype, TFenotype> *pcOther)

template <class TGenotype, class TFenotype>
CGeneticAlgorithm<TGenotype, TFenotype>::~CGeneticAlgorithm()
{
	v_clear_params();
	delete pc_last_best_individual;
}//CGeneticAlgorithm<TGenotype, TFenotype>::~CGeneticAlgorithm()

template <class TGenotype, class TFenotype>
CError CGeneticAlgorithm<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	v_clear_params();

	CError c_error = CPopulationOptimizer<TGenotype, TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		pc_selection = SelectionUtils::pcGetSelection<TGenotype, TFenotype>(pc_problem, pl_gene_patterns, psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		pc_crossover = CrossoverUtils::pcGetCrossover<TGenotype>(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		pc_mutation = MutationUtils::pcGetMutation<TGenotype>(pl_gene_patterns, psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_last_best_reset_frequency(GA_ARGUMENT_LAST_BEST_RESET_FREQUENCY, (uint32_t)1, (uint32_t)UINT32_MAX);
		i_last_best_reset_frequency = p_last_best_reset_frequency.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CGeneticAlgorithm<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
void CGeneticAlgorithm<TGenotype, TFenotype>::vInitialize(time_t tStartTime)
{
	CPopulationOptimizer<TGenotype, TFenotype>::vInitialize(tStartTime);

	delete pc_last_best_individual;
	pc_last_best_individual = nullptr;
}//void CGeneticAlgorithm<TGenotype, TFenotype>::vInitialize(time_t tStartTime)

template <class TGenotype, class TFenotype>
bool CGeneticAlgorithm<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	v_selection();

	v_crossover();
	v_evaluation();

	bool b_updated = b_update_best_individual(iIterationNumber, tStartTime);
	
	v_mutation();
	v_evaluation();

	b_updated = b_update_best_individual(iIterationNumber, tStartTime) || b_updated;

	double d_mean = 0;
	double d_worst = DBL_MAX;
	double d_best = -DBL_MAX;

	CIndividual<TGenotype, TFenotype> *pc_individual;
	double d_fitness_value;

	for (uint32_t i = 0; i < i_population_size; i++)
	{
		pc_individual = *(ppc_population + i);
		d_fitness_value = pc_individual->dGetFitnessValue();

		if (d_worst >= d_fitness_value)
		{
			d_worst = d_fitness_value;
		}//if (d_worst >= d_fitness_value)

		if (d_best <= d_fitness_value)
		{
			d_best = d_fitness_value;
		}//if (d_best <= d_fitness_value)

		d_mean += d_fitness_value;
	}//for (uint32_t i = 0; i < i_population_size; i++)

	d_mean /= (double)i_population_size;

	CString s_log;

	s_log.Format("global best: %f; last best: %f; best: %f; mean: %f; worst: %f", pc_best_individual->dGetFitnessValue(), 
		pc_last_best_individual->dGetFitnessValue(), d_best, d_mean, d_worst);

	pc_log->vPrintLine(s_log, true);

	return b_updated;
}//bool CGeneticAlgorithm<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber)

template <class TGenotype, class TFenotype>
void CGeneticAlgorithm<TGenotype, TFenotype>::v_selection()
{
	CIndividual<TGenotype, TFenotype> **ppc_new_population = new CIndividual<TGenotype, TFenotype>*[i_population_size];

	pc_selection->vSelect(i_population_size, ppc_population, ppc_new_population, pc_best_individual);

	v_replace_population(ppc_new_population);
}//void CGeneticAlgorithm<TGenotype, TFenotype>::v_selection()

template <class TGenotype, class TFenotype>
void CGeneticAlgorithm<TGenotype, TFenotype>::v_crossover()
{
	CIndividual<TGenotype, TFenotype> **ppc_new_population = new CIndividual<TGenotype, TFenotype>*[i_population_size];

	CIndividual<TGenotype, TFenotype> *pc_parent_0, *pc_parent_1, *pc_child_0, *pc_child_1;
	CIndividual<TGenotype, TFenotype> *pc_individual_to_new_population_0, *pc_individual_to_new_population_1;

	random_shuffle(ppc_population + 0, ppc_population + i_population_size);

	for (uint32_t i = 0; i < i_population_size; i += 2)
	{
		pc_parent_0 = *(ppc_population + i + 0);
		pc_parent_1 = *(ppc_population + i + 1);

		if (pc_crossover->bApplyCrossover(pc_parent_0->pcGetGenotype()))
		{
			pc_child_0 = pc_create_individual(pc_generation->pcGenerateEmpty());
			pc_child_1 = pc_create_individual(pc_generation->pcGenerateEmpty());

			pc_parent_0->vCrossover(pc_parent_1, pc_child_0, pc_child_1);

			pc_individual_to_new_population_0 = pc_child_0;
			pc_individual_to_new_population_1 = pc_child_1;
		}//if (pc_crossover->bApplyCrossover(pc_parent_0->pcGetGenotype()))
		else
		{
			 pc_individual_to_new_population_0 = new CIndividual<TGenotype, TFenotype>(pc_parent_0);
			 pc_individual_to_new_population_1 = new CIndividual<TGenotype, TFenotype>(pc_parent_1);
		}//else if (pc_crossover->bApplyCrossover(pc_parent_0->pcGetGenotype()))

		*(ppc_new_population + i + 0) = pc_individual_to_new_population_0;
		*(ppc_new_population + i + 1) = pc_individual_to_new_population_1;
	}//for (uint32_t i = 0; i < i_population_size; i += 2)

	v_replace_population(ppc_new_population);
}//void CGeneticAlgorithm<TGenotype, TFenotype>::v_crossover()

template <class TGenotype, class TFenotype>
void CGeneticAlgorithm<TGenotype, TFenotype>::v_mutation()
{
	CIndividual<TGenotype, TFenotype> *pc_individual;

	for (uint32_t i = 0; i < i_population_size; i++)
	{
		pc_individual = *(ppc_population + i);
		pc_individual->vMutate(i);
	}//for (uint32_t i = 0; i < i_population_size; i++)
}//void CGeneticAlgorithm<TGenotype, TFenotype>::v_mutation()

template <class TGenotype, class TFenotype>
bool CGeneticAlgorithm<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime)
{
	if (iIterationNumber % i_last_best_reset_frequency == 0)
	{
		delete pc_last_best_individual;
		pc_last_best_individual = nullptr;
	}//if (iIterationNumber % i_last_best_reset_frequency == 0)

	CIndividual<TGenotype, TFenotype> *pc_individual;

	for (uint32_t i = 0; i < i_population_size; i++)
	{
		pc_individual = *(ppc_population + i);

		if (!pc_last_best_individual || pc_problem->bIsBetterIndividual(pc_individual, pc_last_best_individual))
		{
			delete pc_last_best_individual;
			pc_last_best_individual = new CIndividual<TGenotype, TFenotype>(pc_individual);
		}//if (!pc_last_best_individual || pc_problem->bIsBetterIndividual(pc_individual, pc_last_best_individual))
	}//for (uint32_t i = 0; i < i_population_size; i++)

	return CPopulationOptimizer<TGenotype, TFenotype>::b_update_best_individual(iIterationNumber, tStartTime);
}//bool CGeneticAlgorithm<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime)

template <class TGenotype, class TFenotype>
void CGeneticAlgorithm<TGenotype, TFenotype>::v_clear_params()
{
	if (b_own_params)
	{
		delete pc_selection;
		pc_selection = nullptr;

		delete pc_crossover;
		pc_crossover = nullptr;

		delete pc_mutation;
		pc_mutation = nullptr;
	}//if (b_own_params)
}//void CGeneticAlgorithm<TGenotype, TFenotype>::v_clear_params()

template class CGeneticAlgorithm<CBinaryCoding, CBinaryCoding>;
template class CGeneticAlgorithm<CRealCoding, CRealCoding>;