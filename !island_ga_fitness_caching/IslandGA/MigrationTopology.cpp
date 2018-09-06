#include "MigrationTopology.h"

#include "BinaryCoding.h"
#include "GenePattern.h"
#include "PointerUtils.h"
#include "RealCoding.h"

#include <algorithm>
#include <functional>
#include <unordered_set>

using namespace std;

template <class TGenotype, class TFenotype>
CMigrationTopology<TGenotype, TFenotype>::CMigrationTopology()
{
	v_init(nullptr);
}//CMigrationTopology<TGenotype, TFenotype>::CMigrationTopology()

template <class TGenotype, class TFenotype>
CMigrationTopology<TGenotype, TFenotype>::CMigrationTopology(CProblem<TGenotype, TFenotype> *pcProblem)
{
	v_init(pcProblem);
}//CMigrationTopology<TGenotype, TFenotype>::CMigrationTopology(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CMigrationTopology<TGenotype, TFenotype>::~CMigrationTopology()
{

}//CMigrationTopology<TGenotype, TFenotype>::~CMigrationTopology()

template <class TGenotype, class TFenotype>
void CMigrationTopology<TGenotype, TFenotype>::v_init(CProblem<TGenotype, TFenotype> *pcProblem)
{
	pc_problem = pcProblem;
}//void CMigrationTopology<TGenotype, TFenotype>::v_init(CProblem<TGenotype, TFenotype> *pcProblem)


template <class TGenotype, class TFenotype>
void CRandomMigrationTopology<TGenotype, TFenotype>::vGetMigrations(uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> **ppcBestPopulationIndividuals, uint8_t *piMigrationsFrom, uint8_t *piMigrationsTo)
{
	for (uint8_t i = 0; i < iNumberOfPopulations; i++)
	{
		*(piMigrationsFrom + i) = i;
		*(piMigrationsTo + i) = i;
	}//for (uint8_t i = 0; i < iNumberOfPopulations; i++)

	random_shuffle(piMigrationsTo + 0, piMigrationsTo + iNumberOfPopulations);

	for (uint8_t i = 0; i < iNumberOfPopulations - 1; i++)
	{
		if (*(piMigrationsTo + i) == i)
		{
			uint8_t i_swap_index;

			if (i > 0 && RandUtils::bSatisfyProbability(0.5))
			{
				i_swap_index = (uint8_t)RandUtils::iRandIndex(i);
			}//if (i > 0 && RandUtils::bSatisfyProbability(0.5))
			else
			{
				i_swap_index = (uint8_t)RandUtils::iRandNumber(i + 1, iNumberOfPopulations - 1);
			}//else if (i > 0 && RandUtils::bSatisfyProbability(0.5))

			PointerUtils::vSwap(piMigrationsTo, i, i_swap_index);
		}//if (*(piMigrationsTo + i) == i)
	}//for (uint8_t i = 0; i < iNumberOfPopulations - 1; i++)

	if (*(piMigrationsTo + iNumberOfPopulations - 1) == iNumberOfPopulations - 1)
	{
		PointerUtils::vSwap(piMigrationsTo, iNumberOfPopulations - 1, RandUtils::iRandIndex(iNumberOfPopulations - 1));
	}//if (*(piMigrationsTo + iNumberOfPopulations - 1) == iNumberOfPopulations - 1)
}//void CRandomMigrationTopology<TGenotype, TFenotype>::vGetMigrations(uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> **ppcBestPopulationIndividuals, uint8_t *piMigrationsFrom, uint8_t *piMigrationsTo)


template <class TGenotype, class TFenotype>
CGenePatternDistanceTopology<TGenotype, TFenotype>::CGenePatternDistanceTopology(bool bMinDistance)
{
	v_init(bMinDistance);
}//CGenePatternDistanceTopology<TGenotype, TFenotype>::CGenePatternDistanceTopology(bool bMinDistance)

template <class TGenotype, class TFenotype>
CGenePatternDistanceTopology<TGenotype, TFenotype>::CGenePatternDistanceTopology(CGenePatternList *plGenePatterns, bool bMinDistance)
{
	v_init(bMinDistance);
	pl_gene_patterns = plGenePatterns;
}//CGenePatternDistanceTopology<TGenotype, TFenotype>::CGenePatternDistanceTopology(CGenePatternList *plGenePatterns, bool bMinDistance)

