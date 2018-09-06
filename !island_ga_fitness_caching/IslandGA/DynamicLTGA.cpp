#include "DynamicLTGA.h"

#include "LTGA.h"

#include <limits>
#include <utility>

using namespace DynamicOptimizer;

CDynamicLTGA::CDynamicLTGA(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CDynamicFromStaticOptimizer<CBinaryCoding, CBinaryCoding>(pcProblem, pcLog, iRandomSeed)
{

}//CDynamicLTGA::CDynamicLTGA(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

void CDynamicLTGA::vChangeState(time_t tStartTime, uint64_t iStartFFE)
{
	CDynamicFromStaticOptimizer<CBinaryCoding, CBinaryCoding>::vChangeState(tStartTime, iStartFFE);

	CLTGA *pc_ltga = (CLTGA*)pc_static_optimizer;

	pc_ltga->pc_recorder->best_fitness = (numeric_limits<int>::min)();
	pc_ltga->pc_recorder->evaluations = 0;
	pc_ltga->pc_recorder->start_time = tStartTime;

	pc_ltga->pc_ltga->fitnesses.clear();

	for (size_t i = 0; i < pc_ltga->pc_ltga->pop.solutions.size(); i++)
	{
		pc_ltga->pc_recorder->evaluate(pc_ltga->pc_ltga->pop.solutions.at(i).v_genotype);
	}//for (size_t i = 0; i < pc_ltga->pc_ltga->pop.solutions.size(); i++)

	pc_ltga->b_update_best_individual(0, tStartTime);
}//void CDynamicLTGA::vChangeState(time_t tStartTime, uint64_t iStartFFE)

CError CDynamicLTGA::e_configure_static_optimizer(istream *psSettings)
{
	pc_static_optimizer = new CLTGA(pc_problem, pc_log, i_random_seed);
	return pc_static_optimizer->eConfigure(psSettings);
}//CError CDynamicLTGA::e_configure_static_optimizer(istream *psSettings)