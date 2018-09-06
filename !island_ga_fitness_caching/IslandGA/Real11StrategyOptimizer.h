#ifndef REAL_11_STRATEGY_OPTIMIZER_H
#define REAL_11_STRATEGY_OPTIMIZER_H

#define REAL_11_STRATEGY_OPTIMIZER_1_5_RATIO 0.2

#define REAL_11_STRATEGY_OPTIMIZER_ARGUMENT_INITIAL_SIGMA "initial_sigma"
#define REAL_11_STRATEGY_OPTIMIZER_ARGUMENT_SIGMA_CHANGE_INCREASE_RATIO "sigma_change_increase_ratio"
#define REAL_11_STRATEGY_OPTIMIZER_ARGUMENT_SIGMA_CHANGE_DECREASE_RATIO "sigma_change_decrease_ratio"
#define REAL_11_STRATEGY_OPTIMIZER_ARGUMENT_SIGMA_CHANGE_FREQUENCY "sigma_change_frequency"

#include "Error.h"
#include "Optimizer.h"
#include "RealCoding.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;

class CReal11StrategyOptimizer : public COptimizer<CRealCoding, CRealCoding>
{
public:
	CReal11StrategyOptimizer(CProblem<CRealCoding, CRealCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CReal11StrategyOptimizer(CReal11StrategyOptimizer *pcOther);

	virtual COptimizer<CRealCoding, CRealCoding> *pcCopy() { return new CReal11StrategyOptimizer(this); };

	virtual CError eConfigure(istream *psSettings);

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);
	virtual void vRun();

private:
	void v_update_params(uint32_t iIterationNumber, bool bUpdated);

	uint8_t i_number_of_successes;
	uint8_t i_sigma_change_frequency;

	double d_sigma_change_increase_ratio;
	double d_sigma_change_decrease_ratio;

	double d_initial_sigma;
	double d_sigma;
};//class CReal11StrategyOptimizer : public COptimizer<CRealCoding, CRealCoding>

#endif//REAL_11_STRATEGY_OPTIMIZER_H

