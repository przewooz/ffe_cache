#include "Optimizer.h"

#include "BinaryCoding.h"
#include "GenePatternReplacementUtils.h"
#include "GenerationUtils.h"
#include "LinkageUtils.h"
#include "RealCoding.h"
#include "StopConditionUtils.h"
#include "UIntCommandParam.h"


template <class TGenotype, class TFenotype>
COptimizer<TGenotype, TFenotype>::COptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
{
	pc_problem = pcProblem;

	pc_stop_condition = nullptr;

	pl_gene_patterns = nullptr;
	pc_linkage = nullptr;

	pc_log = pcLog;

	pc_empty_generation = GenerationUtils::pcGetEmptyGeneration(pcProblem);
	pc_best_individual = nullptr;

	i_random_seed = iRandomSeed;

	b_own_params = true;
	b_own_gene_patterns = true;
}//COptimizer<TGenotype, TFenotype>::COptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

template <class TGenotype, class TFenotype>
COptimizer<TGenotype, TFenotype>::COptimizer(COptimizer<TGenotype, TFenotype> *pcOther)
{
	pc_problem = pcOther->pc_problem;

	pc_stop_condition = pcOther->pc_stop_condition;

	i_gene_pattern_min_length = pcOther->i_gene_pattern_min_length;
	i_linkage_frequency = pcOther->i_linkage_frequency;

	pl_gene_patterns = pcOther->pl_gene_patterns;
	pc_linkage = pcOther->pc_linkage;

	pc_log = pcOther->pc_log;

	pc_empty_generation = pcOther->pc_empty_generation;
	pc_best_individual = nullptr;

	i_random_seed = pcOther->i_random_seed;

	b_own_params = false;
	b_own_gene_patterns = false;
}//COptimizer<TGenotype, TFenotype>::COptimizer(COptimizer<TGenotype, TFenotype> *pcOther)

template <class TGenotype, class TFenotype>
COptimizer<TGenotype, TFenotype>::~COptimizer()
{
	v_clear_params();

	if (b_own_params)
	{
		delete pc_empty_generation;
	}//if (b_own_params)

	vResetBestIndividual();
}//COptimizer::~COptimizer()

template <class TGenotype, class TFenotype>
CError COptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	CError c_error;

	v_clear_params();

	pc_stop_condition = StopConditionUtils::pcGetStopCondition<TGenotype, TFenotype>(pc_problem->pcGetEvaluation(), psSettings, &c_error);


	if (!c_error)
	{
		CUIntCommandParam p_max_number_of_gene_patterns(OPTIMIZER_ARGUMENT_MAX_NUMBER_OF_GENE_PATTERNS, false);
		uint32_t i_max_number_of_gene_patterns = p_max_number_of_gene_patterns.iGetValue(psSettings, &c_error);

		if (!c_error && p_max_number_of_gene_patterns.bHasValue())
		{
			CGenePatternReplacement *pc_gene_pattern_replacement = GenePatternReplacementUtils::pcGetGenePatternReplacement(psSettings, &c_error);
			pl_gene_patterns = new CGenePatternList(i_max_number_of_gene_patterns, pc_gene_pattern_replacement);

			if (!c_error && pl_gene_patterns)
			{
				CUIntCommandParam p_gene_pattern_min_length(OPTIMIZER_ARGUMENT_GENE_PATTERN_MIN_LENGTH, 1, UINT32_MAX);
				i_gene_pattern_min_length = p_gene_pattern_min_length.iGetValue(psSettings, &c_error);
			}//if (!c_error && pl_gene_patterns)
		}//if (!c_error && p_max_number_of_gene_patterns.bHasValue())
	}//if (!c_error)

	if (!c_error)
	{
		pc_linkage = LinkageUtils::pcGetLinkage<TGenotype>(psSettings, &c_error, false);

		if (!c_error && pc_linkage)
		{
			CUIntCommandParam p_linkage_frequency(OPTIMIZER_ARGUMENT_LINKAGE_FREQUENCY, 1, UINT32_MAX);
			i_linkage_frequency = p_linkage_frequency.iGetValue(psSettings, &c_error);
		}//if (!c_error && pc_linkage)
	}//if (!c_error)

	return c_error;
}//CError COptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)



template <class TGenotype, class TFenotype>
CError COptimizer<TGenotype, TFenotype>::eConfigure3LO(istream *psSettings)
{
	CError c_err;

	v_clear_params();

	pc_stop_condition = StopConditionUtils::pcGetStopCondition<TGenotype, TFenotype>(pc_problem->pcGetEvaluation(), psSettings, &c_err);

	return(c_err);
}//template <class TGenotype, class TFenotype>



