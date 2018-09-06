#ifndef MIGRATION_REPLACEMENT_UTILS_H
#define MIGRATION_REPLACEMENT_UTILS_H

#include "BinaryCoding.h"
#include "Error.h"
#include "GenePatternList.h"
#include "MigrationReplacement.h"
#include "Problem.h"

#include <istream>

using namespace std;

namespace MigrationReplacementUtils
{
	template <class TGenotype, class TFenotype>
	CMigrationReplacement<TGenotype, TFenotype> *pcGetMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns,
		istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace MigrationReplacementUtils

#endif//MIGRATION_REPLACEMENT_UTILS_H