template <class TGenotype, class TFenotype>
void CGenePatternDistanceTopology<TGenotype, TFenotype>::v_init(bool bMinDistance)
{
	b_min_distance = bMinDistance;
}//CGenePatternDistanceTopology<TGenotype, TFenotype>::v_init(bool bMinDistance)

template <class TGenotype, class TFenotype>
CError CGenePatternDistanceTopology<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)
{
	CError c_error = CMigrationTopology<TGenotype, TFenotype>::eConfigure(plGenePatterns, psSettings);

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

	return c_error;
}//CError CGenePatternDistanceTopology<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)

#include <iostream>

template <class TGenotype, class TFenotype>
void CGenePatternDistanceTopology<TGenotype, TFenotype>::vGetMigrations(uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> **ppcBestPopulationIndividuals, uint8_t *piMigrationsFrom, uint8_t *piMigrationsTo)
{
	if (pl_gene_patterns->bAny())
	{
		CGenePattern *pc_gene_pattern = pl_gene_patterns->pcGetRandomGenePattern();

		//cout << endl << endl;

		//for (int i = 0; i < pc_gene_pattern->iGetSize(); i++)
		//{
		//	cout << *(pc_gene_pattern->piGetPattern() + i) << " ";
		//}

		//cout << endl << endl;

		//for (uint8_t i = 0; i < iNumberOfPopulations; i++)
		//{
		//	cout << (*(ppcBestPopulationIndividuals + i))->pcGetGenotype() << endl << endl;
		//}//for (uint8_t i = 0; i < iNumberOfPopulations; i++)

		//cout << endl << endl;

		double **ppd_distances = new double*[iNumberOfPopulations];
		uint8_t **ppi_closest_populations = new uint8_t*[iNumberOfPopulations];

		for (uint8_t i = 0; i < iNumberOfPopulations; i++)
		{
			*(ppd_distances + i) = new double[iNumberOfPopulations];
			*(ppi_closest_populations + i) = new uint8_t[iNumberOfPopulations - 1];
		}//for (uint8_t i = 0; i < iNumberOfPopulations; i++)

		double d_distance;

		TGenotype *pc_genotype_0, *pc_genotype_1;

		for (uint8_t i = 0; i < iNumberOfPopulations; i++)
		{
			for (uint8_t j = i + 1; j < iNumberOfPopulations; j++)
			{
				pc_genotype_0 = (*(ppcBestPopulationIndividuals + i))->pcGetGenotype();
				pc_genotype_1 = (*(ppcBestPopulationIndividuals + j))->pcGetGenotype();

				d_distance = pc_genotype_0->dComputeDistance(pc_genotype_1, pc_gene_pattern);

				*(*(ppd_distances + i) + j) = d_distance;
				*(*(ppd_distances + j) + i) = d_distance;

				*(*(ppi_closest_populations + i) + j - 1) = j;
				*(*(ppi_closest_populations + j) + i) = i;
			}//for (uint8_t j = i + 1; j < iNumberOfPopulations; j++)
		}//for (uint8_t i = 0; i < iNumberOfPopulations; i++)

		uint8_t *pi_populations = new uint8_t[iNumberOfPopulations];

		function<bool(uint8_t, uint8_t, uint8_t)> f_comparator = nullptr;

		if (b_min_distance)
		{
			f_comparator = [&](uint8_t i, uint8_t iPopulation0, uint8_t iPopulation1)
			{
				return *(*(ppd_distances + i) + iPopulation0) < *(*(ppd_distances + i) + iPopulation1);
			};
		}//if (b_min_distance)
		else
		{
			f_comparator = [&](uint8_t i, uint8_t iPopulation0, uint8_t iPopulation1)
			{
				return *(*(ppd_distances + i) + iPopulation0) > *(*(ppd_distances + i) + iPopulation1);
			};
		}//else if (b_min_distance)

		for (uint8_t i = 0; i < iNumberOfPopulations; i++)
		{
			*(pi_populations + i) = i;
			
			sort(*(ppi_closest_populations + i) + 0, *(ppi_closest_populations + i) + iNumberOfPopulations - 1, 
				bind(f_comparator, i, placeholders::_1, placeholders::_2));
		}//for (uint8_t i = 0; i < iNumberOfPopulations; i++)

		//random_shuffle(pi_populations + 0, pi_populations + iNumberOfPopulations);

		sort(pi_populations + 0, pi_populations + iNumberOfPopulations, [&](uint8_t iPopulation0, uint8_t iPopulation1) -> bool
		{
			double d_best_distance_0 = *(*(ppd_distances + iPopulation0) + *(*(ppi_closest_populations + iPopulation0) + 0));
			double d_best_distance_1 = *(*(ppd_distances + iPopulation1) + *(*(ppi_closest_populations + iPopulation1) + 0));

			return b_min_distance ? d_best_distance_0 < d_best_distance_1 : d_best_distance_0 > d_best_distance_1;
		});

		unordered_set<uint8_t> s_selected_populations = unordered_set<uint8_t>(iNumberOfPopulations);

		uint8_t i_population, i_closest_population;

		bool b_found_population;

		for (uint8_t i = 0; i < iNumberOfPopulations; i++)
		{
			i_population = *(pi_populations + i);
			b_found_population = false;

			*(piMigrationsFrom + i) = i_population;

			for (uint8_t j = 0; j < iNumberOfPopulations - 1 && !b_found_population; j++)
			{
				i_closest_population = *(*(ppi_closest_populations + i_population) + j);

				if (s_selected_populations.count(i_closest_population) == 0)
				{
					*(piMigrationsTo + i) = i_closest_population;

					s_selected_populations.insert(i_closest_population);
					b_found_population = true;
				}//if (s_selected_populations.count(i_closest_population) == 0)
			}//for (uint8_t j = 0; j < iNumberOfPopulations - 1 && !b_found_population; j++)

			if (i == iNumberOfPopulations - 1 && !b_found_population)
			{
				*(piMigrationsTo + i) = *(piMigrationsTo + i - 1);
				*(piMigrationsTo + i - 1) = i_population;
			}//if (i == iNumberOfPopulations - 1 && !b_found_population)
		}//for (uint8_t i = 0; i < iNumberOfPopulations; i++)

		//cout << endl << endl;

		for (uint8_t i = 0; i < iNumberOfPopulations; i++)
		{
			//cout << (int)*(piMigrationsFrom + i) << " " << (int)*(piMigrationsTo + i) << endl;

			delete *(ppd_distances + i);
			delete *(ppi_closest_populations + i);
		}//for (uint8_t i = 0; i < iNumberOfPopulations; i++)

		//cout << endl << endl;

		delete ppd_distances;
		delete ppi_closest_populations;

		delete pi_populations;

		s_selected_populations.clear();
	}//if (pl_gene_patterns->bAny())
	else
	{
		c_random_migration_topology.vGetMigrations(iNumberOfPopulations, ppcBestPopulationIndividuals, piMigrationsFrom, piMigrationsTo);
	}//else if (pl_gene_patterns->bAny())
}//void CGenePatternDistanceTopology<TGenotype, TFenotype>::vGetMigrations(uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> **ppcBestPopulationIndividuals, uint8_t *piMigrationsFrom, uint8_t *piMigrationsTo)

