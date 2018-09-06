#ifndef LINKAGE_UTILS_H
#define LINKAGE_UTILS_H

#include "Error.h"
#include "Linkage.h"

#include <istream>

using namespace std;

namespace LinkageUtils
{
	template <class TGenotype>
	CLinkage<TGenotype> *pcGetLinkage(istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace LinkageUtils

#endif//LINKAGE_UTILS_H