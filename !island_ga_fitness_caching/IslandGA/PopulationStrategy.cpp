#include "PopulationStrategy.h"

#include "BinaryCoding.h"
#include "RealCoding.h"
#include "UIntCommandParam.h"

template <class TGenotype, class TFenotype>
uint32_t CPopulationStrategy<TGenotype, TFenotype>::iERROR_PARENT_CPopulationStrategy = CError::iADD_ERROR_PARENT("iERROR_PARENT_CPopulationStrategy");

template <class TGenotype, class TFenotype>
CPopulationStrategy<TGenotype, TFenotype>::CPopulationStrategy()
{
	v_init(0, nullptr);
}//CPopulationStrategy<TGenotype, TFenotype>::CPopulationStrategy()

template <class TGenotype, class TFenotype>
CPopulationStrategy<TGenotype, TFenotype>::CPopulationStrategy(uint8_t iInitNumberOfPopulations)
{
	v_init(iInitNumberOfPopulations, nullptr);
}//CPopulationStrategy<TGenotype, TFenotype>::CPopulationStrategy(uint8_t iInitNumberOfPopulations)

template <class TGenotype, class TFenotype>
CPopulationStrategy<TGenotype, TFenotype>::CPopulationStrategy(uint8_t iInitNumberOfPopulations, CProblem<TGenotype, TFenotype> *pcProblem)
{
	v_init(iInitNumberOfPopulations, pcProblem);
}//CPopulationStrategy<TGenotype, TFenotype>::CPopulationStrategy(uint8_t iInitNumberOfPopulations, CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CPopulationStrategy<TGenotype, TFenotype>::~CPopulationStrategy()
{

}//CPopulationStrategy<TGenotype, TFenotype>::~CPopulationStrategy()

template <class TGenotype, class TFenotype>
void CPopulationStrategy<TGenotype, TFenotype>::v_init(uint8_t iInitNumberOfPopulations, CProblem<TGenotype, TFenotype> *pcProblem)
{
	i_init_number_of_populations = iInitNumberOfPopulations;
	pc_problem = pcProblem;
}//void CPopulationStrategy<TGenotype, TFenotype>::v_init(uint8_t iInitNumberOfPopulations, CProblem<TGenotype, TFenotype> *pcProblem)


template <class TGenotype, class TFenotype>
CConstantPopulationStrategy<TGenotype, TFenotype>::CConstantPopulationStrategy()
{

}//CConstantPopulationStrategy<TGenotype, TFenotype>::CConstantPopulationStrategy()

template <class TGenotype, class TFenotype>
CConstantPopulationStrategy<TGenotype, TFenotype>::CConstantPopulationStrategy(uint8_t iInitNumberOfPopulations)
	: CPopulationStrategy<TGenotype, TFenotype>(iInitNumberOfPopulations)
{

}//CConstantPopulationStrategy<TGenotype, TFenotype>::CConstantPopulationStrategy(uint8_t iInitNumberOfPopulations)

template <class TGenotype, class TFenotype>
CError CConstantPopulationStrategy<TGenotype, TFenotype>::eConfigure(istream *psStream)
{
	CError c_error = CPopulationStrategy<TGenotype, TFenotype>::eConfigure(psStream);

	if (!c_error)
	{
		CUIntCommandParam p_number_of_populations(POPULATION_STRATEGY_ARGUMENT_NUMBER_OF_POPULATIONS, 1, UINT8_MAX);
		i_init_number_of_populations = (uint8_t)p_number_of_populations.iGetValue(psStream, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CConstantPopulationStrategy<TGenotype, TFenotype>::eConfigure(istream *psStream)

template class CPopulationStrategy<CBinaryCoding, CBinaryCoding>;
template class CPopulationStrategy<CRealCoding, CRealCoding>;

template class CConstantPopulationStrategy<CBinaryCoding, CBinaryCoding>;
template class CConstantPopulationStrategy<CRealCoding, CRealCoding>;