#ifndef MULTI_DIFFERENT_OPTIMIZER_H
#define MULTI_DIFFERENT_OPTIMIZER_H

#define MULTI_DIFFERENT_OPTIMIZER_ARGUMENT_NUMBER_OF_OPTIMIZERS "number_of_optimizers"

#include "Error.h"
#include "Log.h"
#include "MultiOptimizer.h"
#include "Optimizer.h"
#include "OptimizerUtils.h"
#include "Problem.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;

template <class TGenotype, class TFenotype>
class CMultiDifferentOptimizer : public CMultiOptimizer<TGenotype, TFenotype>
{
public:
	CMultiDifferentOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CMultiDifferentOptimizer(CMultiDifferentOptimizer<TGenotype, TFenotype> *pcOther);

	virtual COptimizer<TGenotype, TFenotype> *pcCopy() { return new CMultiDifferentOptimizer<TGenotype, TFenotype>(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

protected:
	virtual CError e_configure_optimizers(uint8_t iNumberOfOptimizers, istream *psSettings);
	virtual COptimizer<TGenotype, TFenotype> *pc_get_single_optimizer(istream *psSettings, CError *pcError) { return OptimizerUtils::pcGetOptimizer(pc_problem, pc_log, i_random_seed, psSettings, pcError); };
};//class CMultiIndependentOptimizer : public COptimizer<TGenotype, TFenotype>

#endif//MULTI_DIFFERENT_OPTIMIZER_H