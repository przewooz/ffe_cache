#include "ChangeDetector.h"

#include "BinaryCoding.h"
#include "EnumCommandParam.h"
#include "Evaluation.h"
#include "RealCoding.h"
#include "SensorChangeDetector.h"
#include "StopConditionUtils.h"
#include "StringUtils.h"
#include "UIntCommandParam.h"

#include <ctime>
#include <unordered_map>

using namespace ChangeDetector;


template <class TGenotype, class TFenotype>
CChangeDetector<TGenotype, TFenotype>::CChangeDetector(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
{
	pc_problem = pcProblem;
	pc_log = pcLog;
	i_random_seed = iRandomSeed;

	pc_stop_condition = nullptr;

	v_clear_counters();
}//CChangeDetector<TGenotype, TFenotype>::CChangeDetector(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

template <class TGenotype, class TFenotype>
CChangeDetector<TGenotype, TFenotype>::~CChangeDetector()
{
	v_clear_params();
}//CChangeDetector<TGenotype, TFenotype>::~CChangeDetector()

template <class TGenotype, class TFenotype>
CError CChangeDetector<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	CError c_error;

	v_clear_params();

	CUIntCommandParam p_number_of_states(CHANGE_DETECTOR_ARGUMENT_NUMBER_OF_STATES, 1, UINT16_MAX);
	i_number_of_states = p_number_of_states.iGetValue(psSettings, &c_error);

	if (!c_error)
	{
		pc_stop_condition = StopConditionUtils::pcGetStopCondition<TGenotype, TFenotype>(pc_problem->pcGetEvaluation(), psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CChangeDetector<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
void CChangeDetector<TGenotype, TFenotype>::vInitialize()
{
	v_clear_counters();
}//void CChangeDetector<TGenotype, TFenotype>::vInitialize()

template <class TGenotype, class TFenotype>
void CChangeDetector<TGenotype, TFenotype>::vRun()
{
	vInitialize();

	CDynamicEvaluation<TFenotype> *pc_dynamic_evaluation = (CDynamicEvaluation<TFenotype>*)pc_problem->pcGetEvaluation();

	time_t t_start_time;
	uint32_t i_iteration_number;

	bool b_change_detected;

	for (uint16_t i = 0; i < i_number_of_states; i++)
	{
		t_start_time = time(nullptr);
		i_iteration_number = 0;

		while (!pc_stop_condition->bStop(t_start_time, i_iteration_number, 0, nullptr))
		{
			b_change_detected = b_detect_change();

			if (i_iteration_number == 0 && i > 0)
			{
				if (b_change_detected)
				{
					i_true_positive_counter++;
				}//if (b_change_detected)
				else
				{
					i_false_negative_counter++;
				}//else if (b_change_detected)
			}//if (i_iteration_number == 0 && i > 0)
			else
			{
				if (b_change_detected)
				{
					i_false_positive_counter++;
				}//if (b_change_detected)
				else
				{
					i_true_negative_counter++;
				}//else if (b_change_detected)
			}//else if (i_iteration_number == 0 && i > 0)

			i_iteration_number++;
		}//while (!pc_stop_condition->bStop(t_start_time, i_iteration_number, 0, nullptr))

		if (i < i_number_of_states - 1)
		{
			pc_dynamic_evaluation->vChange();
		}//if (i < i_number_of_states - 1)
	}//for (uint16_t i = 0; i < i_number_of_states; i++)
}//void CChangeDetector<TGenotype, TFenotype>::vRun()

template <class TGenotype, class TFenotype>
void CChangeDetector<TGenotype, TFenotype>::v_clear_params()
{
	delete pc_stop_condition;
	pc_stop_condition = nullptr;

	v_clear_counters();
}//void CChangeDetector<TGenotype, TFenotype>::v_clear_params()

template <class TGenotype, class TFenotype>
void CChangeDetector<TGenotype, TFenotype>::v_clear_counters()
{
	i_true_positive_counter = 0;
	i_false_positive_counter = 0;

	i_true_negative_counter = 0;
	i_false_negative_counter = 0;
}//void CChangeDetector<TGenotype, TFenotype>::v_clear_counters()


template <class TGenotype, class TFenotype>
CChangeDetector<TGenotype, TFenotype>* ChangeDetector::pcGetChangeDetector(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CChangeDetector<TGenotype, TFenotype> *pc_change_detector = nullptr;

	size_t i_genotype_type_hash_code = typeid(TGenotype).hash_code();
	size_t i_fenotype_type_hash_code = typeid(TFenotype).hash_code();

	unordered_map<CString, EChangeDetectorType> m_change_detector_types;

	m_change_detector_types.insert(pair<const CString, EChangeDetectorType>(CHANGE_DETECTOR_ARGUMENT_TYPE_SENSOR, CHANGE_DETECTOR_SENSOR));

	if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code() && i_fenotype_type_hash_code == typeid(CRealCoding).hash_code())
	{
		m_change_detector_types.insert(pair<const CString, EChangeDetectorType>(CHANGE_DETECTOR_ARGUMENT_TYPE_INTEGRAL_SENSOR, CHANGE_DETECTOR_INTEGRAL_SENSOR));
		m_change_detector_types.insert(pair<const CString, EChangeDetectorType>(CHANGE_DETECTOR_ARGUMENT_TYPE_DETERMINISTIC_OPTIMIZER, CHANGE_DETECTOR_DETERMINISTIC_OPTIMIZER));
	}//if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code() && i_fenotype_type_hash_code == typeid(CRealCoding).hash_code())

	CEnumCommandParam<EChangeDetectorType> p_type(CHANGE_DETECTOR_ARGUMENT_TYPE, &m_change_detector_types, bIsObligatory);
	EChangeDetectorType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case CHANGE_DETECTOR_SENSOR:
			{
				pc_change_detector = new CSensorChangeDetector<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed);
				break;
			}//case CHANGE_DETECTOR_SENSOR
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "change detector");
				break;
			}//default
		}//switch (e_type)
	}//if (!*pcError && p_type.bHasValue())

	if (!*pcError)
	{
		*pcError = pc_change_detector->eConfigure(psSettings);
	}//if (!*pcError)

	return pc_change_detector;
}//CChangeDetector<TGenotype, TFenotype>* ChangeDetector::pcGetChangeDetector(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLogu, int32_t iRandomSeed, istream *psSettings, CError *pcError, bool bIsObligatory)


template class CChangeDetector<CBinaryCoding, CBinaryCoding>;
template class CChangeDetector<CRealCoding, CRealCoding>;


template CChangeDetector<CBinaryCoding, CBinaryCoding> * ChangeDetector::pcGetChangeDetector(CProblem<CBinaryCoding, CBinaryCoding>*, CLog*, uint32_t, istream*, CError*, bool);
template CChangeDetector<CRealCoding, CRealCoding> * ChangeDetector::pcGetChangeDetector(CProblem<CRealCoding, CRealCoding>*, CLog*, uint32_t, istream*, CError*, bool);