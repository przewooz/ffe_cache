#include "DummyOptimizer.h"

#include "RandUtils.h"
#include "UIntCommandParam.h"

#include <atlstr.h>


uint32_t CDummyOptimizer::iERROR_CDummyOptimizer = CError::iADD_ERROR_PARENT("iERROR_CDummyOptimizer");



CDummyOptimizer::CDummyOptimizer(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CBinaryOptimizer(pcProblem, pcLog, iRandomSeed)
{

}//CDummyOptimizer::CDummyOptimizer(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

CDummyOptimizer::CDummyOptimizer(CDummyOptimizer *pcOther)
	: CBinaryOptimizer(pcOther)
{
	t_log_frequency = pcOther->t_log_frequency;
}//CDummyOptimizer::CDummyOptimizer(CDummyOptimizer *pcOther)

CError CDummyOptimizer::eConfigure(istream *psSettings)
{
	CError c_error = CBinaryOptimizer::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_log_frequency(DUMMY_OPTIMIZER_ARGUMENT_LOG_FREQUENCY);
		t_log_frequency = (time_t)p_log_frequency.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CDummyOptimizer::eConfigure(istream *psSettings)

void CDummyOptimizer::vInitialize(time_t tStartTime)
{
	CBinaryOptimizer::vInitialize(tStartTime);

	//jakies rzeczy robione tylko raz na poczatku wywolania metody
}//void CDummyOptimizer::vInitialize(time_t tStartTime)

bool CDummyOptimizer::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	uint16_t i_number_of_bits = pc_problem->pcGetEvaluation()->iGetNumberOfElements();

	//osobnik w Twojej konwencji
	//int *pi_bits = nullptr;

	//wyliczenie fitnessu
	//double d_fitness_value = d_compute_fitness_value(pi_bits);

	//bool b_updated = false;

	//aktualizacja najlepszego znalezionego osobnika - w przypadku gdy metoda produkuje tylko jednego osobnika podczas pojedynczej iteracji, 
	//metoda b_update_best_individual zostanie wywolana tylko raz z bitami i fitnessem tego konkretnego osobnika
	//w ponizszym przykladzie for jest tylko po to by pokazac, ze metode b_update_best_individual trzeba wywolac dla kazdego "nowego" osobnika
	//for (int i = 0; i < 10; i++)
	//{
	//	b_updated = b_update_best_individual(iIterationNumber, tStartTime, pi_bits, d_fitness_value) || b_updated;
	//}//for (int i = 0; i < 10; i++)

	//bity najlepszego znalezionego do teraz osobnika
	//pc_best_individual->pcGetGenotype()->piGetBits();

	int *pi_bits = new int[i_number_of_bits];

	for (uint16_t i = 0; i < i_number_of_bits; i++)
	{
		//wywolanie utilsow do liczb pseudolosowych (metoda powinna korzystac jedynie z RandUtils do tego typu rzeczy 
		//- w przeciwnym wypadku podany jako parametr seed nie bedzie uwzgledniony)
		*(pi_bits + i) = RandUtils::iRandNumber(0, 1);
	}//for (uint16_t i = 0; i < i_number_of_bits; i++)

	double d_fitness_value = d_compute_fitness_value(pi_bits);
	bool b_updated = b_update_best_individual(iIterationNumber, tStartTime, pi_bits, d_fitness_value);

	if ((time(nullptr) - tStartTime) % t_log_frequency == 0)
	{
		CString s_log_message;
		s_log_message.Format("best fitness: %f; best unitation: %f; ffe: %u; time: %u; iteration: %u", pc_best_individual->dGetFitnessValue(),
			pc_best_individual->pcGetGenotype()->dGetUnitation(), pc_problem->pcGetEvaluation()->iGetFFE(), (uint32_t)(time(nullptr) - tStartTime),
			iIterationNumber);

		pc_log->vPrintLine(s_log_message, true);
	}//if ((time(nullptr) - tStartTime) % t_log_frequency == 0)

	return b_updated;
}//bool CDummyOptimizer::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)