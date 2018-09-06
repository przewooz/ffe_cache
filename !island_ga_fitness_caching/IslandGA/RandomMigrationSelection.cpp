#include "RandomMigrationSelection.h"

#include "BinaryCoding.h"
#include "RandUtils.h"
#include "RealCoding.h"

#include <algorithm>

template <class TGenotype, class TFenotype>
void CRandomMigrationSelection<TGenotype, TFenotype>::vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)
{
	random_shuffle(ppcPopulation + 0, ppcPopulation + iPopulationSize);

	for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
	{
		*(ppcMigratingIndividuals + i) = new CIndividual<TGenotype, TFenotype>(*(ppcPopulation + i));
	}//for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
}//CRandomMigrationSelection<TGenotype, TFenotype>::vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)

template class CRandomMigrationSelection<CBinaryCoding, CBinaryCoding>;
template class CRandomMigrationSelection<CRealCoding, CRealCoding>;