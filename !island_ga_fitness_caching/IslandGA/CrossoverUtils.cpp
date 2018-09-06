#include "CrossoverUtils.h"

#include "BinaryOnePointCrossover.h"
#include "BinaryTwoPointsCrossover.h"
#include "BinaryUniformCrossover.h"
#include "EnumCommandParam.h"
#include "RealCrossover.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>
#include <utility>

template <class TGenotype>
CCrossover<TGenotype> * CrossoverUtils::pcGetCrossover(istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CCrossover<TGenotype> *pc_crossover = nullptr;

	size_t i_genotype_type_hash_code = typeid(TGenotype).hash_code();

	unordered_map<CString, ECrossoverType> m_crossover_types;

	if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		m_crossover_types.insert(pair<const CString, ECrossoverType>(CROSSOVER_ARGUMENT_TYPE_BINARY_ONE_POINT, CROSSOVER_BINARY_ONE_POINT));
		m_crossover_types.insert(pair<const CString, ECrossoverType>(CROSSOVER_ARGUMENT_TYPE_BINARY_TWO_POINTS, CROSSOVER_BINARY_TWO_POINTS));
		m_crossover_types.insert(pair<const CString, ECrossoverType>(CROSSOVER_ARGUMENT_TYPE_BINARY_UNIFORM, CROSSOVER_BINARY_UNIFORM));
	}//if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	else if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code())
	{
		m_crossover_types.insert(pair<const CString, ECrossoverType>(CROSSOVER_ARGUMENT_TYPE_REAL_LINEAR, CROSSOVER_REAL_LINEAR));
		m_crossover_types.insert(pair<const CString, ECrossoverType>(CROSSOVER_ARGUMENT_TYPE_REAL_BIN, CROSSOVER_REAL_BIN));
	}//else if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code())

	CEnumCommandParam<ECrossoverType> p_type(CROSSOVER_ARGUMENT_TYPE, &m_crossover_types, bIsObligatory);
	ECrossoverType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case CROSSOVER_BINARY_ONE_POINT:
			{
				pc_crossover = (CCrossover<TGenotype>*)new CBinaryOnePointCrossover();
				break;
			}//case CROSSOVER_BINARY_ONE_POINT
			case CROSSOVER_BINARY_TWO_POINTS:
			{
				pc_crossover = (CCrossover<TGenotype>*)new CBinaryTwoPointsCrossover();
				break;
			}//case CROSSOVER_BINARY_TWO_POINTS
			case CROSSOVER_BINARY_UNIFORM:
			{
				pc_crossover = (CCrossover<TGenotype>*)new CBinaryUniformCrossover();
				break;
			}//case CROSSOVER_BINARY_UNIFORM
			case CROSSOVER_REAL_LINEAR:
			{
				pc_crossover = (CCrossover<TGenotype>*)new CRealLinearCrossover();
				break;
			}//case CROSSOVER_REAL_LINEAR
			case CROSSOVER_REAL_BIN:
			{
				pc_crossover = (CCrossover<TGenotype>*)new CRealBinCrossover();
				break;
			}//case CROSSOVER_REAL_BIN
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "crossover");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_crossover->eConfigure(psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_crossover;
}//CCrossover<TGenotype> * CrossoverUtils::pcGetCrossover(istream *psSettings, CError *pcError, bool bIsObligatory)


template CCrossover<CBinaryCoding> * CrossoverUtils::pcGetCrossover(istream*, CError*, bool);
template CCrossover<CRealCoding> * CrossoverUtils::pcGetCrossover(istream*, CError*, bool);