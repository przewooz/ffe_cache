#include "ActivePopulationStrategy.h"

#include "BinaryCoding.h"
#include "RealCoding.h"

template <class TGenotype, class TFenotype>
CActivePopulationStrategy<TGenotype, TFenotype>::CActivePopulationStrategy(CProblem<TGenotype, TFenotype> *pcProblem)
	: CClassicPopulationStrategy<TGenotype, TFenotype>(pcProblem)
{
	v_init();
}//CActivePopulationStrategy<TGenotype, TFenotype>::CActivePopulationStrategy(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CActivePopulationStrategy<TGenotype, TFenotype>::CActivePopulationStrategy(uint32_t iNumberOfIterationsWithoutProgress, CProblem<TGenotype, TFenotype> *pcProblem)
	: CClassicPopulationStrategy<TGenotype, TFenotype>(iNumberOfIterationsWithoutProgress, pcProblem)
{
	v_init();
}//CActivePopulationStrategy<TGenotype, TFenotype>::CActivePopulationStrategy(uint32_t iNumberOfIterationsWithoutProgress, CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CActivePopulationStrategy<TGenotype, TFenotype>::~CActivePopulationStrategy()
{
	delete pc_best_individual;
}//CActivePopulationStrategy<TGenotype, TFenotype>::~CActivePopulationStrategy()

template <class TGenotype, class TFenotype>
void CActivePopulationStrategy<TGenotype, TFenotype>::vUpdateNumberOfPopulations(uint32_t iIterationNumber, uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> *pcBestIndividual, CIndividual<TGenotype, TFenotype> **ppcBestPopulationsIndividuals, unordered_set<uint8_t> *psPopulationToRemoveIndexes, uint8_t *piNumberOfPopulationsToCreate)
{
	if (!pc_best_individual || pc_problem->bIsBetterIndividual(pcBestIndividual, pc_best_individual))
	{
		bool b_found_best_population = false;

		for (uint8_t i = 0; i < iNumberOfPopulations; i++)
		{
			if (b_found_best_population || pc_problem->bIsBetterIndividual(pcBestIndividual, *(ppcBestPopulationsIndividuals + i)))
			{
				psPopulationToRemoveIndexes->insert(i);
			}//if (b_found_best_population || pc_problem->bIsBetterIndividual(pcBestIndividual, *(ppcBestPopulationsIndividuals + i)))
			else
			{
				b_found_best_population = true;
			}//else if (b_found_best_population || pc_problem->bIsBetterIndividual(pcBestIndividual, *(ppcBestPopulationsIndividuals + i)))
		}//for (uint8_t i = 0; i < iNumberOfPopulations; i++)

		i_last_progress_iteration_number = iIterationNumber;
		d_last_avg_fitness_value = pcBestIndividual->dGetFitnessValue();

		delete pc_best_individual;
		pc_best_individual = new CIndividual<TGenotype, TFenotype>(pcBestIndividual);
	}//if (!pc_best_individual || pc_problem->bIsBetterIndividual(pcBestIndividual, pc_best_individual))
	else
	{
		CClassicPopulationStrategy<TGenotype, TFenotype>::vUpdateNumberOfPopulations(iIterationNumber, iNumberOfPopulations, pcBestIndividual, 
			ppcBestPopulationsIndividuals, psPopulationToRemoveIndexes, piNumberOfPopulationsToCreate);
	}//else if (!pc_best_individual || pc_problem->bIsBetterIndividual(pcBestIndividual, pc_best_individual))
}//void CActivePopulationStrategy<TGenotype, TFenotype>::vUpdateNumberOfPopulations(uint32_t iIterationNumber, uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> *pcBestIndividual, CIndividual<TGenotype, TFenotype> **ppcBestPopulationsIndividuals, unordered_set<uint8_t> *psPopulationToRemoveIndexes, uint8_t *piNumberOfPopulationsToCreate)

template <class TGenotype, class TFenotype>
void CActivePopulationStrategy<TGenotype, TFenotype>::v_init()
{
	i_init_number_of_populations = DEF_ACTIVE_STRATEGY_INIT_NUMBER_OF_POPULATIONS;
	pc_best_individual = nullptr;
}//void CActivePopulationStrategy<TGenotype, TFenotype>::v_init()

template class CActivePopulationStrategy<CBinaryCoding, CBinaryCoding>;
template class CActivePopulationStrategy<CRealCoding, CRealCoding>;