#ifndef BINARY_DIFFERENTIAL_LINKAGE_H
#define BINARY_DIFFERENTIAL_LINKAGE_H

#include "BinaryCoding.h"
#include "GenePattern.h"
#include "Linkage.h"

class CBinaryDifferentialLinkage : public CLinkage<CBinaryCoding>
{
public:
	virtual CGenePattern *pcGetGenePattern(CBinaryCoding *pcGenotype0, CBinaryCoding *pcGenotype1);
};//class CBinaryDifferentialLinkage

#endif//BINARY_DIFFERENTIAL_LINKAGE_H