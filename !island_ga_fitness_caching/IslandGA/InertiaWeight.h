#ifndef INERTIA_WEIGHT_T
#define INERTIA_WEIGHT_T

#define INERTIA_WEIGHT_ARGUMENT_TYPE "inertia_weight_type"
#define INERTIA_WEIGHT_ARGUMENT_TYPE_CONSTANT "constant"
#define INERTIA_WEIGHT_ARGUMENT_TYPE_RANDOM "random"
#define INERTIA_WEIGHT_ARGUMENT_TYPE_LOGARITHM_DECREASING "logarithm_decreasing"

#define INERTIA_WEIGHT_ARGUMENT_MIN_VALUE "inertia_weight_min_value"
#define INERTIA_WEIGHT_ARGUMENT_MAX_VALUE "inertia_weight_max_value"

#define INERTIA_WEIGHT_ARGUMENT_CONSTANT_VALUE "inertia_weight_constant_value"

#define INERTIA_WEIGHT_ARGUMENT_A "inertia_weight_a"

#include "Error.h"
#include "RandUtils.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;


namespace InertiaWeight
{
	enum EInertiaWeightType
	{
		INERTIA_WEIGHT_CONSTANT = 0,
		INERTIA_WEIGHT_RANDOM = 1,
		INERTIA_WEIGHT_LOGARITHM_DECREASING = 2
	};//enum EStopConditionType


	class CInertiaWeight
	{
	public:
		CInertiaWeight() { };

		virtual ~CInertiaWeight() { };

		virtual CError eConfigure(istream *psSettings) { return CError(); }

		virtual double dGetValue(double dNow, double dMax) = 0;
	};//class CInertiaWeight


	class CConstantInertiaWeight : public CInertiaWeight
	{
	public:
		virtual CError eConfigure(istream *psSettings);

		virtual double dGetValue(double dNow, double dMax) { return d_constant_value; }

	private:
		double d_constant_value;
	};//class CConstantInertiaWeight : public CInertiaWeight


	class CRandomInertiaWeight : public CInertiaWeight
	{
	public:
		virtual double dGetValue(double dNow, double dMax) { return 0.5 + (double)RandUtils::fRandProbability() / 2.0; }
	};//class CRandomInertiaWeight : public CInertiaWeight


	class CLogarithmDecreasingInertiaWeight : public CInertiaWeight
	{
	public:
		virtual CError eConfigure(istream *psSettings);

		virtual double dGetValue(double dNow, double dMax);

	private:
		double d_min_value;
		double d_max_value;

		double d_a;
	};//class CLogarithmDecreasingInertiaWeight : public CInertiaWeight


	CInertiaWeight *pcGetInertiaWeight(istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace InertiaWeight

#endif//INERTIA_WEIGHT_T