template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)
{
	i_caching_fitness = 0;
	i_caching_population = 0;
	vResetBestIndividual();
}//void COptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::vRun()
{
	CTimeCounter  c_time_counter;
	time_t t_start_time = time(nullptr);
	
	double  d_best_fitness;
	uint32_t i_iteration_number = 0;
	double  d_time;
	uint64_t i_ffe, i_prev_ffe;

	vInitialize(t_start_time);
	c_time_counter.vSetStartNow();

	i_prev_ffe = 0;
	i_no_ffe_iterations = 0;
	i_all_iterations = 0;

	while (!pc_stop_condition->bStop(t_start_time, i_iteration_number, pc_problem->pcGetEvaluation()->iGetFFE(), pc_best_individual))
	{
		bRunIteration(i_iteration_number, t_start_time);
		i_iteration_number++;
		i_all_iterations++;

		d_best_fitness = pc_best_individual->dGetFitnessValue();
		c_time_counter.bGetTimePassed(&d_time);
		i_ffe = pc_problem->pcGetEvaluation()->iGetFFE();

		if (i_ffe == i_prev_ffe)  i_no_ffe_iterations++;
		i_prev_ffe = i_ffe;

		CString log_message;
		log_message.AppendFormat("[PRW LOG] best fitness: \t %.8lf \t ffe: \t %u \t time: \t %.4lf \t chachingFFE: \t %llu\t chachingPop: \t %llu\tNoFFEiter:\t %d", d_best_fitness, i_ffe, d_time, iGetCachingFitness(), iGetCachingPopulation(), i_no_ffe_iterations);
		pc_log->vPrintLine(log_message, true, PRW_LOG_SYSTEM);

	}//while (!pc_stop_condition->bStop(t_start_time, i_iteration_number, pc_problem->pcGetEvaluation()->iGetFFE(), pc_best_individual))
}//void COptimizer<TGenotype, TFenotype>::vRun()

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::vResetBestIndividual()
{
	delete pc_best_individual;
	pc_best_individual = nullptr;
}//void COptimizer<TGenotype, TFenotype>::vResetBestIndividual()

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::vSetGenePatterns(CGenePatternList *plGenePatterns, bool bOwn)
{
	if (b_own_gene_patterns)
	{
		delete pl_gene_patterns;
	}//if (b_own_gene_patterns)

	pl_gene_patterns = plGenePatterns;
	b_own_gene_patterns = bOwn;
}//void COptimizer<TGenotype, TFenotype>::vSetGenePatterns(CGenePatternList *plGenePatterns, bool bOwn)

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::vSetBestIndividual(CIndividual<TGenotype, TFenotype> *pcBestIndividual, bool bCopy)
{
	vResetBestIndividual();
	pc_best_individual = bCopy ? new CIndividual<TGenotype, TFenotype>(pcBestIndividual) : pcBestIndividual;
}//void COptimizer<TGenotype, TFenotype>::vSetBestIndividual(CIndividual<TGenotype, TFenotype> *pcBestIndividual, bool bCopy)

template <class TGenotype, class TFenotype>
bool COptimizer<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, CIndividual<TGenotype, TFenotype> *pcIndividual)
{
	bool b_updated = false;

	if (!pc_best_individual || pc_problem->bIsBetterIndividual(pcIndividual, pc_best_individual))
	{
		delete pc_best_individual;
		pc_best_individual = new CIndividual<TGenotype, TFenotype>(pcIndividual);

		v_update_statistics_of_best(iIterationNumber, tStartTime);

		b_updated = true;
	}//if (!pc_best_individual || pc_problem->bIsBetterIndividual(pcIndividual, pc_best_individual))

	return b_updated;
}//bool COptimizer<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, CIndividual<TGenotype, TFenotype> *pcIndividual)

template <class TGenotype, class TFenotype>
bool COptimizer<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, double dCurrentBestFitnessValue, function<void(TGenotype*)> &&fUpdateBestGenotype)
{
	bool b_updated = false;

	if (!pc_best_individual || pc_problem->bIsBetterFitnessValue(dCurrentBestFitnessValue, pc_best_individual->dGetFitnessValue()))
	{
		if (!pc_best_individual)
		{
			pc_best_individual = pc_create_individual(pc_empty_generation->pcGenerateEmpty());
		}//if (!pc_best_individual)

		fUpdateBestGenotype(pc_best_individual->pcGetGenotype());
		pc_best_individual->vSetFitnessValue(dCurrentBestFitnessValue);

		v_update_statistics_of_best(iIterationNumber, tStartTime);

		b_updated = true;
	}//if (!pc_best_individual || pc_problem->bIsBetterFitnessValue(dCurrentBestFitnessValue, pc_best_individual->dGetFitnessValue()))

	return b_updated;
}//bool COptimizer<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, double dCurrentBestFitnessValue, function<void(TGenotype*)> &&fUpdateBestGenotype)

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::v_clear_params()
{
	if (b_own_params)
	{
		delete pc_stop_condition;
		pc_stop_condition = nullptr;

		delete pc_linkage;
		pc_linkage = nullptr;
	}//if (b_own_params)

	if (b_own_gene_patterns)
	{
		delete pl_gene_patterns;
		pl_gene_patterns = nullptr;
	}//if (b_own_gene_patterns)
}//void COptimizer<TGenotype, TFenotype>::v_clear_params()

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::v_update_statistics_of_best(uint32_t iIterationNumber, time_t tStartTime)
{
	t_best_time = time(nullptr) - tStartTime;
	i_best_ffe = pc_problem->pcGetEvaluation()->iGetFFE();
}//void COptimizer<TGenotype, TFenotype>::v_update_statistics_of_best(uint32_t iIterationNumber, time_t tStartTime)

template class COptimizer<CBinaryCoding, CBinaryCoding>;
template class COptimizer<CRealCoding, CRealCoding>;