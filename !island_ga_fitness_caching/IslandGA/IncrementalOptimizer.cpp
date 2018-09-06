#include "IncrementalOptimizer.h"

#include "BinaryCoding.h"
#include "OptimizerUtils.h"
#include "RealCoding.h"

#include <atlstr.h>

template <class TGenotype, class TFenotype>
CIncrementalOptimizer<TGenotype, TFenotype>::CIncrementalOptimizer(CProblem<TGenotype, TFenotype>* pcProblem, CLog* pcLog, uint32_t iRandomSeed)
	: COptimizer<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed)
{
	pc_optimizer = nullptr;
}//CIncrementalOptimizer<TGenotype, TFenotype>::CIncrementalOptimizer(CProblem<TGenotype, TFenotype>* pcProblem, CLog* pcLog, uint32_t iRandomSeed)

template <class TGenotype, class TFenotype>
CIncrementalOptimizer<TGenotype, TFenotype>::CIncrementalOptimizer(CIncrementalOptimizer<TGenotype, TFenotype> *pcOther)
	: COptimizer<TGenotype, TFenotype>(pcOther)
{
	pc_optimizer = pcOther->pc_optimizer;
}//CIncrementalOptimizer<TGenotype, TFenotype>::CIncrementalOptimizer(CIncrementalOptimizer<TGenotype, TFenotype> *pcOther)

template <class TGenotype, class TFenotype>
CIncrementalOptimizer<TGenotype, TFenotype>::~CIncrementalOptimizer()
{
	v_clear_params();

	for (uint16_t i = 0; i < (uint16_t)v_best_individuals.size(); i++)
	{
		delete v_best_individuals.at(i);
	}//for (uint16_t i = 0; i < (uint16_t)v_best_individuals.size(); i++)

	v_best_individuals.clear();
}//CIncrementalOptimizer<TGenotype, TFenotype>::~CIncrementalOptimizer()

template <class TGenotype, class TFenotype>
CError CIncrementalOptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	v_clear_params();

	CError c_error = COptimizer<TGenotype, TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		c_error = e_configure_params_optimizer(psSettings);
	}//if (!c_error)

	return c_error;
}//CError CIncrementalOptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
bool CIncrementalOptimizer<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	uint32_t i_inner_iteration_number = 0;
	time_t t_inner_start_time = time(nullptr);

	while (!pc_stop_condition->bStop(tStartTime, iIterationNumber, pc_problem->pcGetEvaluation()->iGetFFE(), pc_best_individual) && !pc_optimizer->pcGetStopCondition()->bStop(t_inner_start_time, i_inner_iteration_number, pc_problem->pcGetEvaluation()->iGetFFE(), pc_optimizer->pcGetBestIndividual()))
	{
		pc_optimizer->bRunIteration(i_inner_iteration_number, t_inner_start_time);
		i_inner_iteration_number++;
	}//while (!pc_stop_condition->bStop(tStartTime, iIterationNumber, pc_problem->pcGetEvaluation()->iGetFFE(), pc_best_individual) && !pc_optimizer->pcGetStopCondition()->bStop(t_inner_start_time, i_inner_iteration_number, pc_problem->pcGetEvaluation()->iGetFFE(), pc_optimizer->pcGetBestIndividual()))

	bool b_updated = b_update_best_individual(iIterationNumber, tStartTime, pc_optimizer->pcGetBestIndividual());

	CString s_log;
	s_log.Format("iteration: %d; time: %u; best: %f; ffe: %u", iIterationNumber, (uint32_t)(time(nullptr) - tStartTime),
		pc_best_individual->dGetFitnessValue(), pc_problem->pcGetEvaluation()->iGetFFE());

	pc_log->vPrintLine(s_log, true);
	pc_log->vPrintEmptyLine(true);

	v_best_individuals.push_back(new CIndividual<TGenotype, TFenotype>(pc_optimizer->pcGetBestIndividual()));

	pc_optimizer->vInitialize(time(nullptr));

	return b_updated;
}//bool CIncrementalOptimizer<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

template <class TGenotype, class TFenotype>
CError CIncrementalOptimizer<TGenotype, TFenotype>::e_configure_params_optimizer(istream *psSettings)
{
	CError c_error;

	pc_optimizer = OptimizerUtils::pcGetOptimizer(pc_problem, pc_log, i_random_seed, psSettings, &c_error);

	if (!c_error && pc_optimizer)
	{
		pc_optimizer->vSetGenePatterns(pl_gene_patterns);
	}//if (!c_error && pc_optimizer)

	return c_error;
}//CError CIncrementalOptimizer<TGenotype, TFenotype>::e_configure_params_optimizer(istream *psSettings)

template <class TGenotype, class TFenotype>
void CIncrementalOptimizer<TGenotype, TFenotype>::v_clear_params()
{
	if (b_own_params)
	{
		delete pc_optimizer;
	}//if (b_own_params)
}//void CIncrementalOptimizer<TGenotype, TFenotype>::v_clear_params()

template class CIncrementalOptimizer<CBinaryCoding, CBinaryCoding>;
template class CIncrementalOptimizer<CRealCoding, CRealCoding>;