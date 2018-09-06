#include "OptimizationCommand.h"

#include "DynamicOptimizer.h"
#include "EnumCommandParam.h"
#include "Evaluation.h"
#include "EvaluationUtils.h"
#include "Optimizer.h"
#include "OptimizerUtils.h"
#include "Problem.h"
#include "StringUtils.h"
#include "System.h"

#include <unordered_map>
#include <utility>

using namespace DynamicOptimizer;

COptimizationBasedCommand::COptimizationBasedCommand(CString sName, istream *psSettings, CLog *pcLog, ostream *psSummary)
	: CCommandExecution(sName, psSettings, pcLog, psSummary)
{

}//COptimizationBasedCommand::COptimizationBasedCommand(CString sName, istream *psSettings, CLog *pcLog, ostream *psSummary)

CError COptimizationBasedCommand::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)
{
	CError c_error;

	unordered_map<CString, EProblemType> m_problem_types;
	m_problem_types.insert(pair<const CString, EProblemType>(PROBLEM_ARGUMENT_TYPE_BINARY_BINARY, PROBLEM_BINARY_BINARY));
	m_problem_types.insert(pair<const CString, EProblemType>(PROBLEM_ARGUMENT_TYPE_REAL_REAL, PROBLEM_REAL_REAL));

	CEnumCommandParam<EProblemType> p_problem_type(PROBLEM_ARGUMENT_TYPE, &m_problem_types);
	EProblemType e_problem_type = p_problem_type.eGetValue(ps_settings, &c_error);

	if (!c_error)
	{
		switch (e_problem_type)
		{
			case PROBLEM_BINARY_BINARY:
			{
				c_error = e_optimize_binary_binary(iRandomSeed);
				break;
			}//case PROBLEM_BINARY_BINARY
			case PROBLEM_REAL_REAL:
			{
				c_error = e_optimize_real_real(iRandomSeed);
				break;
			}//PROBLEM_REAL_REAL
			default:
			{
				c_error.vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "problem");
				break;
			}//default
		}//switch (e_problem_type)
	}//if (!c_error)

	return c_error;
}//CError COptimizationBasedCommand::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)


COptimizationCommand::COptimizationCommand(istream *psSettings, CLog *pcLog, ostream *psSummary)
	: COptimizationBasedCommand(SYSTEM_ARGUMENT_MODE_OPTIMIZATION, psSettings, pcLog, psSummary)
{

}//COptimizationCommand::COptimizationCommand(istream *psSettings, CLog *pcLog, ostream *psSummary)

template <class TGenotype, class TFenotype>
CError COptimizationCommand::e_optimize(uint32_t iRandomSeed)
{
	CError c_error;

	CEvaluation<TFenotype> *pc_evaluation = EvaluationUtils::pcGetEvaluation<TFenotype>(ps_settings, &c_error);

	if (!c_error)
	{
		CProblem<TGenotype, TFenotype> *pc_problem = new CProblem<TGenotype, TFenotype>(pc_evaluation, new CUniformTransformation<TGenotype>());
		COptimizer<TGenotype, TFenotype> *pc_optimizer = OptimizerUtils::pcGetOptimizer(pc_problem, pc_log, iRandomSeed, ps_settings, &c_error);

		if (!c_error)
		{
			pc_optimizer->vRun();

			CString s_summary;

			
			s_summary.Format("%f\t%f\t%llu\t%u\tFFEall:\t%llu\tCacheFit:\t%llu\tCachePop:\t%llu\tNoFFEiter:\t %d\t AllIter:\t%d", pc_optimizer->pcGetBestIndividual()->dGetFitnessValue(),
				pc_problem->pcGetEvaluation()->dGetMaxValue(), pc_optimizer->iGetBestFFE(),
				(uint32_t)pc_optimizer->tGetBestTime(), pc_problem->pcGetEvaluation()->iGetFFE(), pc_optimizer->iGetCachingFitness(), pc_optimizer->iGetCachingPopulation(), pc_optimizer->iGetNoFFEIterations(), pc_optimizer->iGetAllIterations());



			
			if (typeid(TFenotype).hash_code() == typeid(CBinaryCoding).hash_code())
			{
				s_summary.AppendFormat("\t%f", ((CBinaryCoding*)pc_optimizer->pcGetBestIndividual()->pcGetFenotype())->dGetUnitation());
			}//if (typeid(TFenotype).hash_code() == typeid(CBinaryCoding).hash_code())

			s_summary += pc_optimizer->sAdditionalSummaryInfo();

			(*ps_summary) << s_summary;
			(*ps_summary) << "\t" << pc_optimizer->pcGetBestIndividual()->pcGetFenotype();
		}//if (!c_error)

		delete pc_optimizer;
		delete pc_problem;
	}//if (!c_error)
	else
	{
		delete pc_evaluation;
	}//else if (!c_error)

	return c_error;
}//CError COptimizationCommand::e_optimize(uint32_t iRandomSeed)


