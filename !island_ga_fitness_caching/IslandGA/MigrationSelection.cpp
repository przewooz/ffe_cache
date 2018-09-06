#include "MigrationSelection.h"

#include "BinaryCoding.h"
#include "RealCoding.h"

#include <algorithm>
#include <functional>

using namespace std;

template <class TGenotype, class TFenotype>
uint32_t CMigrationSelection<TGenotype, TFenotype>::iERROR_PARENT_CMigrationSelection = CError::iADD_ERROR_PARENT("CMigrationSelection");

template <class TGenotype, class TFenotype>
CMigrationSelection<TGenotype, TFenotype>::CMigrationSelection()
{
	v_init(nullptr);
}//CMigrationSelection<TGenotype, TFenotype>::CMigrationSelection()

template <class TGenotype, class TFenotype>
CMigrationSelection<TGenotype, TFenotype>::CMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem)
{
	v_init(pcProblem);
}//CMigrationSelection<TGenotype, TFenotype>::CMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CMigrationSelection<TGenotype, TFenotype>::~CMigrationSelection()
{

}//CMigrationSelection<TGenotype, TFenotype>::~CMigrationSelection()

template <class TGenotype, class TFenotype>
void CMigrationSelection<TGenotype, TFenotype>::v_init(CProblem<TGenotype, TFenotype> *pcProblem)
{
	pc_problem = pcProblem;
}//void CMigrationSelection<TGenotype, TFenotype>::v_init(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CWorstMigrationSelection<TGenotype, TFenotype>::CWorstMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem)
	: CMigrationSelection<TGenotype, TFenotype>(pcProblem)
{

}//CWorstMigrationSelection<TGenotype, TFenotype>::CWorstMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
void CWorstMigrationSelection<TGenotype, TFenotype>::vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)
{
	sort(ppcPopulation + 0, ppcPopulation + iPopulationSize, [&](CIndividual<TGenotype, TFenotype> *pcIndividual0, CIndividual<TGenotype, TFenotype> *pcIndividual1)
	{
		return pc_problem->bIsBetterIndividual(pcIndividual1, pcIndividual0);
	});

	for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
	{
		*(ppcMigratingIndividuals + i) = new CIndividual<TGenotype, TFenotype>(*(ppcPopulation + i));
	}//for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
}//void CWorstMigrationSelection<TGenotype, TFenotype>::vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)

template class CMigrationSelection<CBinaryCoding, CBinaryCoding>;
template class CMigrationSelection<CRealCoding, CRealCoding>;

template class CWorstMigrationSelection<CBinaryCoding, CBinaryCoding>;
template class CWorstMigrationSelection<CRealCoding, CRealCoding>;