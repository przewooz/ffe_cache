#ifndef MIGRATION_REPLACEMENT_H
#define MIGRATION_REPLACEMENT_H

#define MIGRATION_REPLACEMENT_ARGUMENT_TYPE "migration_replacement_type"
#define MIGRATION_REPLACEMENT_ARGUMENT_TYPE_MULTI_DETERMINISTIC "multi_deterministic"
#define MIGRATION_REPLACEMENT_ARGUMENT_TYPE_MULTI_RANDOMIZED "multi_randomized"
#define MIGRATION_REPLACEMENT_ARGUMENT_TYPE_WORST "worst"
#define MIGRATION_REPLACEMENT_ARGUMENT_TYPE_RANDOM "random"
#define MIGRATION_REPLACEMENT_ARGUMENT_TYPE_BINARY_GENE_PATTERN_RANDOM "binary_gene_pattern_random"
#define MIGRATION_REPLACEMENT_ARGUMENT_TYPE_BINARY_GENE_PATTERN_BEST "binary_gene_pattern_best"
#define MIGRATION_REPLACEMENT_ARGUMENT_TYPE_BINARY_GENE_PATTERN_BEST_DETERMINISTIC "binary_gene_pattern_best_deterministic"
#define MIGRATION_REPLACEMENT_ARGUMENT_TYPE_BINARY_GENE_PATTERN_WORST "binary_gene_pattern_worst"

#define MIGRATION_REPLACEMENT_ARGUMENT_GENE_PATTERN_EACH_DIFFERENT "migration_replacement_gene_pattern_each_different"

#define MIGRATION_REPLACEMENT_ARGUMENT_NUMBER_OF_MIGRATION_REPLACEMENTS "migration_replacement_number_of_migration_replacements"

#define MIGRATION_REPLACEMENT_ARGUMENT_EACH_DIFFERENT "migration_replacement_each_different"

#include "BinaryCoding.h"
#include "Error.h"
#include "GenePatternList.h"
#include "GenePatternSelection.h"
#include "Individual.h"
#include "Problem.h"

#include <cstdint>
#include <istream>
#include <vector>

using namespace GenePatternSelection;

using namespace std;


enum EMigrationReplacementType
{
	MIGRATION_REPLACEMENT_MULTI_DETERMINISTIC = 0,
	MIGRATION_REPLACEMENT_MULTI_RANDOMIZED = 1,
	MIGRATION_REPLACEMENT_WORST = 2,
	MIGRATION_REPLACEMENT_RANDOM = 3,
	MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_RANDOM = 4,
	MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_BEST = 5,
	MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_BEST_DETERMINISTIC = 6,
	MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_WORST = 7
};//enum EMigrationReplacementType


template <class TGenotype, class TFenotype>
class CMigrationReplacement
{
public:
	CMigrationReplacement();
	CMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem);

	virtual ~CMigrationReplacement();

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings) { return CError(); };

	virtual void vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, 
		uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals) = 0;

	virtual void vUpdateParams() {};

protected:
	CProblem<TGenotype, TFenotype> *pc_problem;

private:
	void v_init(CProblem<TGenotype, TFenotype> *pcProblem);
};//class CMigrationReplacement


template <class TGenotype, class TFenotype>
class CMultiMigrationReplacement : public CMigrationReplacement<TGenotype, TFenotype>
{
public:
	CMultiMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem);

	virtual ~CMultiMigrationReplacement();

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings);

protected:
	vector<CMigrationReplacement<TGenotype, TFenotype>*> v_migration_replacements;

private:
	void v_clear();
};//class CMultiMigrationReplacement : public CMigrationReplacement<TGenotype, TFenotype>


template <class TGenotype, class TFenotype>
class CMultiDeterministicMigrationReplacement : public CMultiMigrationReplacement<TGenotype, TFenotype>
{
public:
	CMultiDeterministicMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem);

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings);

	virtual void vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation,
		uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals);

	virtual void vUpdateParams();

private:
	void v_set_next_index();

	uint8_t i_current_migration_replacement_index;

	bool b_each_different;
};//class CMultiDeterministicMigrationReplacement : public CMultiMigrationReplacement<TGenotype, TFenotype>


template <class TGenotype, class TFenotype>
class CMultiRandomizedMigrationReplacement : public CMultiMigrationReplacement<TGenotype, TFenotype>
{
public:
	CMultiRandomizedMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem);

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings);

	virtual void vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation,
		uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals);

	virtual void vUpdateParams();

private:
	void v_set_random_index();

	bool b_each_different;

	uint8_t i_current_migration_replacement_index;
};//class CMultiRandomizedMigrationReplacement : public CMultiMigrationReplacement<TGenotype, TFenotype>


template <class TFenotype>
class CBinaryGenePatternMigrationReplacement : public CMigrationReplacement<CBinaryCoding, CBinaryCoding>
{
public:
	CBinaryGenePatternMigrationReplacement();
	CBinaryGenePatternMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent);

	virtual ~CBinaryGenePatternMigrationReplacement();

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings);

protected:
	CGenePatternList *pl_gene_patterns;
	bool b_gene_pattern_each_different;
	CGenePatternSelection *pc_gene_pattern_selection;
};//class CBinaryGenePatternRandomMigrationReplacement


template <class TFenotype>
class CBinaryGenePatternRandomMigrationReplacement : public CBinaryGenePatternMigrationReplacement<TFenotype>
{
public:
	CBinaryGenePatternRandomMigrationReplacement();
	CBinaryGenePatternRandomMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent);

	virtual void vReplace(uint32_t iPopulationSize, CIndividual<CBinaryCoding, CBinaryCoding> **ppcPopulation, 
		uint32_t iNumberOfMigratingIndividuals, CIndividual<CBinaryCoding, CBinaryCoding> **ppcMigratingIndividuals);
};//class CBinaryGenePatternRandomMigrationReplacement : public CBinaryGenePatternMigrationReplacement<TFenotype>


template <class TFenotype>
class CBinaryGenePatternBestMigrationReplacement : public CBinaryGenePatternMigrationReplacement<TFenotype>
{
public:
	CBinaryGenePatternBestMigrationReplacement(bool bDeterministic);
	CBinaryGenePatternBestMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent, bool bDeterministic);

	virtual void vReplace(uint32_t iPopulationSize, CIndividual<CBinaryCoding, CBinaryCoding> **ppcPopulation, 
		uint32_t iNumberOfMigratingIndividuals, CIndividual<CBinaryCoding, CBinaryCoding> **ppcMigratingIndividuals);

private:
	bool b_deterministic;
};//class CBinaryGenePatternBestMigrationReplacement : public CBinaryGenePatternMigrationReplacement<TFenotype>


template <class TFenotype>
class CBinaryGenePatternWorstMigrationReplacement : public CBinaryGenePatternMigrationReplacement<TFenotype>
{
public:
	CBinaryGenePatternWorstMigrationReplacement();
	CBinaryGenePatternWorstMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent);

	virtual void vReplace(uint32_t iPopulationSize, CIndividual<CBinaryCoding, CBinaryCoding> **ppcPopulation, 
		uint32_t iNumberOfMigratingIndividuals, CIndividual<CBinaryCoding, CBinaryCoding> **ppcMigratingIndividuals);
};//class CBinaryGenePatternWorstMigrationReplacement : public CBinaryGenePatternMigrationReplacement<TFenotype>

#endif//MIGRATION_REPLACEMENT_H