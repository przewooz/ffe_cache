#ifndef RANDOM_MIGRATION_REPLACEMENT_H
#define RANDOM_MIGRATION_REPLACEMENT_H

#include "Individual.h"
#include "MigrationReplacement.h"
#include "Problem.h"

#include <cstdint>

template <class TGenotype, class TFenotype>
class CRandomMigrationReplacement : public CMigrationReplacement<TGenotype, TFenotype>
{
public:
	virtual void vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, 
		uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals);
};//class CRandomMigrationReplacement

#endif//RANDOM_MIGRATION_REPLACEMENT_H