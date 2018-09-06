#ifndef PARTICLE_SWARM_OPTIMIZATION_H
#define PARTICLE_SWARM_OPTIMIZATION_H

#define PSO_ARGUMENT_C_1 "c_1"
#define PSO_ARGUMENT_C_2 "c_2"

#include "Error.h"
#include "Individual.h"
#include "InertiaWeight.h"
#include "Log.h"
#include "PopulationOptimizer.h"
#include "Problem.h"
#include "RealCoding.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace InertiaWeight;

using namespace std;

class CParticleSwarmOptimization : public CPopulationOptimizer<CRealCoding, CRealCoding>
{
public:
	CParticleSwarmOptimization(CProblem<CRealCoding, CRealCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CParticleSwarmOptimization(CParticleSwarmOptimization *pcOther);

	virtual ~CParticleSwarmOptimization();

	virtual COptimizer<CRealCoding, CRealCoding> *pcCopy() { return new CParticleSwarmOptimization(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

	void vSetCenterPoint(CRealCoding* pcCenterPoint);

protected:
	virtual CIndividual<CRealCoding, CRealCoding> *pc_create_individual(CRealCoding *pcGenotype);

	void v_clear_params();

private:
	void v_update_particles();

	void v_update_params(uint32_t iIterationNumber, time_t tStartTime);

	CRealCoding *pc_center_point;

	CInertiaWeight *pc_inertia_weight;

	double d_inertia_weight;
	double d_c_1;
	double d_c_2;
};//class CParticleSwarmOptimization : CPopulationOptimizer<CRealCoding, CRealCoding>


class CParticleSwarmOptimizationIndividual : public CIndividual<CRealCoding, CRealCoding>
{
public:
	CParticleSwarmOptimizationIndividual(CRealCoding *pcGenotype, CProblem<CRealCoding, CRealCoding> *pcProblem, 
		double *pdVelocity, double *pdInteriaWeight, double *pdC1, double *pdC2);

	virtual ~CParticleSwarmOptimizationIndividual();

	virtual void vEvaluate();

	void vUpdate(CRealCoding *pcLocalBestGenotype);

private:
	void v_update_velocity(CRealCoding *pcLocalBestGenotype);

	double *pd_velocity;
	CRealCoding *pc_best_genotype;

	double *pd_inertia_weight;
	double *pd_c_1;
	double *pd_c_2;

	//TODO: scaling factor
};//class CParticleSwarmOptimizationIndividual : CIndividual<CRealCoding, CRealCoding>

#endif//PARTICLE_SWARM_OPTIMIZATION_H