#ifndef MUTATION_UTILS_H
#define MUTATION_UTILS_H

#include "BinaryCoding.h"
#include "Error.h"
#include "GenePatternList.h"
#include "Mutation.h"
#include "RealCoding.h"

#include <istream>

using namespace std;

namespace MutationUtils
{
	template <class TGenotype>
	CMutation<TGenotype> *pcGetMutation(CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace MutationUtils

#endif//MUTATION_UTILS_H