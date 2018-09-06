#include "MultiDifferentWithGlobalP3.h"

#include "IslandModelP3.h"
#include "UIntCommandParam.h"

CMultiDifferentWithGlobalP3::CMultiDifferentWithGlobalP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CMultiDifferentOptimizer<CBinaryCoding, CBinaryCoding>(pcProblem, pcLog, iRandomSeed), c_global_p3(pcProblem, pcLog, iRandomSeed)
{

}//CMultiDifferentWithGlobalP3::CMultiDifferentWithGlobalP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

CMultiDifferentWithGlobalP3::CMultiDifferentWithGlobalP3(CMultiDifferentWithGlobalP3 *pcOther)
	: CMultiDifferentOptimizer<CBinaryCoding, CBinaryCoding>(pcOther), c_global_p3(pcOther->pc_problem, pcOther->pc_log, pcOther->i_random_seed)
{

}//CMultiDifferentWithGlobalP3::CMultiDifferentWithGlobalP3(CMultiDifferentWithGlobalP3 *pcOther)

CError CMultiDifferentWithGlobalP3::eConfigure(istream *psSettings)
{
	CError c_error = CMultiDifferentOptimizer<CBinaryCoding, CBinaryCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		c_error = c_global_p3.eConfigure(psSettings);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_global_run_frequency(MULTI_DIFFERENT_WITH_GLOBAL_P3_ARGUMENT_GLOBAL_RUN_FREQUENCY, 1, INT32_MAX);
		i_global_run_frequency = p_global_run_frequency.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CMultiDifferentWithGlobalP3::eConfigure(istream *psSettings)

void CMultiDifferentWithGlobalP3::vInitialize(time_t tStartTime)
{
	CMultiDifferentOptimizer<CBinaryCoding, CBinaryCoding>::vInitialize(tStartTime);
	c_global_p3.vInitialize(tStartTime);
}//void CMultiDifferentWithGlobalP3::vInitialize(time_t tStartTime)

bool CMultiDifferentWithGlobalP3::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	bool b_updated = CMultiDifferentOptimizer::bRunIteration(iIterationNumber, tStartTime);

	if (iIterationNumber > 0 && iIterationNumber % i_global_run_frequency == 0)
	{
		time_t t_global_p3_start_time = time(nullptr);
		uint32_t i_global_p3_iteration_number = 0;
		uint64_t i_global_p3_start_ffe = pc_problem->pcGetEvaluation()->iGetFFE();

		CIslandModelP3 *pc_island_p3;

		for (uint8_t i = 0; i < (uint8_t)v_populations.size(); i++)
		{
			pc_island_p3 = (CIslandModelP3*)v_populations.at(i);

			for (uint8_t j = 0; j < pc_island_p3->v_populations.size(); j++)
			{
				c_global_p3.bClimb(pc_island_p3->v_populations.at(j)->pcGetBestIndividual()->pcGetGenotype(), iIterationNumber, tStartTime);
			}//for (uint8_t j = 0; j < pc_island_p3->v_populations.size(); j++)
		}//for (uint8_t i = 0; i < (uint8_t)v_populations.size(); i++)

		b_updated = b_update_best_individual(iIterationNumber, tStartTime, c_global_p3.pcGetBestIndividual()) || b_updated;

		while (!pc_stop_condition->bStop(tStartTime, iIterationNumber, pc_problem->pcGetEvaluation()->iGetFFE(), pc_best_individual) && !c_global_p3.pcGetStopCondition()->bStop(t_global_p3_start_time, i_global_p3_iteration_number, pc_problem->pcGetEvaluation()->iGetFFE() - i_global_p3_start_ffe, pc_best_individual))
		{
			if (c_global_p3.bRunIteration(i_global_p3_iteration_number, t_global_p3_start_time))
			{
				b_updated = b_update_best_individual(iIterationNumber, tStartTime, c_global_p3.pcGetBestIndividual()) || b_updated;
			}//if (c_global_p3.bRunIteration(i_global_p3_iteration_number, t_global_p3_start_time))
			
			i_global_p3_iteration_number++;
		}//while (!pc_stop_condition->bStop(tStartTime, iIterationNumber, pc_problem->pcGetEvaluation()->iGetFFE(), pc_best_individual) && !c_global_p3.pcGetStopCondition()->bStop(t_global_p3_start_time, i_global_p3_iteration_number, pc_problem->pcGetEvaluation()->iGetFFE() - i_global_p3_start_ffe, pc_best_individual))
	}//if (iIterationNumber > 0 && iIterationNumber % i_global_run_frequency == 0)

	return b_updated;
}//bool CMultiDifferentWithGlobalP3::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

COptimizer<CBinaryCoding, CBinaryCoding> * CMultiDifferentWithGlobalP3::pc_get_single_optimizer(istream *psSettings, CError *pcError)
{
	CIslandModelP3 *pc_island_p3 = new CIslandModelP3(pc_problem, pc_log, i_random_seed);
	*pcError = pc_island_p3->eConfigure(psSettings);

	return pc_island_p3;
}//COptimizer<CBinaryCoding, CBinaryCoding> * CMultiDifferentWithGlobalP3::pc_get_single_optimizer(istream *psSettings, CError *pcError)