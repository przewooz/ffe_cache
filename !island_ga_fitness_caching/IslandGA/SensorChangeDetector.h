#ifndef SENSOR_CHANGE_DETECTOR_H
#define SENSOR_CHANGE_DETECTOR_H

#define SENSOR_CHANGE_DETECTOR_ARGUMENT_NUMBER_OF_SENSORS "number_of_sensors"

#include "ChangeDetector.h"
#include "Error.h"
#include "Generation.h"
#include "Individual.h"
#include "Log.h"
#include "Problem.h"

#include <cstdint>
#include <istream>

namespace ChangeDetector
{
	template <class TGenotype, class TFenotype>
	class CSensorChangeDetector : public CChangeDetector<TGenotype, TFenotype>
	{
	public:
		CSensorChangeDetector(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed);

		virtual ~CSensorChangeDetector();

		virtual CError eConfigure(istream *psSettings);

		virtual void vInitialize();
		
	protected:
		virtual bool b_detect_change();

		void v_clear_params();
		void v_clear_sensors();

		CGeneration<TGenotype> *pc_generation;

		uint32_t i_number_of_sensors;

		CIndividual<TGenotype, TFenotype> **ppc_sensors;
	};//class CSensorChangeDetector : public CChangeDetector<TGenotype, TFenotype>
}//namespace ChangeDetector

#endif//SENSOR_CHANGE_DETECTOR_H