#ifndef LINKAGE_H
#define LINKAGE_H

#define LINKAGE_ARGUMENT_TYPE "linkage_type"
#define LINKAGE_ARGUMENT_TYPE_BINARY_DIFFERENTIAL "binary_differential"

#include "Error.h"
#include "GenePattern.h"

#include <istream>

using namespace std;


enum ELinkageType
{
	LINKAGE_BINARY_DIFFERENTIAL = 0
};//ELinkageType


template <class TGenotype>
class CLinkage
{
public:
	virtual ~CLinkage();

	virtual CError eConfigure(istream *psSettings) { return CError(); };

	virtual CGenePattern *pcGetGenePattern(TGenotype *pcGenotype0, TGenotype *pcGenotype1) = 0;
};//class CLinkage

#endif//LINKAGE_H