#include "WorstMigrationReplacement.h"

#include "BinaryCoding.h"
#include "RealCoding.h"

#include <algorithm>
#include <functional>

using namespace std;

template <class TGenotype, class TFenotype>
CWorstMigrationReplacement<TGenotype, TFenotype>::CWorstMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem)
	: CMigrationReplacement<TGenotype, TFenotype>(pcProblem)
{

}//CWorstMigrationReplacement<TGenotype, TFenotype>::CWorstMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
void CWorstMigrationReplacement<TGenotype, TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)
{
	sort(ppcPopulation + 0, ppcPopulation + iPopulationSize, [&](CIndividual<TGenotype, TFenotype> *pcIndividual0, CIndividual<TGenotype, TFenotype> *pcIndividual1)
	{
		return pc_problem->bIsBetterIndividual(pcIndividual1, pcIndividual0);
	});

	for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
	{
		delete *(ppcPopulation + i);

		*(ppcPopulation + i) = *(ppcMigratingIndividuals + i);
	}//for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
}//void CWorstMigrationReplacement<TGenotype, TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)

template class CWorstMigrationReplacement<CBinaryCoding, CBinaryCoding>;
template class CWorstMigrationReplacement<CRealCoding, CRealCoding>;