#ifndef SELECTION_H
#define SELECTION_H

#define SELECTION_ARGUMENT_TYPE "selection_type"
#define SELECTION_ARGUMENT_TYPE_TOURNAMENT "tournament"
#define SELECTION_ARGUMENT_TYPE_GENE_PATTERN_TOURNAMENT "gene_pattern_tournament"

#define SELECTION_ARGUMENT_ELITISM "selection_elitism"
#define SELECTION_ARGUMENT_ELITISM_DEFAULT false

#define SELECTION_ARGUMENT_TOURNAMENT_SIZE "tournament_size"
#define SELECTION_ARGUMENT_TOURNAMENT_SIZE_DEFAULT 2

#define SELECTION_ARGUMENT_ONLY_GENE_PATTERN_DISTANCE "only_gene_pattern_distance"

#include "Error.h"
#include "GenePatternList.h"
#include "Individual.h"
#include "Problem.h"

#include <cstdint>
#include <istream>

using namespace std;


enum ESelectionType
{
	SELECTION_TOURNAMENT = 0,
	SELECTION_GENE_PATTERN_TOURNAMENT = 1,
};//enum ESelectionType


template <class TGenotype, class TFenotype>
class CSelection
{
public:
	CSelection(CProblem<TGenotype, TFenotype> *pcProblem);

	virtual ~CSelection();

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings) { return CError(); };

	virtual void vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcOldPopulation, CIndividual<TGenotype, TFenotype> **ppcNewPopulation,
		CIndividual<TGenotype, TFenotype> *pcBestIndividual) = 0;

protected:
	CProblem<TGenotype, TFenotype> *pc_problem;
};//class CSelection

template <class TGenotype, class TFenotype>
class CTournamentSelection : public CSelection<TGenotype, TFenotype>
{
public:
	CTournamentSelection(CProblem<TGenotype, TFenotype> *pcProblem);
	CTournamentSelection(uint32_t iTournamentSize, bool bElitism, CProblem<TGenotype, TFenotype> *pcProblem);

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings);

	virtual void vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcOldPopulation, CIndividual<TGenotype, TFenotype> **ppcNewPopulation,
		CIndividual<TGenotype, TFenotype> *pcBestIndividual);

	uint32_t iGetTournamentSize() { return i_tournament_size; }

private:
	uint32_t i_tournament_size;
	bool b_elitism;
};//class CTournamentSelection

template <class TGenotype, class TFenotype>
class CGenePatternTournamentSelection : public CSelection<TGenotype, TFenotype>
{
public:
	CGenePatternTournamentSelection(CProblem<TGenotype, TFenotype> *pcProblem);
	CGenePatternTournamentSelection(uint32_t iTournamentSize, CGenePatternList *plGenePatterns, bool bOnlyGenePatternDistance, CProblem<TGenotype, TFenotype> *pcProblem);

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings);

	virtual void vSelect(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcOldPopulation, CIndividual<TGenotype, TFenotype> **ppcNewPopulation,
		CIndividual<TGenotype, TFenotype> *pcBestIndividual);

private:
	CTournamentSelection<TGenotype, TFenotype> c_tournament_selection;

	uint32_t i_tournament_size;
	CGenePatternList *pl_gene_patterns;
	bool b_only_gene_pattern_distance;
};//class CGenePatternTournamentSelection

#endif//SELECTION_H