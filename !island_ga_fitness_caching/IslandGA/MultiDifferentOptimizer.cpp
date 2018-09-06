#include "MultiDifferentOptimizer.h"

#include "BinaryCoding.h"
#include "RealCoding.h"
#include "UIntCommandParam.h"

#include <atlstr.h>

template <class TGenotype, class TFenotype>
CMultiDifferentOptimizer<TGenotype, TFenotype>::CMultiDifferentOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CMultiOptimizer<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed)
{

}//CMultiDifferentOptimizer<TGenotype, TFenotype>::CMultiDifferentOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

template <class TGenotype, class TFenotype>
CMultiDifferentOptimizer<TGenotype, TFenotype>::CMultiDifferentOptimizer(CMultiDifferentOptimizer<TGenotype, TFenotype> *pcOther)
	: CMultiOptimizer<TGenotype, TFenotype>(pcOther)
{

}//CMultiDifferentOptimizer<TGenotype, TFenotype>::CMultiDifferentOptimizer(CMultiDifferentOptimizer<TGenotype, TFenotype> *pcOther)

template <class TGenotype, class TFenotype>
CError CMultiDifferentOptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	CMultiOptimizer<TGenotype, TFenotype>::v_clear_params();
	CMultiOptimizer<TGenotype, TFenotype>::v_clear_populations();

	CError c_error = COptimizer<TGenotype, TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_number_of_optimizers(MULTI_DIFFERENT_OPTIMIZER_ARGUMENT_NUMBER_OF_OPTIMIZERS, 1, UINT8_MAX);
		uint8_t i_number_of_optimizers = p_number_of_optimizers.iGetValue(psSettings, &c_error);

		if (!c_error)
		{
			v_populations.reserve(i_number_of_optimizers);
			v_populations_iterations_from_last_update.reserve(i_number_of_optimizers);

			c_error = e_configure_optimizers(i_number_of_optimizers, psSettings);
		}//if (!c_error)
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_linkage_iterations_from_last_best(MULTI_OPTIMIZER_ARGUMENT_LINKAGE_ITERATIONS_FROM_LAST_BEST);
		i_linkage_iterations_from_last_best = p_linkage_iterations_from_last_best.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CMultiDifferentOptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
void CMultiDifferentOptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)
{
	for (uint8_t i = 0; i < (uint8_t)v_populations.size(); i++)
	{
		v_populations.at(i)->vInitialize(tStartTime);
	}//for (uint8_t i = 0; i < (uint8_t)v_populations.size(); i++)

	b_update_best_individual(0, tStartTime);
}//void CMultiDifferentOptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)

template <class TGenotype, class TFenotype>
bool CMultiDifferentOptimizer<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	v_run_populations_iteration(iIterationNumber, tStartTime);
	v_linkage(iIterationNumber);

	bool b_updated = b_update_best_individual(iIterationNumber, tStartTime);

	CString s_log;
	s_log.Format("iteration: %d; time: %u; number of populations: %d; best: %f", iIterationNumber, (uint32_t)(time(nullptr) - tStartTime),
		v_populations.size(), pc_best_individual->dGetFitnessValue());

	pc_log->vPrintLine(s_log, true);
	pc_log->vPrintEmptyLine(true);

	return b_updated;
}//bool CMultiDifferentOptimizer<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

template <class TGenotype, class TFenotype>
CError CMultiDifferentOptimizer<TGenotype, TFenotype>::e_configure_optimizers(uint8_t iNumberOfOptimizers, istream *psSettings)
{
	CError c_error;

	COptimizer<TGenotype, TFenotype> *pc_single_optimizer;

	for (uint8_t i = 0; i < iNumberOfOptimizers && !c_error; i++)
	{
		pc_single_optimizer = pc_get_single_optimizer(psSettings, &c_error);

		if (!c_error)
		{
			if (!pc_single_optimizer->plGetGenePatterns())
			{
				pc_single_optimizer->vSetGenePatterns(pl_gene_patterns);
			}//if (!pc_single_optimizer->plGetGenePatterns())
		}//if (!c_error)

		v_populations.push_back(pc_single_optimizer);
		v_populations_iterations_from_last_update.push_back(0);
	}//for (uint8_t i = 0; i < iNumberOfOptimizers && !c_error; i++)

	return c_error;
}//CError CMultiDifferentOptimizer<TGenotype, TFenotype>::e_configure_optimizers(uint8_t iNumberOfOptimizers, istream *psSettings)

template class CMultiDifferentOptimizer<CBinaryCoding, CBinaryCoding>;
template class CMultiDifferentOptimizer<CRealCoding, CRealCoding>;