#ifndef MIGRATION_SELECTION_H
#define MIGRATION_SELECTION_H

#define MIGRATION_SELECTION_ARGUMENT_TYPE "migration_selection_type"
#define MIGRATION_SELECTION_ARGUMENT_TYPE_BEST "best"
#define MIGRATION_SELECTION_ARGUMENT_TYPE_RANDOM "random"
#define MIGRATION_SELECTION_ARGUMENT_TYPE_WORST "worst"

#include "Error.h"
#include "Individual.h"
#include "Problem.h"

#include <cstdint>
#include <istream>

using namespace std;


enum EMigrationSelectionType
{
	MIGRATION_SELECTION_BEST = 0,
	MIGRATION_SELECTION_RANDOM = 1,
	MIGRATION_SELECTION_WORST = 2
};//enum EMigrationSelectionType


template <class TGenotype, class TFenotype>
class CMigrationSelection
{
public:
	static uint32_t iERROR_PARENT_CMigrationSelection;

	CMigrationSelection();
	CMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem);

	virtual ~CMigrationSelection();

	virtual CError eConfigure(istream *psSettings) { return CError(iERROR_PARENT_CMigrationSelection); };

	virtual void vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, 
		uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals) = 0;

protected:
	CProblem<TGenotype, TFenotype> *pc_problem;

private:
	void v_init(CProblem<TGenotype, TFenotype> *pcProblem);
};//class CMigrationSelection


template <class TGenotype, class TFenotype>
class CWorstMigrationSelection : public CMigrationSelection<TGenotype, TFenotype>
{
public:
	CWorstMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem);

	virtual void vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, 
		uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals);
};//class CWorstMigrationSelection

#endif//MIGRATION_SELECTION_H