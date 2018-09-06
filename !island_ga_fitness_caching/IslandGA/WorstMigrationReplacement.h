#ifndef WORST_MIGRATION_REPLACEMENT_H
#define WORST_MIGRATION_REPLACEMENT_H

#include "Individual.h"
#include "MigrationReplacement.h"
#include "Problem.h"

#include <cstdint>

template <class TGenotype, class TFenotype>
class CWorstMigrationReplacement : public CMigrationReplacement<TGenotype, TFenotype>
{
public:
	CWorstMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem);

	virtual void vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, 
		uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals);
};//class CWorstMigrationReplacement

#endif//WORST_MIGRATION_REPLACEMENT_H