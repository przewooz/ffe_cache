#ifndef CHANGE_DETECTOR_H
#define CHANGE_DETECTOR_H

#define CHANGE_DETECTOR_ARGUMENT_TYPE "change_detector_type"
#define CHANGE_DETECTOR_ARGUMENT_TYPE_SENSOR "sensor"
#define CHANGE_DETECTOR_ARGUMENT_TYPE_INTEGRAL_SENSOR "integral_sensor"
#define CHANGE_DETECTOR_ARGUMENT_TYPE_DETERMINISTIC_OPTIMIZER "deterministic_optimizer"

#define CHANGE_DETECTOR_ARGUMENT_NUMBER_OF_STATES "number_of_states"

#include "Error.h"
#include "Log.h"
#include "Problem.h"
#include "StopCondition.h"

#include <cstdint>
#include <istream>

using namespace std;


namespace ChangeDetector
{
	enum EChangeDetectorType
	{
		CHANGE_DETECTOR_SENSOR = 0,
		CHANGE_DETECTOR_INTEGRAL_SENSOR = 1,
		CHANGE_DETECTOR_DETERMINISTIC_OPTIMIZER = 2
	};//enum EChangeDetectorType


	template <class TGenotype, class TFenotype>
	class CChangeDetector
	{
	public:
		CChangeDetector(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);

		virtual ~CChangeDetector();

		virtual CError eConfigure(istream *psSettings);

		virtual void vInitialize();
		virtual void vRun();

		uint16_t iGetTruePositiveCount() { return i_true_positive_counter; };
		uint16_t iGetFalsePositiveCount() { return i_false_positive_counter; };

		uint16_t iGetTrueNegativeCount() { return i_true_negative_counter; };
		uint16_t iGetFalseNegativeCount() { return i_false_negative_counter; };

	protected:
		virtual bool b_detect_change() = 0;
		
		void v_clear_params();
		void v_clear_counters();

		CProblem<TGenotype, TFenotype> *pc_problem;

		CStopCondition<TGenotype, TFenotype> *pc_stop_condition;

		uint16_t i_number_of_states;

		uint16_t i_true_positive_counter;
		uint16_t i_false_positive_counter;

		uint16_t i_true_negative_counter;
		uint16_t i_false_negative_counter;

		CLog *pc_log;

		uint32_t i_random_seed;
	};//class CChangeDetector

	template <class TGenotype, class TFenotype>
	CChangeDetector<TGenotype, TFenotype> *pcGetChangeDetector(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, 
		uint32_t iRandomSeed, istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace ChangeDetector

#endif//CHANGE_DETECTOR_H