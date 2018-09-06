#include "MultiPopulationGeneticAlgorithm.h"

#include "BinaryCoding.h"
#include "CommandParam.h"
#include "GenePattern.h"
#include "GeneticAlgorithm.h"
#include "PopulationStrategyUtils.h"
#include "RealCoding.h"
#include "UIntCommandParam.h"

#include <unordered_set>

template <class TGenotype, class TFenotype>
CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::CMultiPopulationGeneticAlgorithm(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CMultiOptimizer<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed)
{

}//CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::CMultiPopulationGeneticAlgorithm(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

template <class TGenotype, class TFenotype>
CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::CMultiPopulationGeneticAlgorithm(CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype> *pcOther)
	: CMultiOptimizer<TGenotype, TFenotype>(pcOther)
{
	b_only_last_bests = pcOther->b_only_last_bests;
}//CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::CMultiPopulationGeneticAlgorithm(CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype> *pcOther)

template<class TGenotype, class TFenotype>
CError CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	CError c_error = CMultiOptimizer<TGenotype, TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		CBoolCommandParam p_only_last_best(MGA_ARGUMENT_ONLY_LAST_BESTS);
		b_only_last_bests = p_only_last_best.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
bool CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	v_update_operators();
	return CMultiOptimizer<TGenotype, TFenotype>::bRunIteration(iIterationNumber, tStartTime);
}//bool CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

template <class TGenotype, class TFenotype>
CError CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::e_configure_params_optimizer(istream *psSettings)
{
	pc_params_optimizer = new CGeneticAlgorithm<TGenotype, TFenotype>(pc_problem, pc_log, i_random_seed);
	CError c_error = pc_params_optimizer->eConfigure(psSettings);

	if (!c_error && pc_params_optimizer)
	{
		pc_params_optimizer->vSetGenePatterns(pl_gene_patterns);
	}//if (!c_error && pc_params_optimizer)

	return c_error;
}//CError CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::e_configure_params_optimizer(istream *psSettings)

template <class TGenotype, class TFenotype>
void CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::v_update_operators()
{
	uint8_t i_number_of_populations = (uint8_t)v_populations.size();

	CIndividual<TGenotype, TFenotype> **ppc_best_populations_individuals = b_only_last_bests
		? ppc_get_last_best_populations_individuals()
		: ppc_get_best_populations_individuals();

	TGenotype **ppc_best_populations_genotypes = new TGenotype*[i_number_of_populations];

	for (uint8_t i = 0; i < i_number_of_populations; i++)
	{
		*(ppc_best_populations_genotypes + i) = (*(ppc_best_populations_individuals + i))->pcGetGenotype();
	}//for (uint8_t i = 0; i < i_number_of_populations; i++)

	CGeneticAlgorithm<TGenotype, TFenotype> *pc_params_genetic_algorithm = (CGeneticAlgorithm<TGenotype, TFenotype>*)pc_params_optimizer;

	pc_params_genetic_algorithm->pcGetGeneration()->vUpdateBestGenotypes(i_number_of_populations, ppc_best_populations_genotypes);
	pc_params_genetic_algorithm->pcGetMutation()->vUpdateBestGenotypes(i_number_of_populations, ppc_best_populations_genotypes);

	if (pl_gene_patterns && pl_gene_patterns->bAny())
	{
		pc_params_genetic_algorithm->pcGetGeneration()->vSetGenePattern(pl_gene_patterns->pcGetRandomGenePattern());
	}//if (pl_gene_patterns && pl_gene_patterns->bAny())

	delete ppc_best_populations_genotypes;
	delete ppc_best_populations_individuals;
}//void CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::v_update_operators()

template <class TGenotype, class TFenotype>
CIndividual<TGenotype, TFenotype> ** CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::ppc_get_last_best_populations_individuals()
{
	uint8_t i_number_of_populations = (uint8_t)v_populations.size();

	CIndividual<TGenotype, TFenotype> **ppc_last_best_populations_individuals = new CIndividual<TGenotype, TFenotype>*[i_number_of_populations];

	CGeneticAlgorithm<TGenotype, TFenotype> *pc_genetic_algorithm;

	for (uint8_t i = 0; i < i_number_of_populations; i++)
	{
		pc_genetic_algorithm = (CGeneticAlgorithm<TGenotype, TFenotype>*)v_populations.at(i);
		*(ppc_last_best_populations_individuals + i) = pc_genetic_algorithm->pcGetLastBestIndividual();
	}//for (uint8_t i = 0; i < i_number_of_populations; i++)

	return ppc_last_best_populations_individuals;
}//CIndividual<TGenotype, TFenotype> ** CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::ppc_get_last_best_populations_individuals()

template <class TGenotype, class TFenotype>
CIndividual<TGenotype, TFenotype> ** CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::ppc_get_all_best_populations_individuals()
{
	uint8_t i_number_of_populations = (uint8_t)v_populations.size();

	CIndividual<TGenotype, TFenotype> **ppc_last_best_populations_individuals = new CIndividual<TGenotype, TFenotype>*[i_number_of_populations * 2];

	CGeneticAlgorithm<TGenotype, TFenotype> *pc_genetic_algorithm;

	for (uint8_t i = 0; i < i_number_of_populations; i++)
	{
		pc_genetic_algorithm = (CGeneticAlgorithm<TGenotype, TFenotype>*)v_populations.at(i);
		*(ppc_last_best_populations_individuals + 2 * i + 0) = pc_genetic_algorithm->pcGetBestIndividual();
		*(ppc_last_best_populations_individuals + 2 * i + 1) = pc_genetic_algorithm->pcGetLastBestIndividual();
	}//for (uint8_t i = 0; i < i_number_of_populations; i++)

	return ppc_last_best_populations_individuals;
}//CIndividual<TGenotype, TFenotype> ** CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>::ppc_get_all_best_populations_individuals()

template class CMultiPopulationGeneticAlgorithm<CBinaryCoding, CBinaryCoding>;
template class CMultiPopulationGeneticAlgorithm<CRealCoding, CRealCoding>;