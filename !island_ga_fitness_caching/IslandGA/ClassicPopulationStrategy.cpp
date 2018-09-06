#include "ClassicPopulationStrategy.h"

#include "BinaryCoding.h"
#include "RealCoding.h"
#include "UIntCommandParam.h"

template <class TGenotype, class TFenotype>
CClassicPopulationStrategy<TGenotype, TFenotype>::CClassicPopulationStrategy(CProblem<TGenotype, TFenotype> *pcProblem)
	: CPopulationStrategy<TGenotype, TFenotype>(DEF_CLASSIC_STRATEGY_INIT_NUMBER_OF_POPULATIONS, pcProblem)
{
	v_init();
}//CClassicPopulationStrategy<TGenotype, TFenotype>::CClassicPopulationStrategy(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CClassicPopulationStrategy<TGenotype, TFenotype>::CClassicPopulationStrategy(uint32_t iMaxNumberOfIterationsWithoutProgress, CProblem<TGenotype, TFenotype> *pcProblem)
	: CPopulationStrategy<TGenotype, TFenotype>(DEF_CLASSIC_STRATEGY_INIT_NUMBER_OF_POPULATIONS, pcProblem)
{
	v_init();
	i_max_number_of_iterations_without_progress = iMaxNumberOfIterationsWithoutProgress;
}//CClassicPopulationStrategy<TGenotype, TFenotype>::CClassicPopulationStrategy(uint32_t iMaxNumberOfIterationsWithoutProgress, CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CError CClassicPopulationStrategy<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	CError c_error = CPopulationStrategy<TGenotype, TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_number_of_iterations(POPULATION_STRATEGY_ARGUMENT_NUMBER_OF_ITERATIONS);
		i_max_number_of_iterations_without_progress = p_number_of_iterations.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CClassicPopulationStrategy<TGenotype, TFenotype>::eConfigure(istream *psSettings)

#include <iostream>
using namespace std;

template <class TGenotype, class TFenotype>
void CClassicPopulationStrategy<TGenotype, TFenotype>::vUpdateNumberOfPopulations(uint32_t iIterationNumber, uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> *pcBestIndividual, CIndividual<TGenotype, TFenotype> **ppcBestPopulationsIndividuals, unordered_set<uint8_t> *psPopulationToRemoveIndexes, uint8_t *piNumberOfPopulationsToCreate)
{
	CIndividual<TGenotype, TFenotype> *pc_best_individual;
	double d_avg_fitness_value = 0;

	for (uint8_t i = 0; i < iNumberOfPopulations; i++)
	{
		pc_best_individual = *(ppcBestPopulationsIndividuals + i);
		d_avg_fitness_value += pc_best_individual->dGetFitnessValue();
	}//for (uint8_t i = 0; i < iNumberOfPopulations; i++)

	d_avg_fitness_value /= (double)iNumberOfPopulations;

	cout << endl << "last avg: " << d_last_avg_fitness_value << "; current avg: " << d_avg_fitness_value << endl << endl;

	cout << i_last_progress_iteration_number << endl;

	if (!pc_problem->bIsBetterFitnessValue(d_avg_fitness_value, d_last_avg_fitness_value))
	{
		cout << "inside" << endl;

		cout << (iIterationNumber - i_last_progress_iteration_number) << endl << endl;

		if (iIterationNumber - i_last_progress_iteration_number == i_max_number_of_iterations_without_progress)
		{
			cout << "classic" << endl << endl;

			CIndividual<TGenotype, TFenotype> *pc_best_individual_0;
			CIndividual<TGenotype, TFenotype> *pc_best_individual_1;

			for (uint8_t i = 0; i < iNumberOfPopulations; i++)
			{
				pc_best_individual_0 = *(ppcBestPopulationsIndividuals + i);

				for (uint8_t j = i + 1; j < iNumberOfPopulations; j++)
				{
					if (psPopulationToRemoveIndexes->count(i) == 0 && psPopulationToRemoveIndexes->count(j) == 0)
					{
						pc_best_individual_1 = *(ppcBestPopulationsIndividuals + j);

						if (*pc_best_individual_0->pcGetGenotype() == *pc_best_individual_1->pcGetGenotype())
						{
							psPopulationToRemoveIndexes->insert(!pc_problem->bIsBetterIndividual(pc_best_individual_0, pc_best_individual_1) ? i : j);
						}//if (*pc_best_individual_0->pcGetGenotype() == *pc_best_individual_1->pcGetGenotype())
					}//if (psPopulationToRemoveIndexes->count(i) == 0 && psPopulationToRemoveIndexes->count(j) == 0)
				}//for (uint8_t j = i + 1; j < iNumberOfPopulations; j++)
			}//for (uint8_t i = 0; i < iNumberOfPopulations; i++)

			*piNumberOfPopulationsToCreate = DEF_CLASSIC_STRATEGY_NUMBER_OF_POPULATIONS_TO_CREATE;

			d_last_avg_fitness_value = -DBL_MAX;
			i_last_progress_iteration_number = iIterationNumber;
		}//if (iIterationNumber - i_last_progress_iteration_number == i_max_number_of_iterations_without_progress)
	}//if (!pc_problem->bIsBetterFitnessValue(d_avg_fitness_value, d_last_avg_fitness_value))
	else
	{
		d_last_avg_fitness_value = d_avg_fitness_value;
		i_last_progress_iteration_number = iIterationNumber;
	}//else if (!pc_problem->bIsBetterFitnessValue(d_avg_fitness_value, d_last_avg_fitness_value))
}//void CClassicPopulationStrategy<TGenotype, TFenotype>::vUpdateNumberOfPopulations(uint32_t iIterationNumber, uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> *pcBestIndividual, CIndividual<TGenotype, TFenotype> **ppcBestPopulationsIndividuals, unordered_set<uint8_t> *psPopulationToRemoveIndexes, uint8_t *piNumberOfPopulationsToCreate)

template <class TGenotype, class TFenotype>
void CClassicPopulationStrategy<TGenotype, TFenotype>::v_init()
{
	d_last_avg_fitness_value = -DBL_MAX;
}//void CClassicPopulationStrategy<TGenotype, TFenotype>::v_init()

template class CClassicPopulationStrategy<CBinaryCoding, CBinaryCoding>;
template class CClassicPopulationStrategy<CRealCoding, CRealCoding>;