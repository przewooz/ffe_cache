#include "ParticleSwarmOptimization.h"

#include "FloatCommandParam.h"
#include "MathUtils.h"
#include "RandUtils.h"

#include <atlstr.h>

CParticleSwarmOptimization::CParticleSwarmOptimization(CProblem<CRealCoding, CRealCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CPopulationOptimizer<CRealCoding, CRealCoding>(pcProblem, pcLog, iRandomSeed)
{
	pc_center_point = nullptr;

	pc_inertia_weight = nullptr;
}//CParticleSwarmOptimization::CParticleSwarmOptimization(CProblem<CRealCoding, CRealCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

CParticleSwarmOptimization::CParticleSwarmOptimization(CParticleSwarmOptimization *pcOther)
	: CPopulationOptimizer<CRealCoding, CRealCoding>(pcOther)
{
	pc_center_point = pcOther->pc_center_point ? new CRealCoding(pcOther->pc_center_point) : nullptr;

	pc_inertia_weight = pcOther->pc_inertia_weight;

	d_inertia_weight = pcOther->d_inertia_weight;
	d_c_1 = pcOther->d_c_1;
	d_c_2 = pcOther->d_c_2;
}//CParticleSwarmOptimization::CParticleSwarmOptimization(CParticleSwarmOptimization *pcOther)

CParticleSwarmOptimization::~CParticleSwarmOptimization()
{
	delete pc_center_point;
	v_clear_params();
}//CParticleSwarmOptimization::~CParticleSwarmOptimization()

CError CParticleSwarmOptimization::eConfigure(istream *psSettings)
{
	v_clear_params();

	CError c_error = CPopulationOptimizer<CRealCoding, CRealCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		pc_inertia_weight = pcGetInertiaWeight(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CFloatCommandParam p_c_1(PSO_ARGUMENT_C_1);
		d_c_1 = (double)p_c_1.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CFloatCommandParam p_c_2(PSO_ARGUMENT_C_2);
		d_c_2 = (double)p_c_2.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CParticleSwarmOptimization::eConfigure(istream *psSettings)

bool CParticleSwarmOptimization::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	v_update_params(iIterationNumber, tStartTime);

	v_update_particles();
	v_evaluation();

	bool b_updated = b_update_best_individual(iIterationNumber, tStartTime);

	CString s_log_message;
	s_log_message.Format("iteration: %u; time: %u; ffe: %u; best: %f", iIterationNumber, (uint32_t)(time(nullptr) - tStartTime),
		pc_problem->pcGetEvaluation()->iGetFFE(), pc_best_individual->dGetFitnessValue());
	pc_log->vPrintLine(s_log_message, true);

	return b_updated;
}//bool CParticleSwarmOptimization::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

void CParticleSwarmOptimization::vSetCenterPoint(CRealCoding *pcCenterPoint)
{
	delete pc_center_point;
	pc_center_point = new CRealCoding(pcCenterPoint);
}//void CParticleSwarmOptimization::vSetCenterPoint(CRealCoding *pcCenterPoint)

CIndividual<CRealCoding, CRealCoding> * CParticleSwarmOptimization::pc_create_individual(CRealCoding *pcGenotype)
{
	//TODO: create velocity
	double *pd_velocity = new double[pcGenotype->iGetNumberOfDimensions()];

	for (uint16_t i = 0; i < pcGenotype->iGetNumberOfDimensions(); i++)
	{
		if (pc_center_point)
		{
			*(pd_velocity + i) = *(pc_center_point->pdGetValues() + i) - *(pcGenotype->pdGetValues() + i);
			*(pd_velocity + i) *= (double)RandUtils::fRandProbability();
		}//if (pc_center_point)
		else
		{
			*(pd_velocity + i) = (double)RandUtils::fRandProbability();
		}//else if (pc_center_point)
	}//for (uint16_t i = 0; i < pcGenotype->iGetNumberOfDimensions(); i++)

	//if (pc_center_point)
	//{
	//	MathUtils::vNormalize(pd_velocity, pcGenotype->iGetNumberOfDimensions());
	//}//if (pc_center_point)

	return new CParticleSwarmOptimizationIndividual(pcGenotype, pc_problem, pd_velocity, &d_inertia_weight, &d_c_1, &d_c_2);
}//CIndividual<CRealCoding, CRealCoding> * CParticleSwarmOptimization::pc_create_individual(CRealCoding *pcGenotype)

void CParticleSwarmOptimization::v_clear_params()
{
	if (b_own_params)
	{
		delete pc_inertia_weight;
		pc_inertia_weight = nullptr;
	}//if (b_own_params)
}//void CParticleSwarmOptimization::v_clear_params()

void CParticleSwarmOptimization::v_update_particles()
{
	CParticleSwarmOptimizationIndividual *pc_particle;

	for (uint32_t i = 0; i < i_population_size; i++)
	{
		pc_particle = (CParticleSwarmOptimizationIndividual*)(*(ppc_population + i));
		pc_particle->vUpdate(pc_best_individual->pcGetGenotype());
	}//for (uint32_t i = 0; i < i_population_size; i++)
}//void CParticleSwarmOptimization::v_update_particles()

void CParticleSwarmOptimization::v_update_params(uint32_t iIterationNumber, time_t tStartTime)
{
	double d_now = pc_stop_condition->dGetNow(tStartTime, iIterationNumber, pc_problem->pcGetEvaluation()->iGetFFE(), pc_best_individual);
	double d_max = pc_stop_condition->dGetMax();

	d_inertia_weight = pc_inertia_weight->dGetValue(d_now, d_max);
}//void CParticleSwarmOptimization::v_update_params(uint32_t iIterationNumber, time_t tStartTime)


CParticleSwarmOptimizationIndividual::CParticleSwarmOptimizationIndividual(CRealCoding *pcGenotype, CProblem<CRealCoding, CRealCoding> *pcProblem, double *pdVelocity, double *pdInteriaWeight, double *pdC1, double *pdC2)
	: CIndividual<CRealCoding, CRealCoding>(pcGenotype, pcProblem)
{
	pd_velocity = pdVelocity;
	pc_best_genotype = new CRealCoding(pcGenotype);

	pd_inertia_weight = pdInteriaWeight;
	pd_c_1 = pdC1;
	pd_c_2 = pdC2;
}//CParticleSwarmOptimizationIndividual::CParticleSwarmOptimizationIndividual(CRealCoding *pcGenotype, CProblem<CRealCoding, CRealCoding> *pcProblem, double *pdVelocity, double *pdInteriaWeight, double *pdC1, double *pdC2)

CParticleSwarmOptimizationIndividual::~CParticleSwarmOptimizationIndividual()
{
	delete pd_velocity;
	delete pc_best_genotype;
}//CParticleSwarmOptimizationIndividual::~CParticleSwarmOptimizationIndividual()

void CParticleSwarmOptimizationIndividual::vEvaluate()
{
	if (!bIsEvaluated())
	{
		double d_old_fitness_value = dGetFitnessValue();

		CIndividual<CRealCoding, CRealCoding>::vEvaluate();

		if (dGetFitnessValue() > d_old_fitness_value)
		{
			delete pc_best_genotype;
			pc_best_genotype = new CRealCoding(pcGetGenotype());
		}//if (dGetFitnessValue() > d_old_fitness_value)
	}//if (!bIsEvaluated())
}//void CParticleSwarmOptimizationIndividual::vEvaluate()

void CParticleSwarmOptimizationIndividual::vUpdate(CRealCoding *pcLocalBestGenotype)
{
	v_update_velocity(pcLocalBestGenotype);

	for (uint16_t i = 0; i < pcGetGenotype()->iGetNumberOfDimensions(); i++)
	{
		*(pcGetGenotype()->pdGetValues() + i) += *(pd_velocity + i);
	}//for (uint16_t i = 0; i < pcGetGenotype()->iGetNumberOfDimensions(); i++)

	pcGetGenotype()->vRepair();

	vIsEvaluated(false);
}//void CParticleSwarmOptimizationIndividual::vUpdate(CRealCoding *pcLocalBestGenotype)

void CParticleSwarmOptimizationIndividual::v_update_velocity(CRealCoding *pcLocalBestGenotype)
{
	double d_r_1 = (double)RandUtils::fRandProbability();
	double d_r_2 = (double)RandUtils::fRandProbability();

	for (uint16_t i = 0; i < pcGetGenotype()->iGetNumberOfDimensions(); i++)
	{
		*(pd_velocity + i) = *pd_inertia_weight * *(pd_velocity + i);
		*(pd_velocity + i) += d_r_1 * *pd_c_1 * (*(pc_best_genotype->pdGetValues() + i) - *(pcGetGenotype()->pdGetValues() + i));
		*(pd_velocity + i) += d_r_2 * *pd_c_2 * (*(pcLocalBestGenotype->pdGetValues() + i) - *(pcGetGenotype()->pdGetValues() + i));

		//if (*(pd_velocity + i) > *(pcGetGenotype()->pdGetMaxValues() + i))
		//{
		//	*(pd_velocity + i) = *(pcGetGenotype()->pdGetMaxValues() + i);
		//}//if (*(pd_velocity + i) > *(pcGetGenotype()->pdGetMaxValues() + i))

		//if (*(pd_velocity + i) < *(pcGetGenotype()->pdGetMinValues() + i))
		//{
		//	*(pd_velocity + i) = *(pcGetGenotype()->pdGetMinValues() + i);
		//}//if (*(pd_velocity + i) < *(pcGetGenotype()->pdGetMinValues() + i))
	}//for (uint16_t i = 0; i < pcGetGenotype()->iGetNumberOfDimensions(); i++)
}//void CParticleSwarmOptimizationIndividual::v_update_velocity(CRealCoding *pcLocalBestGenotype)