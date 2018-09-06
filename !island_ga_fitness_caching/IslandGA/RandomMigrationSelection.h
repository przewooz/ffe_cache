#ifndef RANDOM_MIGRATION_SELECTION_H
#define RANDOM_MIGRATION_SELECTION_H

#include "Individual.h"
#include "MigrationSelection.h"
#include "Problem.h"

#include <cstdint>

template <class TGenotype, class TFenotype>
class CRandomMigrationSelection : public CMigrationSelection<TGenotype, TFenotype>
{
public:
	virtual void vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, 
		uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals);
};//class CRandomMigrationSelection

#endif//RANDOM_MIGRATION_SELECTION_H