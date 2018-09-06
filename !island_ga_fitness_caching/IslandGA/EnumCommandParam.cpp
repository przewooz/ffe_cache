#include "EnumCommandParam.h"

#include "ChangeDetector.h"
#include "Crossover.h"
#include "DynamicOptimizer.h"
#include "Evaluation.h"
#include "GenePatternReplacement.h"
#include "GenePatternSelection.h"
#include "Generation.h"
#include "InertiaWeight.h"
#include "Linkage.h"
#include "MigrationReplacement.h"
#include "MigrationSelection.h"
#include "MigrationTopology.h"
#include "Mutation.h"
#include "Optimizer.h"
#include "PopulationStrategy.h"
#include "Problem.h"
#include "Selection.h"
#include "StopCondition.h"

template <typename TEnum>
CEnumCommandParam<TEnum>::CEnumCommandParam(CString sName, unordered_map<CString, TEnum> *pmMappedValues, bool bIsObligatory)
	: CCommandParam(sName, bIsObligatory, false)
{
	v_init(pmMappedValues, e_default_value);
}//CEnumCommandParam<TEnum>::CEnumCommandParam(CString sName, unordered_map<CString, TEnum> *pmMappedValues, bool bIsObligatory)

template <typename TEnum>
CEnumCommandParam<TEnum>::CEnumCommandParam(CString sName, unordered_map<CString, TEnum> *pmMappedValues, TEnum eDefaultValue)
	: CCommandParam(sName, false, true)
{
	v_init(pmMappedValues, eDefaultValue);
}//CEnumCommandParam<TEnum>::CEnumCommandParam(CString sName, unordered_map<CString, TEnum> *pmMappedValues, TEnum eDefaultValue)

template <typename TEnum>
TEnum CEnumCommandParam<TEnum>::eGetValue(istream *psSettings, CError *pcError)
{
	*pcError = eSetValue(psSettings);
	return e_value;
}//TEnum CEnumCommandParam<TEnum>::eGetValue(istream *psSettings, CError *pcError)

template <typename TEnum>
CError CEnumCommandParam<TEnum>::eSetValue(CString sValue)
{
	CError c_error;
	
	if (pm_mapped_values->count(sValue) > 0)
	{
		e_value = pm_mapped_values->at(sValue);
		v_set_value();
	}//if (pm_mapped_values->count(sValue) > 0)
	else
	{
		CString s_error_message;
		s_error_message.AppendFormat("%s {", sGetName());

		unordered_map<CString, TEnum>::iterator i_iterator = pm_mapped_values->begin();

		while (i_iterator != pm_mapped_values->end())
		{
			s_error_message.Append(i_iterator->first);
			i_iterator++;

			if (i_iterator != pm_mapped_values->end())
			{
				s_error_message.Append(", ");
			}//if (i_iterator != pm_mapped_values->end())
		}//while (i_iterator != pm_mapped_values->end())

		s_error_message.AppendChar('}');

		c_error.vSetError(CError::iERROR_CODE_SYSTEM_OUT_OF_BOUND_ARGUMENT, s_error_message);
	}//else if (pm_mapped_values->count(sValue) > 0)

	return c_error;
}//CError CEnumCommandParam<TEnum>::eSetValue(CString sValue)

template <typename TEnum>
void CEnumCommandParam<TEnum>::v_init(unordered_map<CString, TEnum> *pmMappedValues, TEnum eDefaultValue)
{
	pm_mapped_values = pmMappedValues;
	e_default_value = eDefaultValue;
}//void CEnumCommandParam<TEnum>::v_init(unordered_map<CString, TEnum> *pmMappedValues, TEnum eDefaultValue)


template class CEnumCommandParam<ChangeDetector::EChangeDetectorType>;
template class CEnumCommandParam<ECrossoverType>;
template class CEnumCommandParam<DynamicOptimizer::EDynamicOptimizerType>;
template class CEnumCommandParam<EEvaluationType>;
template class CEnumCommandParam<EGenePatternReplacementType>;
template class CEnumCommandParam<GenePatternSelection::EGenePatternSelectionType>;
template class CEnumCommandParam<EGenerationType>;
template class CEnumCommandParam<InertiaWeight::EInertiaWeightType>;
template class CEnumCommandParam<ELinkageType>;
template class CEnumCommandParam<EMigrationReplacementType>;
template class CEnumCommandParam<EMigrationSelectionType>;
template class CEnumCommandParam<EMigrationTopologyType>;
template class CEnumCommandParam<EMutationType>;
template class CEnumCommandParam<EOptimizerType>;
template class CEnumCommandParam<EPopulationStrategyType>;
template class CEnumCommandParam<EProblemType>;
template class CEnumCommandParam<ESelectionType>;
template class CEnumCommandParam<EStopConditionType>;