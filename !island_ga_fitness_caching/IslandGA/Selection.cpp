#include "Selection.h"

#include "BinaryCoding.h"
#include "CommandParam.h"
#include "GenePattern.h"
#include "RandUtils.h"
#include "RealCoding.h"
#include "UIntCommandParam.h"

#include <unordered_set>

using namespace std;

template <class TGenotype, class TFenotype>
CSelection<TGenotype, TFenotype>::CSelection(CProblem<TGenotype, TFenotype> *pcProblem)
{
	pc_problem = pcProblem;
}//CSelection<TGenotype, TFenotype>::CSelection(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CSelection<TGenotype, TFenotype>::~CSelection()
{

}//CSelection<TGenotype, TFenotype>::~CSelection()


template <class TGenotype, class TFenotype>
CTournamentSelection<TGenotype, TFenotype>::CTournamentSelection(CProblem<TGenotype, TFenotype> *pcProblem)
	: CSelection<TGenotype, TFenotype>(pcProblem)
{

}//CTournamentSelection<TGenotype, TFenotype>::CTournamentSelection(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CTournamentSelection<TGenotype, TFenotype>::CTournamentSelection(uint32_t iTournamentSize, bool bElitism, CProblem<TGenotype, TFenotype> *pcProblem)
	: CSelection<TGenotype, TFenotype>(pcProblem)
{
	i_tournament_size = iTournamentSize;
	b_elitism = bElitism;
}//CTournamentSelection<TGenotype, TFenotype>::CTournamentSelection(uint32_t iTournamentSize, bool bElitism, CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CError CTournamentSelection<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)
{
	CError c_error = CSelection<TGenotype, TFenotype>::eConfigure(plGenePatterns, psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_tournament_size(SELECTION_ARGUMENT_TOURNAMENT_SIZE, 2, UINT32_MAX, (uint32_t)SELECTION_ARGUMENT_TOURNAMENT_SIZE_DEFAULT);
		i_tournament_size = p_tournament_size.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CBoolCommandParam p_elitism(SELECTION_ARGUMENT_ELITISM, SELECTION_ARGUMENT_ELITISM_DEFAULT, false);
		b_elitism = p_elitism.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CTournamentSelection<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)

template <class TGenotype, class TFenotype>
void CTournamentSelection<TGenotype, TFenotype>::vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcOldPopulation, CIndividual<TGenotype, TFenotype> **ppcNewPopulation, CIndividual<TGenotype, TFenotype> *pcBestIndividual)
{
	unordered_set<uint32_t> s_selected_random_individuals_indexes(i_tournament_size);
    CIndividual<TGenotype, TFenotype> *pc_best_tournament_individual, *pc_random_individual;

	uint32_t i_number_of_tournaments = iPopulationSize;

	if (b_elitism)
	{
		i_number_of_tournaments--;
	}//if (b_elitism)

    uint32_t i_random_index;

	for (uint32_t i = 0; i < i_number_of_tournaments; i++)
	{
		s_selected_random_individuals_indexes.clear();
        pc_best_tournament_individual = nullptr;

		for (uint32_t j = 0; j < i_tournament_size; j++)
        {
			do//while (s_selected_random_individuals_indexes.count(i_random_index) > 0)
            {
                i_random_index = RandUtils::iRandIndex(iPopulationSize);
            } while (s_selected_random_individuals_indexes.count(i_random_index) > 0);

			s_selected_random_individuals_indexes.insert(i_random_index);

            pc_random_individual = *(ppcOldPopulation + i_random_index);

            if (!pc_best_tournament_individual || pc_problem->bIsBetterIndividual(pc_random_individual, pc_best_tournament_individual))
            {
                pc_best_tournament_individual = pc_random_individual;
            }//if (!pc_best_tournament_individual || pc_problem->bIsBetterIndividual(pc_random_individual, pc_best_tournament_individual))
		}//for (uint32_t j = 0; j < i_tournament_size; j++)

		*(ppcNewPopulation + i) = new CIndividual<TGenotype, TFenotype>(pc_best_tournament_individual);
	}//for (uint32_t i = 0; i < i_number_of_tournaments; i++)

	if (b_elitism)
	{
		*(ppcNewPopulation + iPopulationSize - 1) = new CIndividual<TGenotype, TFenotype>(pcBestIndividual);
	}//if (b_elitism)

	s_selected_random_individuals_indexes.clear();
}//void CTournamentSelection<TGenotype, TFenotype>::vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcOldPopulation, CIndividual<TGenotype, TFenotype> **ppcNewPopulation, CIndividual<TGenotype, TFenotype> *pcBestIndividual)


