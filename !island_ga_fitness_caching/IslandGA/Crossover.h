#ifndef CROSSOVER_H
#define CROSSOVER_H

#define CROSSOVER_ARGUMENT_TYPE "crossover_type"
#define CROSSOVER_ARGUMENT_TYPE_BINARY_ONE_POINT "binary_one_point"
#define CROSSOVER_ARGUMENT_TYPE_BINARY_TWO_POINTS "binary_two_points"
#define CROSSOVER_ARGUMENT_TYPE_BINARY_UNIFORM "binary_uniform"
#define CROSSOVER_ARGUMENT_TYPE_REAL_LINEAR "real_linear"
#define CROSSOVER_ARGUMENT_TYPE_REAL_BIN "real_bin"

#define CROSSOVER_ARGUMENT_PROBABILITY "crossover_probability"
#define CROSSOVER_ARGUMENT_CONSTANT "crossover_constant"

#include "Error.h"
#include "RandUtils.h"

#include <istream>

using namespace std;


enum ECrossoverType
{
	CROSSOVER_BINARY_ONE_POINT = 0,
	CROSSOVER_BINARY_TWO_POINTS = 1,
	CROSSOVER_BINARY_UNIFORM = 2,
	CROSSOVER_REAL_LINEAR = 3,
	CROSSOVER_REAL_BIN = 4
};//enum ECrossoverType


template <class TGenotype>
class CCrossover
{
public:
	CCrossover();
	CCrossover(float fProbability);

	virtual ~CCrossover();

	virtual CError eConfigure(istream *psSettings);

	virtual bool bApplyCrossover(TGenotype *pcGenotype) { return RandUtils::bSatisfyProbability(f_probability); };
	virtual void vCrossover(TGenotype *pcParent0, TGenotype *pcParent1, TGenotype *pcChild0, TGenotype *pcChild1) = 0;

protected:
	float f_probability;
};//class CCrossover

#endif//CROSSOVER_H