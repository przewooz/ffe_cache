#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#define OPTIMIZER_ARGUMENT_TYPE "optimizer_type"
#define OPTIMIZER_ARGUMENT_TYPE_MULTI "multi"
#define OPTIMIZER_ARGUMENT_TYPE_GA "ga"
#define OPTIMIZER_ARGUMENT_TYPE_MGA "mga"
#define OPTIMIZER_ARGUMENT_TYPE_ISLAND_GA "island_ga"
#define OPTIMIZER_ARGUMENT_TYPE_DE "de"
#define OPTIMIZER_ARGUMENT_TYPE_PSO "pso"
#define OPTIMIZER_ARGUMENT_TYPE_P3 "p3"
#define OPTIMIZER_ARGUMENT_TYPE_ISLAND_P3 "island_p3"
#define OPTIMIZER_ARGUMENT_TYPE_INCREMENTAL_P3 "incremental_p3"
#define OPTIMIZER_ARGUMENT_TYPE_MULTI_DIFFERENT_WITH_GLOBAL_P3 "multi_different_with_global_p3"
#define OPTIMIZER_ARGUMENT_TYPE_LTGA "ltga"
#define OPTIMIZER_ARGUMENT_TYPE_DSMGA2 "dsmga2"
#define OPTIMIZER_ARGUMENT_TYPE_ISLAND_COEVOLUTION_DSMGA2 "island_coevolution_dsmga2"
#define OPTIMIZER_ARGUMENT_TYPE_DUMMY "dummy"

#define OPTIMIZER_ARGUMENT_CONFIG_FILE_PATH "optimizer_config_file_path"

#define OPTIMIZER_ARGUMENT_MAX_NUMBER_OF_GENE_PATTERNS "max_number_of_gene_patterns"
#define OPTIMIZER_ARGUMENT_GENE_PATTERN_MIN_LENGTH "gene_pattern_min_length"

#define OPTIMIZER_ARGUMENT_LINKAGE_FREQUENCY "linkage_frequency"

#define PRW_LOG_SYSTEM 1



#include "Error.h"
#include "GenePatternList.h"
#include "Generation.h"
#include "Individual.h"
#include "Linkage.h"
#include "Log.h"
#include "Problem.h"
#include "StopCondition.h"

#include <cstdint>
#include <ctime>
#include <functional>
#include <istream>

#include  "util\timer.h"

using namespace std;
using namespace TimeCounters;


enum EOptimizerType
{
	OPTIMIZER_MULTI = 0,
	OPTIMIZER_GA = 1,
	OPTIMIZER_MGA = 2,
	OPTIMIZER_ISLAND_GA = 3,
	OPTIMIZER_DE = 4,
	OPTIMIZER_PSO = 5,
	OPTIMIZER_P3 = 6,
	OPTIMIZER_ISLAND_P3 = 7,
	OPTIMIZER_INCREMENTAL_P3 = 8,
	OPTIMIZER_MULTI_DIFFERENT_WITH_GLOBAL_P3 = 9,
	OPTIMIZER_LTGA = 10,
	OPTIMIZER_DSMGA2 = 11,
	OPTIMIZER_DUMMY = 12,
	OPTIMIZER_ISLAND_COEVOLUTION_DSMGA2 = 13
};//enum EOptimizerType


template <class TGenotype, class TFenotype>
class COptimizer
{
public:
	COptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	COptimizer(COptimizer<TGenotype, TFenotype> *pcOther);

	virtual ~COptimizer();

	virtual COptimizer<TGenotype, TFenotype> *pcCopy() = 0;

	virtual CError eConfigure(istream *psSettings);
	virtual CError eConfigure3LO(istream *psSettings);

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime) = 0;

	virtual void vRun();

	void vResetBestIndividual();

	CProblem<TGenotype, TFenotype> *pcGetProblem() { return pc_problem; };

	CStopCondition<TGenotype, TFenotype> *pcGetStopCondition() { return pc_stop_condition; };

	CGenePatternList *plGetGenePatterns() { return pl_gene_patterns; };
	void vSetGenePatterns(CGenePatternList *plGenePatterns, bool bOwn = false);

	CLinkage<TGenotype> *pcGetLinkage() { return pc_linkage; };

	CIndividual<TGenotype, TFenotype> *pcGetBestIndividual() { return pc_best_individual; };
	void vSetBestIndividual(CIndividual<TGenotype, TFenotype> *pcBestIndividual, bool bCopy = true);

	virtual CString  sAdditionalSummaryInfo() { return(""); }

	time_t tGetBestTime() { return t_best_time; };
	uint64_t iGetBestFFE() { return i_best_ffe; };
	uint64_t iGetCachingFitness() { return i_caching_fitness; };
	uint64_t iGetCachingPopulation() { return i_caching_population; };
	int  iGetNoFFEIterations() { return(i_no_ffe_iterations); }
	int  iGetAllIterations() { return(i_all_iterations); }
	
	



protected:
	bool b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, CIndividual<TGenotype, TFenotype> *pcIndividual);
	bool b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, double dCurrentBestFitnessValue, function<void(TGenotype*)> &&fUpdateBestGenotype);

	virtual CIndividual<TGenotype, TFenotype> *pc_create_individual(TGenotype *pcGenotype)
	{
		return new CIndividual<TGenotype, TFenotype>(pcGenotype, pc_problem->pcGetEvaluation(), nullptr, nullptr, pc_problem->pcGetTransformation());
	};

	void v_clear_params();

	CProblem<TGenotype, TFenotype> *pc_problem;
	
	CStopCondition<TGenotype, TFenotype> *pc_stop_condition;

	uint32_t i_gene_pattern_min_length;
	uint32_t i_linkage_frequency;

	CGenePatternList *pl_gene_patterns;
	CLinkage<TGenotype> *pc_linkage;

	CGeneration<TGenotype> *pc_empty_generation;

	CIndividual<TGenotype, TFenotype> *pc_best_individual;

	time_t t_best_time;
	uint64_t i_best_ffe;
	uint64_t i_caching_fitness;
	uint64_t i_caching_population;

	int  i_no_ffe_iterations;
	int  i_all_iterations;


	CLog *pc_log;

	uint32_t i_random_seed;

	bool b_own_params;
	bool b_own_gene_patterns;

private:
	void v_update_statistics_of_best(uint32_t iIterationNumber, time_t tStartTime);
};//class COptimizer

#endif//OPTIMIZER_H