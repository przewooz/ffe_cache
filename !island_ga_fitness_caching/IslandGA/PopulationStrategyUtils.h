#ifndef POPULATION_STRATEGY_UTILS_H
#define POPULATION_STRATEGY_UTILS_H

#include "Error.h"
#include "PopulationStrategy.h"

#include <istream>

using namespace std;

namespace PopulationStrategyUtils
{
	template <class TGenotype, class TFenotype>
	CPopulationStrategy<TGenotype, TFenotype> *pcGetPopulationStrategy(CProblem<TGenotype, TFenotype> *pcProblem, istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace PopulationStrategyUtils

#endif//POPULATION_STRATEGY_UTILS_H