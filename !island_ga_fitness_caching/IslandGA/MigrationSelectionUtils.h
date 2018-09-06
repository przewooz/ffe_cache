#ifndef MIGRATION_SELECTION_UTILS_H
#define MIGRATION_SELECTION_UTILS_H

#include "Error.h"
#include "GenePatternList.h"
#include "MigrationSelection.h"
#include "Problem.h"

#include <istream>

using namespace std;

namespace MigrationSelectionUtils
{
	template <class TGenotype, class TFenotype>
	CMigrationSelection<TGenotype, TFenotype> *pcGetMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, 
		istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace MigrationSelectionUtils

#endif//MIGRATION_SELECTION_UTILS_H