template <class TGenotype, class TFenotype>
CGenePatternTournamentSelection<TGenotype, TFenotype>::CGenePatternTournamentSelection(CProblem<TGenotype, TFenotype> *pcProblem)
	: CSelection<TGenotype, TFenotype>(pcProblem), c_tournament_selection(2, true, pcProblem)
{

}//CGenePatternTournamentSelection<TGenotype, TFenotype>::CGenePatternTournamentSelection(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CGenePatternTournamentSelection<TGenotype, TFenotype>::CGenePatternTournamentSelection(uint32_t iTournamentSize, CGenePatternList *plGenePatterns, bool bOnlyGenePatternDistance, CProblem<TGenotype, TFenotype> *pcProblem)
	: CSelection<TGenotype, TFenotype>(pcProblem), c_tournament_selection(2, true, pcProblem)
{
	i_tournament_size = iTournamentSize;
	pl_gene_patterns = plGenePatterns;
	b_only_gene_pattern_distance = bOnlyGenePatternDistance;
}//CGenePatternTournamentSelection<TGenotype, TFenotype>::CGenePatternTournamentSelection(uint32_t iTournamentSize, CGenePatternList *plGenePatterns, bool bOnlyGenePatternDistance, CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CError CGenePatternTournamentSelection<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)
{
	CError c_error = CSelection<TGenotype, TFenotype>::eConfigure(plGenePatterns, psSettings);

	if (!c_error)
	{
		if (plGenePatterns)
		{
			pl_gene_patterns = plGenePatterns;
		}//if (plGenePatterns)
		else
		{
			c_error.vSetError(CError::iERROR_CODE_SYSTEM_MISSING_ARGUMENT, "gene patterns");
		}//else if (plGenePatterns)
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_tournament_size(SELECTION_ARGUMENT_TOURNAMENT_SIZE, 2, UINT32_MAX, (uint32_t)SELECTION_ARGUMENT_TOURNAMENT_SIZE_DEFAULT);
		i_tournament_size = p_tournament_size.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CBoolCommandParam p_only_gene_pattern_distance(SELECTION_ARGUMENT_ONLY_GENE_PATTERN_DISTANCE);
		b_only_gene_pattern_distance = p_only_gene_pattern_distance.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CGenePatternTournamentSelection<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)

#include <iostream>
using namespace std;

template <class TGenotype, class TFenotype>
void CGenePatternTournamentSelection<TGenotype, TFenotype>::vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcOldPopulation, CIndividual<TGenotype, TFenotype> **ppcNewPopulation, CIndividual<TGenotype, TFenotype> *pcBestIndividual)
{
	if (pl_gene_patterns && pl_gene_patterns->bAny())
	{
		CGenePattern *pc_gene_pattern;// = pl_gene_patterns->pcGetRandomGenePattern();

		unordered_set<uint32_t> s_selected_random_individuals_indexes(i_tournament_size);
		
		CIndividual<TGenotype, TFenotype> *pc_min_distance_individual, *pc_random_individual, *pc_individual;
		CIndividual<TGenotype, TFenotype> *pc_worst_individual = nullptr;

		double d_min_distance;

		uint32_t i_random_index, i_worst_individual_index;

		//for (int j = 0; j < pc_gene_pattern->iGetSize(); j++)
		//{
		//	cout << *(pc_gene_pattern->piGetPattern() + j) << " ";
		//}

		//cout << endl;

		bool b_best_survived = false;

		//bool b_only_gene_pattern_distance;

		for (uint32_t i = 0; i < iPopulationSize; i++)
		{
			pc_gene_pattern = pl_gene_patterns->pcGetRandomGenePattern();

			//b_only_gene_pattern_distance = RandUtils::bSatisfyProbability(0.5);

			s_selected_random_individuals_indexes.insert(i);

			pc_individual = *(ppcOldPopulation + i);

			d_min_distance = DBL_MAX;

			for (uint32_t j = 0; j < i_tournament_size - 1; j++)
			{
				do//while (s_selected_random_individuals_indexes.count(i_random_index) > 0)
				{
					i_random_index = RandUtils::iRandIndex(iPopulationSize);
				} while (s_selected_random_individuals_indexes.count(i_random_index) > 0);

				s_selected_random_individuals_indexes.insert(i_random_index);

				pc_random_individual = *(ppcOldPopulation + i_random_index);

				if (!pc_individual->pcGetGenotype()->bExceededMaxDistance(pc_random_individual->pcGetGenotype(), pc_gene_pattern, d_min_distance, &d_min_distance, b_only_gene_pattern_distance))
				{
					pc_min_distance_individual = pc_random_individual;
				}//if (!pc_individual->pcGetGenotype()->bExceededMaxDistance(pc_random_individual->pcGetGenotype(), pc_gene_pattern, d_min_distance, &d_min_distance, b_only_gene_pattern_distance))
			}//for (uint32_t j = 0; j < i_tournament_size - 1; j++)

			//cout << "distance: " << d_min_distance << "; fitness difference: " << (pc_individual->dGetFitnessValue() - pc_min_distance_individual->dGetFitnessValue());
			//cout << "; best: " << pc_problem->pcGetBetterIndividual(pc_individual, pc_min_distance_individual)->dGetFitnessValue();
			//cout << endl << endl;

			*(ppcNewPopulation + i) = new CIndividual<TGenotype, TFenotype>(pc_problem->pcGetBetterIndividual(pc_individual, pc_min_distance_individual));

			if (!pc_worst_individual || pc_problem->bIsBetterIndividual(pc_worst_individual, *(ppcNewPopulation + i)))
			{
				pc_worst_individual = *(ppcNewPopulation + i);
				i_worst_individual_index = i;
			}//if (!pc_worst_individual || pc_problem->bIsBetterIndividual(pc_worst_individual, *(ppcNewPopulation + i)))

			if (!b_best_survived && !pc_problem->bIsBetterIndividual(pcBestIndividual, *(ppcNewPopulation + i)))
			{
				b_best_survived = true;
			}//if (!b_best_survived && !pc_problem->bIsBetterIndividual(pcBestIndividual, *(ppcNewPopulation + i)))

			/*if (!pc_individual->bIsOwn())
			{
				cout << "BEGIN" << endl;

				cout << endl;

				cout << i << endl;

				cout << endl;

				cout << "number of gene patterns: " << pl_gene_patterns->iGetNumberOfGenePatterns() << endl;

				cout << d_min_distance << endl;

				cout << endl;

				for (int j = 0; j < pc_gene_pattern->iGetSize(); j++)
				{
					cout << *(pc_gene_pattern->piGetPattern() + j) << " ";
				}

				cout << endl;

				cout << endl;

				cout << pc_individual->pcGetFenotype() << endl;

				cout << endl;

				cout << pc_min_distance_individual->pcGetFenotype() << endl;

				cout << endl;

				cout << "END" << endl;
			}*/

			

			s_selected_random_individuals_indexes.clear();
		}//for (uint32_t i = 0; i < iPopulationSize; i++)

		//if (!b_best_survived)
		{
			//cout << "zamiana" << endl;

			delete *(ppcNewPopulation + i_worst_individual_index);
			*(ppcNewPopulation + i_worst_individual_index) = new CIndividual<TGenotype, TFenotype>(pcBestIndividual);
		}//if (!b_best_survived)
	}//if (pl_gene_patterns && pl_gene_patterns->bAny())
	else
	{
		c_tournament_selection.vSelect(iPopulationSize, ppcOldPopulation, ppcNewPopulation, pcBestIndividual);
	}//else if (pl_gene_patterns && pl_gene_patterns->bAny())
}//void CGenePatternTournamentSelection<TGenotype, TFenotype>::vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcOldPopulation, CIndividual<TGenotype, TFenotype> **ppcNewPopulation, CIndividual<TGenotype, TFenotype> *pcBestIndividual)

template class CSelection<CBinaryCoding, CBinaryCoding>;
template class CSelection<CRealCoding, CRealCoding>;

template class CTournamentSelection<CBinaryCoding, CBinaryCoding>;
template class CTournamentSelection<CRealCoding, CRealCoding>;

template class CGenePatternTournamentSelection<CBinaryCoding, CBinaryCoding>;
template class CGenePatternTournamentSelection<CRealCoding, CRealCoding>;