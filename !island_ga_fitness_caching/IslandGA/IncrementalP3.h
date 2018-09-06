#ifndef INCREMENTAL_P3_H
#define INCREMENTAL_P3_H

#define INCREMENTAL_P3_ARGUMENT_ADD_ONLY_LAST_BEST "add_only_last_best"

#define INCREMENTAL_P3_ARGUMENT_CLIMB "climb"

#include "BinaryCoding.h"
#include "Error.h"
#include "Log.h"
#include "IncrementalOptimizer.h"
#include "Problem.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;

class CIncrementalP3 : public CIncrementalOptimizer<CBinaryCoding, CBinaryCoding>
{
public:
	CIncrementalP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CIncrementalP3(CIncrementalP3 *pcOther);

	virtual COptimizer<CBinaryCoding, CBinaryCoding> *pcCopy() { return new CIncrementalP3(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

protected:
	virtual CError e_configure_params_optimizer(istream *psSettings);

	bool b_add_only_last_best;
	bool b_climb;
};//class CIncrementalP3 : public CIncrementalOptimizer<CBinaryCoding, CBinaryCoding>

#endif//INCREMENTAL_P3_H