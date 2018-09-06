#include "DifferentialEvolution.h"

#include "BinaryCoding.h"
#include "CrossoverUtils.h"
#include "MutationUtils.h"
#include "RealCoding.h"

#include <atlstr.h>

template <class TGenotype, class TFenotype>
CDifferentialEvolution<TGenotype, TFenotype>::CDifferentialEvolution(CProblem<TFenotype, TGenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CPopulationOptimizer<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed)
{
	pc_crossover = nullptr;
	pc_mutation = nullptr;

	ppc_mutants_trials = nullptr;
}//CDifferentialEvolution<TGenotype, TFenotype>::CDifferentialEvolution(CProblem<TFenotype, TGenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

template <class TGenotype, class TFenotype>
CDifferentialEvolution<TGenotype, TFenotype>::CDifferentialEvolution(CDifferentialEvolution<TGenotype, TFenotype> *pcOther)
	: CPopulationOptimizer<TGenotype, TFenotype>(pcOther)
{
	pc_crossover = pcOther->pc_crossover;
	pc_mutation = pcOther->pc_mutation;

	ppc_mutants_trials = nullptr;
}//CDifferentialEvolution<TGenotype, TFenotype>::CDifferentialEvolution(CDifferentialEvolution<TGenotype, TFenotype> *pcOther)

template <class TGenotype, class TFenotype>
CDifferentialEvolution<TGenotype, TFenotype>::~CDifferentialEvolution()
{
	delete ppc_mutants_trials;
	v_clear_params();
}//CDifferentialEvolution<TGenotype, TFenotype>::~CDifferentialEvolution()

template <class TGenotype, class TFenotype>
CError CDifferentialEvolution<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	v_clear_params();

	CError c_error = CPopulationOptimizer<TGenotype, TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		pc_crossover = CrossoverUtils::pcGetCrossover<TGenotype>(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		pc_mutation = MutationUtils::pcGetMutation<TGenotype>(pl_gene_patterns, psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CDifferentialEvolution<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
void CDifferentialEvolution<TGenotype, TFenotype>::vInitialize(time_t tStartTime)
{
	CPopulationOptimizer<TGenotype, TFenotype>::vInitialize(tStartTime);

	delete ppc_mutants_trials;
	ppc_mutants_trials = new CIndividual<TGenotype, TFenotype>*[i_population_size];

	CString s_log_message;
	s_log_message.Format("init time: %u", (uint32_t)(time(nullptr) - tStartTime));
	pc_log->vPrintLine(s_log_message, true);
}//void CDifferentialEvolution<TGenotype, TFenotype>::vInitialize(time_t tStartTime)

template <class TGenotype, class TFenotype>
bool CDifferentialEvolution<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	v_update_operators();

	v_mutation(ppc_mutants_trials);
	v_crossover(ppc_mutants_trials);
	v_selection(ppc_mutants_trials);

	bool b_updated = b_update_best_individual(iIterationNumber, tStartTime);

	CString s_log_message;
	s_log_message.Format("iteration: %u; time: %u; ffe: %u; best: %f", iIterationNumber, (uint32_t)(time(nullptr) - tStartTime),
		pc_problem->pcGetEvaluation()->iGetFFE(), pc_best_individual->dGetFitnessValue());
	pc_log->vPrintLine(s_log_message, true);

	return b_updated;
}//bool CDifferentialEvolution<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

template <class TGenotype, class TFenotype>
void CDifferentialEvolution<TGenotype, TFenotype>::v_clear_params()
{
	if (b_own_params)
	{
		delete pc_crossover;
		pc_crossover = nullptr;

		delete pc_mutation;
		pc_mutation = nullptr;
	}//if (b_own_params)
}//void CDifferentialEvolution<TGenotype, TFenotype>::v_clear_params()

template <class TGenotype, class TFenotype>
void CDifferentialEvolution<TGenotype, TFenotype>::v_mutation(CIndividual<TGenotype, TFenotype> **ppcMutants)
{
	CIndividual<TGenotype, TFenotype> *pc_mutant;

	for (uint32_t i = 0; i < i_population_size; i++)
	{
		pc_mutant = new CIndividual<TGenotype, TFenotype>(*(ppc_population + i));
		pc_mutant->vMutate(i);

		*(ppcMutants + i) = pc_mutant;
	}//for (uint32_t i = 0; i < i_population_size; i++)
}//void CDifferentialEvolution<TGenotype, TFenotype>::v_mutation(CIndividual<TGenotype, TFenotype> **ppcMutants)

template <class TGenotype, class TFenotype>
void CDifferentialEvolution<TGenotype, TFenotype>::v_crossover(CIndividual<TGenotype, TFenotype> **ppcMutantsTrials)
{
	CIndividual<TGenotype, TFenotype> *pc_individual, *pc_mutant, *pc_trial;

	for (uint32_t i = 0; i < i_population_size; i++)
	{
		pc_individual = *(ppc_population + i);
		pc_mutant = *(ppcMutantsTrials + i);
		pc_trial = pc_create_individual(pc_generation->pcGenerateEmpty());

		pc_individual->vCrossover(pc_mutant, pc_trial, nullptr);

		*(ppcMutantsTrials + i) = pc_trial;

		delete pc_mutant;
	}//for (uint32_t i = 0; i < i_population_size; i++)
}//void CDifferentialEvolution<TGenotype, TFenotype>::v_crossover(CIndividual<TGenotype, TFenotype> **ppcMutantsTrials)

template <class TGenotype, class TFenotype>
void CDifferentialEvolution<TGenotype, TFenotype>::v_selection(CIndividual<TGenotype, TFenotype> **ppcTrials)
{
	CIndividual<TGenotype, TFenotype> *pc_individual, *pc_trial, *pc_better;

	for (uint32_t i = 0; i < i_population_size; i++)
	{
		pc_individual = *(ppc_population + i);
		pc_trial = *(ppcTrials + i);

		pc_individual->vEvaluate();
		pc_trial->vEvaluate();

		pc_better = pc_problem->pcGetBetterIndividual(pc_individual, pc_trial);

		*(ppc_population + i) = pc_better;

		if (pc_individual == pc_better)
		{
			delete pc_trial;
		}//if (pc_individual == pc_better)
		else
		{
			delete pc_individual;
		}//else if (pc_individual == pc_better)
	}//for (uint32_t i = 0; i < i_population_size; i++)
}//void CDifferentialEvolution<TGenotype, TFenotype>::v_selection(CIndividual<TGenotype, TFenotype> **ppcTrials)

template <class TGenotype, class TFenotype>
void CDifferentialEvolution<TGenotype, TFenotype>::v_update_operators()
{
	if (pc_mutation->bPopulationRequired())
	{
		pc_mutation->vUpdatePopulation(i_population_size, pcGetPopulationGenotypes());
	}//if (pc_mutation->bPopulationRequired())

	if (pc_mutation->bBestGenotypeRequired())
	{
		pc_mutation->vUpdateBestGenotype(pc_best_individual->pcGetGenotype());
	}//if (pc_mutation->bBestGenotypeRequired())
}//void CDifferentialEvolution<TGenotype, TFenotype>::v_update_operators()

template class CDifferentialEvolution<CBinaryCoding, CBinaryCoding>;
template class CDifferentialEvolution<CRealCoding, CRealCoding>;