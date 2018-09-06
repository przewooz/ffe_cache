#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cfloat>
#include <cstdint>
#include <functional>

using namespace std;

namespace MathUtils
{
	double dComputeAngle(double *pdValues0, double *pdValues1, uint32_t iLength);
	double dComputeDotProduct(double *pdValues0, double *pdValues1, uint32_t iLength);
	double dComputeSecondNorm(double *pdValues, uint32_t iLength);
	double dComputeSquareDistance(double *pdValues0, double *pdValues1, uint32_t iLength, double dMaxValue = DBL_MAX);
	
	double dMaxValue(double *pdValues, uint32_t iLength);
	double dSum(double *pdValues, uint32_t iLength);

	double dComputeMonteCarloIntegral(function<double(double*)> fFunction, double dMaxFunctionValue, double *pdMinValues, 
		double *pdMaxValues, uint8_t iNumberOfDimensions, uint32_t iNumberOfSamples);

	void vNormalize(double *pcValues, uint32_t iLength);
}//namespace MathUtils

#endif//MATH_UTILS_H