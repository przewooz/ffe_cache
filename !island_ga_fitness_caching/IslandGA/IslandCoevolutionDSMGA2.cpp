#include "IslandCoevolutionDSMGA2.h"

#include "BinaryEvaluation.h"
#include "FloatCommandParam.h"
#include "Transformation.h"
#include "UIntCommandParam.h"

CIslandCoevolutionDSMGA2::CIslandCoevolutionDSMGA2(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)
	: CMultiOptimizer<CBinaryCoding, CBinaryCoding>(pcProblem, pcLog, iRandomSeed)
{
	pc_cc_dsmga2 = nullptr;
}//CIslandCoevolutionDSMGA2::CIslandCoevolutionDSMGA2(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog, uint32_t iRandomSeed)

CIslandCoevolutionDSMGA2::CIslandCoevolutionDSMGA2(CIslandCoevolutionDSMGA2 *pcOther)
	: CMultiOptimizer<CBinaryCoding, CBinaryCoding>(pcOther)
{
	i_cc_frequency = pcOther->i_cc_frequency;
	f_cc_range = pcOther->f_cc_range;

	pc_cc_dsmga2 = pcOther->pc_cc_dsmga2;
}//CIslandCoevolutionDSMGA2::CIslandCoevolutionDSMGA2(CIslandCoevolutionDSMGA2 *pcOther)

CIslandCoevolutionDSMGA2::~CIslandCoevolutionDSMGA2()
{
	v_clear_params();
}//CIslandCoevolutionDSMGA2::~CIslandCoevolutionDSMGA2()

CError CIslandCoevolutionDSMGA2::eConfigure(istream *psSettings)
{
	v_clear_params();

	CError c_error = CMultiOptimizer<CBinaryCoding, CBinaryCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_cc_frequency("cc_frequency");
		i_cc_frequency = p_cc_frequency.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CFloatCommandParam p_cc_range("cc_range", 0, 1);
		f_cc_range = p_cc_range.fGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CEvaluation<CBinaryCoding> *pc_cc_evaluation = nullptr;
		pc_cc_evaluation = new CBinaryCooperativeCoevolutionEvaluation((CEvaluation<CBinaryCoding>*)pc_problem->pcGetEvaluation());

		CProblem<CBinaryCoding, CBinaryCoding> *pc_cc_problem;
		pc_cc_problem = new CProblem<CBinaryCoding, CBinaryCoding>(pc_cc_evaluation, new CUniformTransformation<CBinaryCoding>());

		pc_cc_dsmga2 = new CDSMGA2(pc_cc_problem, pc_log, i_random_seed);

		c_error = pc_cc_dsmga2->eConfigure(psSettings);
	}//if (!c_error)

	if (!c_error)
	{
		CDSMGA2::vSetInjectedEvaluation(pc_problem->pcGetEvaluation());
	}//if (!c_error)

	return c_error;
}//CError CIslandCoevolutionDSMGA2::eConfigure(istream *psSettings)

bool CIslandCoevolutionDSMGA2::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	bool b_updated = CMultiOptimizer<CBinaryCoding, CBinaryCoding>::bRunIteration(iIterationNumber, tStartTime);

	b_updated = b_run_cc(iIterationNumber) || b_updated;

	return b_updated;
}//bool CIslandCoevolutionDSMGA2::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

CError CIslandCoevolutionDSMGA2::e_configure_params_optimizer(istream *psSettings)
{
	pc_params_optimizer = new CDSMGA2(pc_problem, pc_log, i_random_seed);
	return pc_params_optimizer->eConfigure(psSettings);
}//CError CIslandCoevolutionDSMGA2::e_configure_params_optimizer(istream *psSettings)

void CIslandCoevolutionDSMGA2::v_clear_params()
{
	if (b_own_params)
	{
		if (pc_cc_dsmga2)
		{
			delete pc_cc_dsmga2->pcGetProblem();
		}//if (pc_cc_dsmga2)

		delete pc_cc_dsmga2;
		pc_cc_dsmga2 = nullptr;
	}//if (b_own_params)
}//void CIslandCoevolutionDSMGA2::v_clear_params()

bool CIslandCoevolutionDSMGA2::b_run_cc(uint32_t iIterationNumber)
{
	bool b_updated = false;

	if (iIterationNumber > 0 && iIterationNumber % i_cc_frequency == 0)
	{
		uint16_t i_number_of_elements = pc_problem->pcGetEvaluation()->iGetNumberOfElements();
		uint8_t i_number_of_populations = (uint8_t)v_populations.size();

		CBinaryCooperativeCoevolutionEvaluation *pc_cc_evaluation = (CBinaryCooperativeCoevolutionEvaluation*)pc_cc_dsmga2->pcGetProblem()->pcGetEvaluation();

		pc_cc_evaluation->vClearGenesIndexes();

		CBinaryCoding *pc_cc_complementary_fenotype = new CBinaryCoding(i_number_of_elements);

		uint32_t i_value;

		bool b_is_difference;

		CString s_log;

		for (uint16_t i = 0; i < i_number_of_elements; i++)
		{
			i_value = *(v_populations.at(0)->pcGetBestIndividual()->pcGetFenotype()->piGetBits() + i);

			b_is_difference = false;

			for (uint8_t j = 1; j < i_number_of_populations && !b_is_difference; j++)
			{
				if (*(v_populations.at(j)->pcGetBestIndividual()->pcGetFenotype()->piGetBits() + i) != i_value)
				{
					s_log.Format("%d", i);
					//pc_log->vPrintLine(s_log, true);

					pc_cc_evaluation->vAddGeneIndex(i);
					b_is_difference = true;
				}//if (*(v_populations.at(j)->pcGetBestIndividual()->pcGetFenotype()->piGetBits() + i) != i_value)
			}//for (uint8_t j = 1; j < i_number_of_populations && !b_is_difference; j++)

			if (!b_is_difference)
			{
				*(pc_cc_complementary_fenotype->piGetBits() + i) = i_value;
			}//if (!b_is_difference)
		}//for (uint16_t i = 0; i < i_number_of_elements; i++)

		if (pc_cc_evaluation->iGetNumberOfElements() > 0)
		{
			s_log.Format("number of elements: %d", pc_cc_evaluation->iGetNumberOfElements());
			pc_log->vPrintLine(s_log, true);

			pc_cc_evaluation->vSetComplementaryFenotype(pc_cc_complementary_fenotype);

			CDSMGA2::vSetInjectedEvaluation(pc_cc_evaluation);

			pc_cc_dsmga2->vRun();

			if (pc_problem->bIsBetterIndividual(pc_cc_dsmga2->pcGetBestIndividual(), pc_best_individual))
			{
				vSetBestIndividual(pc_cc_dsmga2->pcGetBestIndividual(), true);
				b_updated = true;
			}//if (pc_problem->bIsBetterIndividual(pc_cc_dsmga2->pcGetBestIndividual(), pc_best_individual))

			CDSMGA2::vSetInjectedEvaluation(pc_problem->pcGetEvaluation());
		}//if (pc_cc_evaluation->iGetNumberOfElements() > 0)

		delete pc_cc_complementary_fenotype;
	}//if (iIterationNumber > 0 && iIterationNumber % i_cc_frequency == 0)

	return b_updated;
}//bool CIslandCoevolutionDSMGA2::b_run_cc(uint32_t iIterationNumber)