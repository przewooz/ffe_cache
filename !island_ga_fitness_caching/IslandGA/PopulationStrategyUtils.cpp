#include "PopulationStrategyUtils.h"

#include "ActivePopulationStrategy.h"
#include "BinaryCoding.h"
#include "ClassicPopulationStrategy.h"
#include "DoublePopulationStrategy.h"
#include "EnumCommandParam.h"
#include "Problem.h"
#include "RealCoding.h"
#include "SinglePopulationStrategy.h"
#include "StringUtils.h"

#include <cstdint>
#include <unordered_map>

template <class TGenotype, class TFenotype>
CPopulationStrategy<TGenotype, TFenotype> *PopulationStrategyUtils::pcGetPopulationStrategy(CProblem<TGenotype, TFenotype> *pcProblem, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CPopulationStrategy<TGenotype, TFenotype> *pc_population_strategy;
	pc_population_strategy = nullptr;

	unordered_map<CString, EPopulationStrategyType> m_population_strategy_types;
	m_population_strategy_types.insert(pair<const CString, EPopulationStrategyType>(POPULATION_STRATEGY_ARGUMENT_TYPE_CLASSIC, POPULATION_STRATEGY_CLASSIC));
	m_population_strategy_types.insert(pair<const CString, EPopulationStrategyType>(POPULATION_STRATEGY_ARGUMENT_TYPE_ACTIVE, POPULATION_STRATEGY_ACTIVE));
	m_population_strategy_types.insert(pair<const CString, EPopulationStrategyType>(POPULATION_STRATEGY_ARGUMENT_TYPE_SINGLE, POPULATION_STRATEGY_SINGLE));
	m_population_strategy_types.insert(pair<const CString, EPopulationStrategyType>(POPULATION_STRATEGY_ARGUMENT_TYPE_DOUBLE, POPULATION_STRATEGY_DOUBLE));
	m_population_strategy_types.insert(pair<const CString, EPopulationStrategyType>(POPULATION_STRATEGY_ARGUMENT_TYPE_CONSTANT, POPULATION_STRATEGY_CONSTANT));

	CEnumCommandParam<EPopulationStrategyType> p_type(POPULATION_STRATEGY_ARGUMENT_TYPE, &m_population_strategy_types, bIsObligatory);
	EPopulationStrategyType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case POPULATION_STRATEGY_CLASSIC:
			{
				pc_population_strategy = new CClassicPopulationStrategy<TGenotype, TFenotype>(pcProblem);
				break;
			}//case POPULATION_STRATEGY_CLASSIC
			case POPULATION_STRATEGY_ACTIVE:
			{
				pc_population_strategy = new CActivePopulationStrategy<TGenotype, TFenotype>(pcProblem);
				break;
			}//case POPULATION_STRATEGY_CLASSIC POPULATION_STRATEGY_ACTIVE
			case POPULATION_STRATEGY_SINGLE:
			{
				pc_population_strategy = new CSinglePopulationStrategy<TGenotype, TFenotype>();
				break;
			}//case POPULATION_STRATEGY_SINGLE
			case POPULATION_STRATEGY_DOUBLE:
			{
				pc_population_strategy = new CDoublePopulationStrategy<TGenotype, TFenotype>();
				break;
			}//case POPULATION_STRATEGY_DOUBLE
			case POPULATION_STRATEGY_CONSTANT:
			{
				pc_population_strategy = new CConstantPopulationStrategy<TGenotype, TFenotype>();
				break;
			}//case POPULATION_STRATEGY_CONSTANT
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "population strategy");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_population_strategy->eConfigure(psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_population_strategy;
}//CPopulationStrategy<TGenotype, TFenotype> *PopulationStrategyUtils::pcGetPopulationStrategy(CProblem<TGenotype, TFenotype> *pcProblem, istream *psSettings, CError *pcError, bool bIsObligatory)

template CPopulationStrategy<CBinaryCoding, CBinaryCoding> * PopulationStrategyUtils::pcGetPopulationStrategy(CProblem<CBinaryCoding, CBinaryCoding>*, istream*, CError*, bool);
template CPopulationStrategy<CRealCoding, CRealCoding> * PopulationStrategyUtils::pcGetPopulationStrategy(CProblem<CRealCoding, CRealCoding>*, istream*, CError*, bool);