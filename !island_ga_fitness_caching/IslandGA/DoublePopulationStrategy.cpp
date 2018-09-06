#include "DoublePopulationStrategy.h"

#include "BinaryCoding.h"
#include "RealCoding.h"

template <class TGenotype, class TFenotype>
CDoublePopulationStrategy<TGenotype, TFenotype>::CDoublePopulationStrategy()
	: CPopulationStrategy<TGenotype, TFenotype>(DEF_DOUBLE_STRATEGY_INIT_NUMBER_OF_POPULATIONS, nullptr)
{

}//CSinglePopulationStrategy<TGenotype, TFenotype>::CDoublePopulationStrategy()

template class CDoublePopulationStrategy<CBinaryCoding, CBinaryCoding>;
template class CDoublePopulationStrategy<CRealCoding, CRealCoding>;