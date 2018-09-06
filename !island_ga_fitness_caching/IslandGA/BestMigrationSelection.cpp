#include "BestMigrationSelection.h"

#include "BinaryCoding.h"
#include "RealCoding.h"

#include <algorithm>
#include <functional>

using namespace std;

template <class TGenotype, class TFenotype>
CBestMigrationSelection<TGenotype, TFenotype>::CBestMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem)
	: CMigrationSelection<TGenotype, TFenotype>(pcProblem)
{

}//CBestMigrationSelection<TGenotype, TFenotype>::CBestMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
void CBestMigrationSelection<TGenotype, TFenotype>::vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)
{
	sort(ppcPopulation + 0, ppcPopulation + iPopulationSize, [&](CIndividual<TGenotype, TFenotype> *pcIndividual0, CIndividual<TGenotype, TFenotype> *pcIndividual1)
	{
		return pc_problem->bIsBetterIndividual(pcIndividual0, pcIndividual1);
	});

	for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
	{
		*(ppcMigratingIndividuals + i) = new CIndividual<TGenotype, TFenotype>(*(ppcPopulation + i));
	}//for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
}//void CBestMigrationSelection<TGenotype, TFenotype>::vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)

template class CBestMigrationSelection<CBinaryCoding, CBinaryCoding>;
template class CBestMigrationSelection<CRealCoding, CRealCoding>;