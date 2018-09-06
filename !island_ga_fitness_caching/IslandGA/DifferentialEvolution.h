#ifndef DIFFERENTIAL_EVOLUTION_H
#define DIFFERENTIAL_EVOLUTION_H

#include "Error.h"
#include "Crossover.h"
#include "Mutation.h"
#include "Individual.h"
#include "Optimizer.h"
#include "PopulationOptimizer.h"
#include "Problem.h"

#include <ctime>
#include <cstdint>
#include <istream>

using namespace std;

template <class TGenotype, class TFenotype>
class CDifferentialEvolution : public CPopulationOptimizer<TGenotype, TFenotype>
{
public:
	CDifferentialEvolution(CProblem<TFenotype, TGenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CDifferentialEvolution(CDifferentialEvolution<TGenotype, TFenotype> *pcOther);

	virtual ~CDifferentialEvolution();

	virtual COptimizer<TGenotype, TFenotype> *pcCopy() { return new CDifferentialEvolution<TGenotype, TFenotype>(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

protected:
	virtual CIndividual<TGenotype, TFenotype> *pc_create_individual(TGenotype *pcGenotype)
	{
		return new CIndividual<TGenotype, TFenotype>(pcGenotype, pc_problem->pcGetEvaluation(), pc_crossover, pc_mutation, pc_problem->pcGetTransformation());
	};

	void v_clear_params();

private:
	void v_mutation(CIndividual<TGenotype, TFenotype> **ppcMutants);
	void v_crossover(CIndividual<TGenotype, TFenotype> **ppcMutantsTrials);
	void v_selection(CIndividual<TGenotype, TFenotype> **ppcTrials);

	void v_update_operators();

	CCrossover<TGenotype> *pc_crossover;
	CMutation<TGenotype> *pc_mutation;

	CIndividual<TGenotype, TFenotype> **ppc_mutants_trials;
};//class CDifferentialEvolution

#endif//DIFFERENTIAL_EVOLUTION_H