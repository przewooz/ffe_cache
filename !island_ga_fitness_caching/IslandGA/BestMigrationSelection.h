#ifndef BEST_MIGRATION_SELECTION_H
#define BEST_MIGRATION_SELECTION_H

#include "Individual.h"
#include "MigrationSelection.h"
#include "Problem.h"

#include <cstdint>

template <class TGenotype, class TFenotype>
class CBestMigrationSelection : public CMigrationSelection<TGenotype, TFenotype>
{
public:
	CBestMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem);

	virtual void vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, 
		uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals);
};//class CBestMigrationSelection

#endif//BEST_MIGRATION_SELECTION_H