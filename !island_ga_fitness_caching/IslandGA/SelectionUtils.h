#ifndef SELECTION_UTILS_H
#define SELECTION_UTILS_H

#include "Error.h"
#include "GenePatternList.h"
#include "Selection.h"

#include <istream>

using namespace std;

namespace SelectionUtils
{
	template <class TGenotype, class TFenotype>
	CSelection<TGenotype, TFenotype> *pcGetSelection(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, istream *psSettings, 
		CError *pcError, bool bIsObligatory = true);
}//namespace SelectionUtils

#endif//SELECTION_UTILS_H