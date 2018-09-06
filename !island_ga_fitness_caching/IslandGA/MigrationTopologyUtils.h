#ifndef MIGRATION_TOPOLOGY_UTILS_H
#define MIGRATION_TOPOLOGY_UTILS_H

#include "Error.h"
#include "GenePatternList.h"
#include "MigrationTopology.h"
#include "Problem.h"

#include <istream>

using namespace std;

namespace MigrationTopologyUtils
{
	template <class TGenotype, class TFenotype>
	CMigrationTopology<TGenotype, TFenotype> *pcGetMigrationTopology(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, 
		istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace MigrationTopologyUtils

#endif//MIGRATION_TOPOLOGY_UTILS_H