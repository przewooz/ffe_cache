#ifndef ISLAND_COEVOLUTION_DSMGA2_H
#define ISLAND_COEVOLUTION_DSMGA2_H

#include "BinaryCoding.h"
#include "DSMGA2.h"
#include "Error.h"
#include "Log.h"
#include "MultiOptimizer.h"
#include "Problem.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;

class CIslandCoevolutionDSMGA2 : public CMultiOptimizer<CBinaryCoding, CBinaryCoding>
{
public:
	CIslandCoevolutionDSMGA2(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);
	CIslandCoevolutionDSMGA2(CIslandCoevolutionDSMGA2 *pcOther);

	virtual ~CIslandCoevolutionDSMGA2();

	virtual CError eConfigure(istream *psSettings);

	virtual COptimizer<CBinaryCoding, CBinaryCoding> *pcCopy() { return new CIslandCoevolutionDSMGA2(this); };

	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

protected:
	virtual CError e_configure_params_optimizer(istream *psSettings);

private:
	void v_clear_params();

	bool b_run_cc(uint32_t iIterationNumber);

	uint32_t i_cc_frequency;
	float f_cc_range;

	CDSMGA2 *pc_cc_dsmga2;
};//class CIslandCoevolutionDSMGA2

#endif//ISLAND_COEVOLUTION_DSMGA2_H
