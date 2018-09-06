#ifndef GENE_PATTERN_REPLACEMENT_UTILS_H
#define GENE_PATTERN_REPLACEMENT_UTILS_H

#include "Error.h"
#include "GenePatternReplacement.h"

#include <istream>

using namespace std;

namespace GenePatternReplacementUtils
{
	CGenePatternReplacement *pcGetGenePatternReplacement(istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace GenePatternReplacementUtils

#endif//GENE_PATTERN_REPLACEMENT_UTILS_H