#include "RandomMigrationReplacement.h"

#include "BinaryCoding.h"
#include "RandUtils.h"
#include "RealCoding.h"

#include <algorithm>

template <class TGenotype, class TFenotype>
void CRandomMigrationReplacement<TGenotype, TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)
{
	random_shuffle(ppcPopulation + 0, ppcPopulation + iPopulationSize);

	for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
	{
		delete *(ppcPopulation + i);

		*(ppcPopulation + i) = *(ppcMigratingIndividuals + i);
	}//for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
}//void CRandomMigrationReplacement<TGenotype, TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)

template class CRandomMigrationReplacement<CBinaryCoding, CBinaryCoding>;
template class CRandomMigrationReplacement<CRealCoding, CRealCoding>;