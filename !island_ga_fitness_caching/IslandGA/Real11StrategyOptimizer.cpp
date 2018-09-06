#include "Real11StrategyOptimizer.h"

#include "FloatCommandParam.h"
#include "RandUtils.h"
#include "UIntCommandParam.h"

#include <cfloat>

CReal11StrategyOptimizer::CReal11StrategyOptimizer(CProblem<CRealCoding, CRealCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: COptimizer<CRealCoding, CRealCoding>(pcProblem, pcLog, iRandomSeed)
{

}//CReal11StrategyOptimizer::CReal11StrategyOptimizer(CProblem<CRealCoding, CRealCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

CReal11StrategyOptimizer::CReal11StrategyOptimizer(CReal11StrategyOptimizer *pcOther)
	: COptimizer<CRealCoding, CRealCoding>(pcOther)
{
	d_initial_sigma = pcOther->d_initial_sigma;
	d_sigma_change_increase_ratio = pcOther->d_sigma_change_increase_ratio;
	d_sigma_change_decrease_ratio = pcOther->d_sigma_change_decrease_ratio;
}//CReal11StrategyOptimizer::CReal11StrategyOptimizer(CReal11StrategyOptimizer *pcOther)

CError CReal11StrategyOptimizer::eConfigure(istream *psSettings)
{
	CError c_error = COptimizer<CRealCoding, CRealCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		CFloatCommandParam p_initial_sigma(REAL_11_STRATEGY_OPTIMIZER_ARGUMENT_INITIAL_SIGMA, 0, FLT_MAX);
		d_initial_sigma = (double)p_initial_sigma.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CFloatCommandParam p_sigma_change_increase_ratio(REAL_11_STRATEGY_OPTIMIZER_ARGUMENT_SIGMA_CHANGE_INCREASE_RATIO, 1, FLT_MAX);
		d_sigma_change_increase_ratio = (double)p_sigma_change_increase_ratio.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CFloatCommandParam p_sigma_change_decrease_ratio(REAL_11_STRATEGY_OPTIMIZER_ARGUMENT_SIGMA_CHANGE_DECREASE_RATIO, 0, 1);
		d_sigma_change_decrease_ratio = (double)p_sigma_change_decrease_ratio.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_sigma_change_frequency(REAL_11_STRATEGY_OPTIMIZER_ARGUMENT_SIGMA_CHANGE_FREQUENCY, 5, UINT8_MAX);
		i_sigma_change_frequency = (uint8_t)p_sigma_change_frequency.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CReal11StrategyOptimizer::eConfigure(istream psSettings)

void CReal11StrategyOptimizer::vInitialize(time_t tStartTime)
{
	i_number_of_successes = 0;
	d_sigma = d_initial_sigma;
}//void CReal11StrategyOptimizer::vInitialize(time_t tStartTime)

bool CReal11StrategyOptimizer::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	bool b_updated = false;

	CRealCoding *pc_trial_genotype = pc_empty_generation->pcGenerateEmpty();

	for (uint16_t i = 0; i < pc_trial_genotype->iGetNumberOfDimensions(); i++)
	{
		*(pc_trial_genotype->pdGetValues() + i) = *(pc_best_individual->pcGetGenotype()->pdGetValues() + i)
			+ d_sigma * RandUtils::dRandGaussianNumber();
	}//for (uint16_t i = 0; i < pc_trial_genotype->iGetNumberOfDimensions(); i++)

	pc_trial_genotype->vRepair();

	CIndividual<CRealCoding, CRealCoding> *pc_trial = pc_create_individual(pc_trial_genotype);
	pc_trial->vEvaluate();

	if (pc_problem->bIsBetterIndividual(pc_trial, pc_best_individual))
	{
		//pc_log->vPrintLine(pc_trial->pcGetFenotype()->sToString());

		vSetBestIndividual(pc_trial, false);
		b_updated = true;
	}//if (pc_problem->bIsBetterIndividual(pc_trial, pc_best_individual))
	else
	{
		delete pc_trial;
	}//else if (pc_problem->bIsBetterIndividual(pc_trial, pc_best_individual))

	v_update_params(iIterationNumber, b_updated);

	return b_updated;
}//bool CReal11StrategyOptimizer::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

void CReal11StrategyOptimizer::vRun()
{
	COptimizer<CRealCoding, CRealCoding>::vRun();
}//void CReal11StrategyOptimizer::vRun()

void CReal11StrategyOptimizer::v_update_params(uint32_t iIterationNumber, bool bUpdated)
{
	//cout << iIterationNumber << " " << pc_best_individual->dGetFitnessValue() << endl;

	if (bUpdated)
	{
		i_number_of_successes++;
	}//if (bUpdated)

	if (iIterationNumber > 0 && iIterationNumber % i_sigma_change_frequency == 0)
	{
		double d_success_ratio = (double)i_number_of_successes / (double)i_sigma_change_frequency;

		if (d_success_ratio > REAL_11_STRATEGY_OPTIMIZER_1_5_RATIO)
		{
			d_sigma *= d_sigma_change_increase_ratio;
		}//if (d_success_ratio > REAL_11_STRATEGY_OPTIMIZER_1_5_RATIO)
		else if (d_success_ratio < REAL_11_STRATEGY_OPTIMIZER_1_5_RATIO)
		{
			d_sigma *= d_sigma_change_decrease_ratio;
		}//else if (d_success_ratio < REAL_11_STRATEGY_OPTIMIZER_1_5_RATIO)

		i_number_of_successes = 0;
	}//if (iIterationNumber > 0 && iIterationNumber % i_sigma_change_frequency == 0)
}//void CReal11StrategyOptimizer::v_update_params(uint32_t iIterationNumber, bool bUpdated)