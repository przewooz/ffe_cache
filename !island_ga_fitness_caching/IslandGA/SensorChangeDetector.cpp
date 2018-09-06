#include "SensorChangeDetector.h"

#include "BinaryCoding.h"
#include "GenerationUtils.h"
#include "RealCoding.h"
#include "UIntCommandParam.h"

using namespace ChangeDetector;

template <class TGenotype, class TFenotype>
CSensorChangeDetector<TGenotype, TFenotype>::CSensorChangeDetector(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CChangeDetector<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed)
{
	pc_generation = nullptr;
	ppc_sensors = nullptr;
}//CSensorChangeDetector<TGenotype, TFenotype>::CSensorChangeDetector(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

template <class TGenotype, class TFenotype>
CSensorChangeDetector<TGenotype, TFenotype>::~CSensorChangeDetector()
{
	v_clear_params();
}//CSensorChangeDetector<TGenotype, TFenotype>::~CSensorChangeDetector()

template <class TGenotype, class TFenotype>
CError CSensorChangeDetector<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	v_clear_params();

	CError c_error = CChangeDetector<TGenotype, TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		pc_generation = GenerationUtils::pcGetGeneration(pc_problem, nullptr, psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_number_of_sensors(SENSOR_CHANGE_DETECTOR_ARGUMENT_NUMBER_OF_SENSORS, 1, UINT32_MAX);
		i_number_of_sensors = p_number_of_sensors.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CSensorChangeDetector<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
void CSensorChangeDetector<TGenotype, TFenotype>::vInitialize()
{
	CChangeDetector<TGenotype, TFenotype>::vInitialize();

	v_clear_sensors();

	ppc_sensors = new CIndividual<TGenotype, TFenotype>*[i_number_of_sensors];

	CIndividual<TGenotype, TFenotype> *pc_single_sensor;

	for (uint32_t i = 0; i < i_number_of_sensors; i++)
	{
		pc_single_sensor = new CIndividual<TGenotype, TFenotype>(pc_generation->pcGenerate(), pc_problem);
		pc_single_sensor->vEvaluate();

		*(ppc_sensors + i) = pc_single_sensor;
	}//for (uint32_t i = 0; i < i_number_of_sensors; i++)
}//void CSensorChangeDetector<TGenotype, TFenotype>::vInitialize()

template <class TGenotype, class TFenotype>
bool CSensorChangeDetector<TGenotype, TFenotype>::b_detect_change()
{
	bool b_change_detected = false;

	CIndividual<TGenotype, TFenotype> *pc_sensor;

	double d_previous_fitness;

	for (uint32_t i = 0; i < i_number_of_sensors; i++)
	{
		pc_sensor = *(ppc_sensors + i);
		d_previous_fitness = pc_sensor->dGetFitnessValue();

		pc_sensor->vIsEvaluated(false);
		pc_sensor->vEvaluate();

		b_change_detected |= d_previous_fitness != pc_sensor->dGetFitnessValue();
	}//for (uint32_t i = 0; i < i_number_of_sensors; i++)

	return b_change_detected;
}//bool CSensorChangeDetector<TGenotype, TFenotype>::b_detect_change()

template <class TGenotype, class TFenotype>
void CSensorChangeDetector<TGenotype, TFenotype>::v_clear_params()
{
	delete pc_generation;
	pc_generation = nullptr;

	v_clear_sensors();
}//void CSensorChangeDetector<TGenotype, TFenotype>::v_clear_params()

template <class TGenotype, class TFenotype>
void CSensorChangeDetector<TGenotype, TFenotype>::v_clear_sensors()
{
	if (ppc_sensors)
	{
		for (uint32_t i = 0; i < i_number_of_sensors; i++)
		{
			delete *(ppc_sensors + i);
		}//for (uint32_t i = 0; i < i_number_of_sensors; i++)

		delete ppc_sensors;
	}//if (ppc_sensors)

	ppc_sensors = nullptr;
}//void CSensorChangeDetector<TGenotype, TFenotype>::v_clear_sensors()


template class CSensorChangeDetector<CBinaryCoding, CBinaryCoding>;
template class CSensorChangeDetector<CRealCoding, CRealCoding>;