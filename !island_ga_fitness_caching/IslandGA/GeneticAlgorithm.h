#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#define GA_ARGUMENT_GENE_PATTERN_MIN_LENGTH "gene_pattern_min_length"

#define GA_ARGUMENT_LINKAGE_FREQUENCY "linkage_frequency"

#define GA_ARGUMENT_LAST_BEST_RESET_FREQUENCY "last_best_reset_frequency"

#include "Crossover.h"
#include "Error.h"
#include "Individual.h"
#include "Log.h"
#include "Mutation.h"
#include "PopulationOptimizer.h"
#include "Problem.h"
#include "Selection.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;

template <class TGenotype, class TFenotype>
class CGeneticAlgorithm : public CPopulationOptimizer<TGenotype, TFenotype>
{
public:
	CGeneticAlgorithm(CProblem<TFenotype, TGenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CGeneticAlgorithm(CGeneticAlgorithm<TGenotype, TFenotype> *pcOther);
	
	virtual ~CGeneticAlgorithm();

	virtual CGeneticAlgorithm<TGenotype, TFenotype> *pcCopy() { return new CGeneticAlgorithm<TGenotype, TFenotype>(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

	CGeneration<TGenotype> *pcGetGeneration() { return pc_generation; };
	CMutation<TGenotype> *pcGetMutation() { return pc_mutation; };

	CIndividual<TGenotype, TFenotype> *pcGetLastBestIndividual() { return pc_last_best_individual; };

protected:
	void v_clear_params();

private:
	void v_selection();
	void v_crossover();
	void v_mutation();

	virtual bool b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime);

	virtual CIndividual<TGenotype, TFenotype> *pc_create_individual(TGenotype *pcGenotype)
	{
		return new CIndividual<TGenotype, TFenotype>(pcGenotype, pc_problem->pcGetEvaluation(), pc_crossover, pc_mutation, pc_problem->pcGetTransformation());
	};

	CIndividual<TGenotype, TFenotype> *pc_last_best_individual;

	uint32_t i_last_best_reset_frequency;

	CSelection<TGenotype, TFenotype> *pc_selection;
	CCrossover<TGenotype> *pc_crossover;
	CMutation<TGenotype> *pc_mutation;
};//class CGeneticAlgorithm

#endif//GENETIC_ALGORITHM_H