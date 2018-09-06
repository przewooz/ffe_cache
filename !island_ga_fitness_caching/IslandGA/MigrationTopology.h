#ifndef MIGRATION_TOPOLOGY_H
#define MIGRATION_TOPOLOGY_H

#define MIGRATION_TOPOLOGY_ARGUMENT_TYPE "migration_topology_type"
#define MIGRATION_TOPOLOGY_ARGUMENT_TYPE_RANDOM "random"
#define MIGRATION_TOPOLOGY_ARGUMENT_TYPE_GENE_PATTERN_MIN_DISTANCE "gene_pattern_min_distance"
#define MIGRATION_TOPOLOGY_ARGUMENT_TYPE_GENE_PATTERN_MAX_DISTANCE "gene_pattern_max_distance"
#define MIGRATION_TOPOLOGY_ARGUMENT_TYPE_RING "ring"

#include "Error.h"
#include "GenePatternList.h"
#include "Individual.h"
#include "Problem.h"

#include <cstdint>
#include <istream>

using namespace std;


enum EMigrationTopologyType
{
	MIGRATION_TOPOLOGY_RANDOM = 0,
	MIGRATION_TOPOLOGY_GENE_PATTERN_MIN_DISTANCE = 1,
	MIGRATION_TOPOLOGY_GENE_PATTERN_MAX_DISTANCE = 2,
	MIGRATION_TOPOLOGY_RING = 3
};//enum EMigrationTopologyType


template <class TGenotype, class TFenotype>
class CMigrationTopology
{
public:
	CMigrationTopology();
	CMigrationTopology(CProblem<TGenotype, TFenotype> *pcProblem);

	virtual ~CMigrationTopology();

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings) { return CError(); };

	virtual void vGetMigrations(uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> **ppcBestPopulationIndividuals, 
		uint8_t *piMigrationsFrom, uint8_t *piMigrationsTo) = 0;

protected:
	CProblem<TGenotype, TFenotype> *pc_problem;

private:
	void v_init(CProblem<TGenotype, TFenotype> *pcProblem);
};//class CMigrationTopology

template <class TGenotype, class TFenotype>
class CRandomMigrationTopology : public CMigrationTopology<TGenotype, TFenotype>
{
public:
	virtual void vGetMigrations(uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> **ppcBestPopulationIndividuals, 
		uint8_t *piMigrationsFrom, uint8_t *piMigrationsTo);
};//class CRandomMigrationTopology

template <class TGenotype, class TFenotype>
class CGenePatternDistanceTopology : public CMigrationTopology<TGenotype, TFenotype>
{
public:
	CGenePatternDistanceTopology(bool bMinDistance);
	CGenePatternDistanceTopology(CGenePatternList *plGenePatterns, bool bMinDistance);

	virtual CError eConfigure(CGenePatternList *plGenePatterns, istream *psSettings);

	virtual void vGetMigrations(uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> **ppcBestPopulationIndividuals, 
		uint8_t *piMigrationsFrom, uint8_t *piMigrationsTo);

private:
	void v_init(bool bMinDistance);

	CGenePatternList *pl_gene_patterns;
	bool b_min_distance;

	CRandomMigrationTopology<TGenotype, TFenotype> c_random_migration_topology;
};//class CGenePatternDistanceTopology

template <class TGenotype, class TFenotype>
class CRingMigrationTopology : public CMigrationTopology<TGenotype, TFenotype>
{
public:
	virtual void vGetMigrations(uint8_t iNumberOfPopulations, CIndividual<TGenotype, TFenotype> **ppcBestPopulationIndividuals, 
		uint8_t *piMigrationsFrom, uint8_t *piMigrationsTo);
};//class CRingMigrationTopology

#endif//MIGRATION_TOPOLOGY_H