CDynamicOptimizationCommand::CDynamicOptimizationCommand(istream *psSettings, CLog *pcLog, ostream *psSummary)
	: COptimizationBasedCommand(SYSTEM_ARGUMENT_MODE_DYNAMIC_OPTIMIZATION, psSettings, pcLog, psSummary)
{

}//CDynamicOptimizationCommand::CDynamicOptimizationCommand(istream *psSettings, CLog *pcLog, ostream *psSummary)

template <class TGenotype, class TFenotype>
CError CDynamicOptimizationCommand::e_optimize(uint32_t iRandomSeed)
{
	CError c_error;

	CDynamicEvaluation<TFenotype> *pc_dynamic_evaluation = EvaluationUtils::pcGetDynamicEvaluation<TFenotype>(ps_settings, &c_error);

	if (!c_error)
	{
		CProblem<TGenotype, TFenotype> *pc_problem = new CProblem<TGenotype, TFenotype>(pc_dynamic_evaluation, new CUniformTransformation<TGenotype>());
		CDynamicOptimizer<TGenotype, TFenotype> *pc_dynamic_optimizer = pcGetDynamicOptimizer(pc_problem, pc_log, iRandomSeed, ps_settings, &c_error);

		if (!c_error)
		{
			pc_dynamic_optimizer->vRun();

			CString s_summary;

			CIndividual<TGenotype, TFenotype> *pc_best_individual;

			uint16_t i_evaluation_index;

			for (uint16_t i = 0; i < pc_dynamic_optimizer->iGetNumberOfStates(); i++)
			{
				i_evaluation_index = i % pc_dynamic_evaluation->iGetNumberOfStates();

				pc_best_individual = *(pc_dynamic_optimizer->ppcGetBestIndividuals() + i);

				s_summary.Format("%f\t%f\t%llu\t%u\t", pc_best_individual->dGetFitnessValue(),
					*(pc_dynamic_evaluation->pdGetMaxValues() + i_evaluation_index), *(pc_dynamic_optimizer->piGetBestFFEs() + i),
					(uint32_t)*(pc_dynamic_optimizer->ptGetBestTimes() + i));

				if (typeid(TFenotype).hash_code() == typeid(CBinaryCoding).hash_code())
				{
					s_summary.AppendFormat("\t%f", ((CBinaryCoding*)pc_best_individual->pcGetFenotype())->dGetUnitation());
				}//if (typeid(TFenotype).hash_code() == typeid(CBinaryCoding).hash_code())

				(*ps_summary) << s_summary;
				(*ps_summary) << "\t" << pc_best_individual->pcGetFenotype();

				if (i < pc_dynamic_optimizer->iGetNumberOfStates() - 1)
				{
					(*ps_summary) << "\t";
				}//if (i < pc_dynamic_optimizer->iGetNumberOfStates() - 1)
			}//for (uint16_t i = 0; i < pc_dynamic_optimizer->iGetNumberOfStates(); i++)
		}//if (!c_error)

		delete pc_dynamic_optimizer;
		delete pc_problem;
	}//if (!c_error)
	else
	{
		delete pc_dynamic_evaluation;
	}//else if (!c_error)

	return c_error;
}//CError CDynamicOptimizationCommand::e_optimize(uint32_t iRandomSeed)