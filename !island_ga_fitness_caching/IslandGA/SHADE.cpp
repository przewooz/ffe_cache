#include "SHADE.h"

#include "FloatCommandParam.h"
#include "UIntCommandParam.h"

CSHADE::CSHADE(CProblem<CRealCoding, CRealCoding> *pcProblem, CLog *pcLog)
	: COptimizer<CRealCoding, CRealCoding>(pcProblem, pcLog)
{
	pc_shade = nullptr;
}//CSHADE::CSHADE(CProblem<CRealCoding, CRealCoding> *pcProblem, CLog *pcLog)

CSHADE::CSHADE(CSHADE *pcOther)
	: COptimizer<CRealCoding, CRealCoding>(pcOther)
{
	pc_shade = nullptr;
}//CSHADE::CSHADE(CSHADE *pcOther)

CSHADE::~CSHADE()
{
	delete pc_shade;
}//CSHADE::~CSHADE()

CError CSHADE::eConfigure(istream *psSettings)
{
	delete pc_shade;
	pc_shade = nullptr;

	CError c_error = COptimizer<CRealCoding, CRealCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_population_size("population_size", 2, UINT32_MAX);
		i_population_size = p_population_size.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CFloatCommandParam p_arc_rate("arc_rate");
		d_arc_rate = (double)p_arc_rate.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CFloatCommandParam p_best_rate("best_rate");
		d_best_rate = (double)p_best_rate.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		pc_shade = new SHADE(pc_problem, pc_stop_condition, (int)i_population_size, d_arc_rate, (double)d_best_rate);
	}//if (!c_error)

	return c_error;
}//CError CSHADE::eConfigure(istream *psSettings)

void CSHADE::vRun()
{
	double d_best_fitness = (double)pc_shade->run();

	delete pc_best_individual;
	
	CRealCoding *pc_sample_real_coding = pc_problem->pcGetEvaluation()->pcCreateSampleFenotype();
	pc_sample_real_coding->vSetValues(new double[pc_sample_real_coding->iGetNumberOfDimensions()], true);

	pc_best_individual = pc_create_individual(pc_sample_real_coding);
	pc_best_individual->vSetFitnessValue(d_best_fitness);
}//void CSHADE::vRun()