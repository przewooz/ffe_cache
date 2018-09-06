#ifndef DUMMY_OPTIMIZER_H
#define DUMMY_OPTIMIZER_H

#define DUMMY_OPTIMIZER_ARGUMENT_LOG_FREQUENCY "log_frequency"

#include "BinaryCoding.h"
#include "BinaryOptimizer.h"
#include "Error.h"
#include "Log.h"
#include "Optimizer.h"
#include "Problem.h"

#include <istream>

class CDummyOptimizer : public CBinaryOptimizer
{
public:
	static uint32_t iERROR_CDummyOptimizer;

	CDummyOptimizer(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CDummyOptimizer(CDummyOptimizer *pcOther);

	virtual COptimizer<CBinaryCoding, CBinaryCoding> *pcCopy() { return new CDummyOptimizer(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

private:
	time_t t_log_frequency;
};//class CDummyOptimizer : public CBinaryOptimizer

#endif//DUMMY_OPTIMIZER_H