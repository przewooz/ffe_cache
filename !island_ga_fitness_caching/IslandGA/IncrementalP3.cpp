#include "IncrementalP3.h"

#include "CommandParam.h"
#include "P3.h"

#include <vector>

CIncrementalP3::CIncrementalP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CIncrementalOptimizer<CBinaryCoding, CBinaryCoding>(pcProblem, pcLog, iRandomSeed)
{

}//CIncrementalP3::CIncrementalP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

CIncrementalP3::CIncrementalP3(CIncrementalP3 *pcOther)
	: CIncrementalOptimizer<CBinaryCoding, CBinaryCoding>(pcOther)
{

}//CIncrementalP3::CIncrementalP3(CIncrementalP3 *pcOther)

CError CIncrementalP3::eConfigure(istream *psSettings)
{
	CError c_error = CIncrementalOptimizer<CBinaryCoding, CBinaryCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		CBoolCommandParam p_add_only_last_best(INCREMENTAL_P3_ARGUMENT_ADD_ONLY_LAST_BEST);
		b_add_only_last_best = p_add_only_last_best.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CBoolCommandParam p_climb(INCREMENTAL_P3_ARGUMENT_CLIMB);
		b_climb = p_climb.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CIncrementalP3::eConfigure(istream *psSettings)

bool CIncrementalP3::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	bool b_updated = CIncrementalOptimizer<CBinaryCoding, CBinaryCoding>::bRunIteration(iIterationNumber, tStartTime);

	CP3 *pc_p3 = (CP3*)pc_optimizer;

	uint32_t i_number_of_best_individuals_to_add = b_add_only_last_best ? 1 : (uint32_t)v_best_individuals.size();

	CIndividual<CBinaryCoding, CBinaryCoding> *pc_best_individual_to_add;

	for (uint32_t i = 0; i < i_number_of_best_individuals_to_add; i++)
	{
		pc_best_individual_to_add = v_best_individuals.at(v_best_individuals.size() - 1 - i);

		//vector<bool> v_p3_individual;
		CLtgaIndividual c_new_ind;
		c_new_ind.v_genotype.reserve(pc_best_individual_to_add->pcGetGenotype()->iGetNumberOfBits());
		//v_p3_individual.reserve(pc_best_individual_to_add->pcGetGenotype()->iGetNumberOfBits());

		for (uint16_t i = 0; i < pc_best_individual_to_add->pcGetGenotype()->iGetNumberOfBits(); i++)
		{
			c_new_ind.v_genotype.push_back(*(pc_best_individual_to_add->pcGetGenotype()->piGetBits() + i) == 1);
		}//for (uint16_t i = 0; i < pc_best_individual_to_add->pcGetGenotype()->iGetNumberOfBits(); i++)

		//float f_p3_individual_fitness_value = pc_p3->pc_recorder->evaluate(v_p3_individual);

		if (b_climb)
		{
			pc_p3->pc_p3->climb(c_new_ind);
		}//if (b_climb)
		else
		{
			pc_p3->pc_p3->add_unique(c_new_ind, 0);
		}//else if (b_climb)
	}//for (uint32_t i = 0; i < i_number_of_best_individuals_to_add; i++)

	return b_updated;
}//bool CIncrementalP3::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

CError CIncrementalP3::e_configure_params_optimizer(istream *psSettings)
{
	pc_optimizer = new CP3(pc_problem, pc_log, i_random_seed);
	return pc_optimizer->eConfigure(psSettings);
}//CError CIncrementalP3::e_configure_params_optimizer(istream *psSettings)