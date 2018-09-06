#ifndef INCREMENTAL_OPTIMIZER_H
#define INCREMENTAL_OPTIMIZER_H

#include "Log.h"
#include "Error.h"
#include "Individual.h"
#include "Optimizer.h"
#include "Problem.h"

#include <cstdint>
#include <ctime>
#include <istream>
#include <vector>

using namespace std;

template <class TGenotype, class TFenotype>
class CIncrementalOptimizer : public COptimizer<TGenotype, TFenotype>
{
public:
	CIncrementalOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CIncrementalOptimizer(CIncrementalOptimizer<TGenotype, TFenotype> *pcOther);

	virtual ~CIncrementalOptimizer();

	virtual COptimizer<TGenotype, TFenotype> *pcCopy() { return new CIncrementalOptimizer<TGenotype, TFenotype>(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual void vInitialize(time_t tStartTime) { pc_optimizer->vInitialize(tStartTime); };
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

protected:
	virtual CError e_configure_params_optimizer(istream *psSettings);

	virtual void v_clear_params();

	COptimizer<TGenotype, TFenotype> *pc_optimizer;

	vector<CIndividual<TGenotype, TFenotype>*> v_best_individuals;
};//class CIncrementalOptimizer : public COptimizer<TGenotype, TFenotype>

#endif//INCREMENTAL_OPTIMIZER_H