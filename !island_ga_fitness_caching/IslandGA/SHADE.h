#ifndef SHADE_H
#define SHADE_H

#include "Error.h"
#include "Log.h"
#include "Optimizer.h"
#include "Problem.h"
#include "RealCoding.h"

#include "../SHADE/de.h"

#include <ctime>
#include <cstdint>
#include <istream>

class CSHADE : COptimizer<CRealCoding, CRealCoding>
{
public:
	CSHADE(CProblem<CRealCoding, CRealCoding> *pcProblem, CLog *pcLog);
	CSHADE(CSHADE *pcOther);

	virtual ~CSHADE();

	virtual CError eConfigure(istream *psSettings);

	virtual COptimizer<CRealCoding, CRealCoding> *pcCopy() { return new CSHADE(this); };

	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime) { return false; };

	virtual void vRun();

private:
	uint32_t i_population_size;
	double d_arc_rate;
	double d_best_rate;

	SHADE *pc_shade;
};//class CSHADE : COptimizer<CRealCoding, CRealCoding>

#endif//SHADE_H