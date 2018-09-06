#include "InertiaWeight.h"

#include "EnumCommandParam.h"
#include "FloatCommandParam.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <cfloat>
#include <cmath>
#include <unordered_map>

using namespace InertiaWeight;


CError CConstantInertiaWeight::eConfigure(istream *psSettings)
{
	CError c_error = CInertiaWeight::eConfigure(psSettings);

	if (!c_error)
	{
		CFloatCommandParam p_constant_value(INERTIA_WEIGHT_ARGUMENT_CONSTANT_VALUE, 0, FLT_MAX);
		d_constant_value = (double)p_constant_value.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CConstantInertiaWeight::eConfigure(istream *psSettings)


CError CLogarithmDecreasingInertiaWeight::eConfigure(istream *psSettings)
{
	CError c_error = CInertiaWeight::eConfigure(psSettings);

	if (!c_error)
	{
		CFloatCommandParam p_min_value(INERTIA_WEIGHT_ARGUMENT_MIN_VALUE, 0, FLT_MAX);
		d_min_value = (double)p_min_value.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CFloatCommandParam p_max_value(INERTIA_WEIGHT_ARGUMENT_MAX_VALUE, (float)d_min_value, FLT_MAX);
		d_max_value = (double)p_max_value.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CFloatCommandParam p_a(INERTIA_WEIGHT_ARGUMENT_A);
		d_a = (double)p_a.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CLogarithmDecreasingInertiaWeight::eConfigure(istream *psSettings)

double CLogarithmDecreasingInertiaWeight::dGetValue(double dNow, double dMax)
{
	double d_value = 0;

	d_value = log10(d_a + 10 * dNow / dMax);
	d_value *= d_min_value - d_max_value;
	d_value += d_max_value;

	return d_value;
}//double CLogarithmDecreasingInertiaWeight::dGetValue(double dNow, double dMax)


CInertiaWeight * InertiaWeight::pcGetInertiaWeight(istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CInertiaWeight *pc_inertia_weight = nullptr;

	unordered_map<CString, EInertiaWeightType> m_inertia_weight_types;
	m_inertia_weight_types.insert(pair<const CString, EInertiaWeightType>(INERTIA_WEIGHT_ARGUMENT_TYPE_CONSTANT, INERTIA_WEIGHT_CONSTANT));
	m_inertia_weight_types.insert(pair<const CString, EInertiaWeightType>(INERTIA_WEIGHT_ARGUMENT_TYPE_RANDOM, INERTIA_WEIGHT_RANDOM));
	m_inertia_weight_types.insert(pair<const CString, EInertiaWeightType>(INERTIA_WEIGHT_ARGUMENT_TYPE_LOGARITHM_DECREASING, INERTIA_WEIGHT_LOGARITHM_DECREASING));

	CEnumCommandParam<EInertiaWeightType> p_type(INERTIA_WEIGHT_ARGUMENT_TYPE, &m_inertia_weight_types);
	EInertiaWeightType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError)
	{
		switch (e_type)
		{
			case INERTIA_WEIGHT_CONSTANT:
			{
				pc_inertia_weight = new CConstantInertiaWeight();
				break;
			}//case INERTIA_WEIGHT_CONSTANT
			case INERTIA_WEIGHT_RANDOM:
			{
				pc_inertia_weight = new CRandomInertiaWeight();
				break;
			}//case INERTIA_WEIGHT_RANDOM
			case INERTIA_WEIGHT_LOGARITHM_DECREASING:
			{
				pc_inertia_weight = new CLogarithmDecreasingInertiaWeight();
				break;
			}//case INERTIA_WEIGHT_LOGARITHM_DECREASING
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "inertia weight");
				break;
			}//default
		}//switch (e_type)

		if (!*pcError)
		{
			*pcError = pc_inertia_weight->eConfigure(psSettings);
		}//if (!*pcError)
	}//if (!*pcError)

	return pc_inertia_weight;
}//CInertiaWeight * InertiaWeight::pcGetInertiaWeight(istream *psSettings, CError *pcError, bool bIsObligatory)