template <class TGenotype, class TFenotype>
void CRingMigrationTopology<TGenotype, TFenotype>::vGetMigrations(uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> **ppcBestPopulationIndividuals, uint8_t *piMigrationsFrom, uint8_t *piMigrationsTo)
{
	for (uint8_t i = 0; i < iNumberOfPopulations - 1; i++)
	{
		*(piMigrationsFrom + i) = i;
		*(piMigrationsTo + i) = i + 1;
	}//for (uint8_t i = 0; i < iNumberOfPopulations - 1; i++)

	*(piMigrationsFrom + iNumberOfPopulations - 1) = iNumberOfPopulations - 1;
	*(piMigrationsTo + iNumberOfPopulations - 1) = 0;
}//void CRingMigrationTopology<TGenotype, TFenotype>::vGetMigrations(uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> **ppcBestPopulationIndividuals, uint8_t *piMigrationsFrom, uint8_t *piMigrationsTo)

template class CGenePatternDistanceTopology<CBinaryCoding, CBinaryCoding>;
template class CGenePatternDistanceTopology<CRealCoding, CRealCoding>;

template class CRandomMigrationTopology<CBinaryCoding, CBinaryCoding>;
template class CRandomMigrationTopology<CRealCoding, CRealCoding>;

template class CMigrationTopology<CBinaryCoding, CBinaryCoding>;
template class CMigrationTopology<CRealCoding, CRealCoding>;

template class CRingMigrationTopology<CBinaryCoding, CBinaryCoding>;
template class CRingMigrationTopology<CRealCoding, CRealCoding>;