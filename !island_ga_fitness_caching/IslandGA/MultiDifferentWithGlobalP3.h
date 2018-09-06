#ifndef MULTI_DIFFERENT_WITH_GLOBAL_P3
#define MULTI_DIFFERENT_WITH_GLOBAL_P3

#define MULTI_DIFFERENT_WITH_GLOBAL_P3_ARGUMENT_GLOBAL_RUN_FREQUENCY "global_run_frequency"

#include "BinaryCoding.h"
#include "Error.h"
#include "Log.h"
#include "MultiDifferentOptimizer.h"
#include "Optimizer.h"
#include "P3.h"
#include "Problem.h"

#include <cstdint>
#include <ctime>
#include <istream>

class CMultiDifferentWithGlobalP3 : CMultiDifferentOptimizer<CBinaryCoding, CBinaryCoding>
{
public:
	CMultiDifferentWithGlobalP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CMultiDifferentWithGlobalP3(CMultiDifferentWithGlobalP3 *pcOther);

	virtual COptimizer<CBinaryCoding, CBinaryCoding> *pcCopy() { return new CMultiDifferentWithGlobalP3(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

protected:
	virtual COptimizer<CBinaryCoding, CBinaryCoding> *pc_get_single_optimizer(istream *psSettings, CError *pcError);

private:
	uint32_t i_global_run_frequency;
	CP3 c_global_p3;
};//class CMultiDifferentWithGlobalP3 : CMultiDifferentOptimizer<CBinaryCoding, CBinaryCoding>

#endif//MULTI_DIFFERENT_WITH_GLOBAL_P3