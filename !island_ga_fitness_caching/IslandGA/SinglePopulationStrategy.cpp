#include "SinglePopulationStrategy.h"

#include "BinaryCoding.h"
#include "RealCoding.h"

template <class TGenotype, class TFenotype>
CSinglePopulationStrategy<TGenotype, TFenotype>::CSinglePopulationStrategy()
	: CPopulationStrategy<TGenotype, TFenotype>(DEF_SINGLE_STRATEGY_INIT_NUMBER_OF_POPULATIONS)
{

}//CSinglePopulationStrategy<TGenotype, TFenotype>::CSinglePopulationStrategy()

template class CSinglePopulationStrategy<CBinaryCoding, CBinaryCoding>;
template class CSinglePopulationStrategy<CRealCoding, CRealCoding>;