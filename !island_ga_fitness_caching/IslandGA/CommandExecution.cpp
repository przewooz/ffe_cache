#include "CommandExecution.h"

#include "BinaryCoding.h"
#include "Cluster.h"
#include "ChangeDetector.h"
#include "CompProblem.h"
#include "DifferentialEvolution.h"
#include "EnumCommandParam.h"
#include "Evaluation.h"
#include "EvaluationUtils.h"
#include "FloatCommandParam.h"
#include "KMeansClustering.h"
#include "MathUtils.h"
#include "ParticleSwarmOptimization.h"
#include "Problem.h"
#include "RandUtils.h"
#include "RealCoding.h"
#include "RealRandomGeneration.h"
#include "StringCommandParam.h"
#include "StringUtils.h"
#include "System.h"
#include "Transformation.h"
#include "UIntCommandParam.h"

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

using namespace ChangeDetector;
using namespace Clustering;


CCommandExecution::CCommandExecution(CString sName, istream *psSettings, CLog *pcLog, ostream *psSummary)
{
	s_name = sName;
	ps_settings = psSettings;
	pc_log = pcLog;
	ps_summary = psSummary;
}//CCommandExecution::CCommandExecution(CString sName, CLog *pcLog, ostream *psSummary)

CCommandExecution::~CCommandExecution()
{

}//CCommandExecution::~CCommandExecution()

#include "BinaryEvaluation.h"

CError CReportDeceptive::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)
{
	CError c_error;

	CBinaryDeceptiveConcatenationEvaluation c_deceptive;
	c_error = c_deceptive.eConfigure(ps_settings);

	if (!c_error)
	{
		CStringCommandParam p_report("report_file_path");
		CString s_report = p_report.sGetValue(ps_settings, &c_error);

		if (!c_error)
		{
			FILE *pf_report = fopen(s_report, "wb");

			c_error = c_deceptive.eReport(pf_report);

			fclose(pf_report);
		}
	}

	return c_error;
}

#include "BinaryGeneration.h"
#include <vector>

CError CRandomSet::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)
{
	CError c_error;

	CEvaluation<CBinaryCoding> *pc_evaluation = EvaluationUtils::pcGetEvaluation<CBinaryCoding>(ps_settings, &c_error);

	if (!c_error)
	{
		CBinaryRandomGeneration c_generator(pc_evaluation->iGetNumberOfElements());
		c_error = c_generator.eConfigure(nullptr, ps_settings);

		if (!c_error)
		{
			CUIntCommandParam p_number_of_samples("number_of_samples");
			uint32_t i_number_of_samples = p_number_of_samples.iGetValue(ps_settings, &c_error);

			if (!c_error)
			{
				vector<CBinaryCoding*> v_uniq_genotypes;
				v_uniq_genotypes.reserve(i_number_of_samples);

				CBinaryCoding *pc_sample;

				for (uint32_t i = 0; i < i_number_of_samples; i++)
				{
					bool b_done = false;

					do
					{
						pc_sample = c_generator.pcGenerate();

						bool b_was = false;

						for (size_t j = 0; j < v_uniq_genotypes.size() && !b_was; j++)
						{
							b_was = pc_sample == v_uniq_genotypes.at(j);
						}

						b_done = !b_was;

						if (!b_done)
						{
							delete pc_sample;
						}
					} while (!b_done);

					CIndividual<CBinaryCoding, CBinaryCoding> c_individual(pc_sample, pc_evaluation, nullptr, nullptr, new CUniformTransformation<CBinaryCoding>());
					c_individual.vEvaluate();

					CString s_message(pc_sample->sToString());
					s_message.AppendFormat("\t%f", c_individual.dGetFitnessValue());

					pc_log->vPrintLine(s_message);
				}
			}
		}
	}

	delete pc_evaluation;

	return c_error;
}

CAvisSubsetSumKnapsackProblemGenerator::CAvisSubsetSumKnapsackProblemGenerator(CLog *pcLog, ostream *psSummary)
	: CCommandExecution(SYSTEM_ARGUMENT_MODE_AVIS_SUBSET_SUM_KNAPSACK_PROBLEM_GENERATOR, nullptr, pcLog, psSummary)
{

}//CAvisSubsetSumKnapsackProblemGenerator::CAvisSubsetSumKnapsackProblemGenerator(CLog *pcLog, ostream *psSummary)

CError CAvisSubsetSumKnapsackProblemGenerator::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)
{
	CError c_error;

	CUIntCommandParam c_number_of_variables(SYSTEM_ARGUMENT_NUMBER_OF_VARIABLES, 1, UINT32_MAX);
	CStringCommandParam c_output_file_path(SYSTEM_ARGUMENT_OUTPUT_FILE_PATH);

	pcParser->vRegister(&c_number_of_variables);
	pcParser->vRegister(&c_output_file_path);

	c_error = pcParser->eParse();

	if (!c_error)
	{
		ProblemTools::CKnapsackProblem c_knapsack_problem;

		uint32_t i_number_of_variables = c_number_of_variables.iGetValue();
		
		double d_capacity = i_number_of_variables * (i_number_of_variables + 1) * ((double)(i_number_of_variables - 1) / 2.0) 
			+ (double)(i_number_of_variables * (i_number_of_variables - 1)) / 2.0;

		c_error = c_knapsack_problem.eConfigure(i_number_of_variables, -1, d_capacity);

		double d_value_or_weight;

		for (uint32_t i = 0; i < i_number_of_variables && !c_error; i++)
		{
			d_value_or_weight = i_number_of_variables * (i_number_of_variables + 1) + i + 1;

			c_error = c_knapsack_problem.eAddValue(i, d_value_or_weight);

			if (!c_error)
			{
				c_error = c_knapsack_problem.eAddWeight(i, d_value_or_weight);
			}//if (!c_error)
		}//for (uint32_t i = 0; i < c_number_of_clauses.iGetValue() && !c_error; i++)

		if (!c_error)
		{
			FILE *pf_output = fopen(c_output_file_path.sGetValue(), "w");

			if (pf_output != nullptr)
			{
				c_knapsack_problem.vSaveSettings(pf_output);
				fclose(pf_output);
			}//if (pf_output != nullptr)
			else
			{
				c_error.vSetError(CError::iERROR_CODE_SYSTEM_FILE_NOT_FOUND, c_output_file_path.sGetValue());
			}//else if (pf_output != nullptr)
		}//if (!c_error)
	}//if (!c_error)

	return c_error;
}//CError CAvisSubsetSumKnapsackProblemGenerator::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)

CGenerateRandomMaxSat::CGenerateRandomMaxSat(CLog *pcLog, ostream *psSummary)
	: CCommandExecution(SYSTEM_ARGUMENT_MODE_GENERATE_RANDOM_MAX_SAT, nullptr, pcLog, psSummary)
{

}//CGenerateRandomMaxSat::CGenerateRandomMaxSat(CLog *pcLog, ostream *psSummary)

CError CGenerateRandomMaxSat::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)
{
	CError c_error;

	CUIntCommandParam c_number_of_variables(SYSTEM_ARGUMENT_NUMBER_OF_VARIABLES, 1, INT_MAX);
	CUIntCommandParam c_number_of_clauses(SYSTEM_ARGUMENT_NUMBER_OF_CLAUSES, 1, INT_MAX);
	CUIntCommandParam c_clause_length(SYSTEM_ARGUMENT_CLAUSE_LENGTH, 1, INT_MAX);
	CStringCommandParam c_output_file_path(SYSTEM_ARGUMENT_OUTPUT_FILE_PATH);

	pcParser->vRegister(&c_number_of_variables);
	pcParser->vRegister(&c_number_of_clauses);
	pcParser->vRegister(&c_clause_length);
	pcParser->vRegister(&c_output_file_path);

	c_error = pcParser->eParse();

	if (!c_error)
	{
		ProblemTools::CMaxSatProblem c_max_sat_problem;

		c_error = c_max_sat_problem.eConfigure(c_number_of_variables.iGetValue(), c_number_of_clauses.iGetValue());

		int32_t i_variable_value;

		for (uint32_t i = 0; i < c_number_of_clauses.iGetValue() && !c_error; i++)
		{
			c_error = c_max_sat_problem.eAddClause(i, c_clause_length.iGetValue());

			for (uint32_t j = 0; j < c_clause_length.iGetValue() && !c_error; j++)
			{
				do//while (i_variable_value == 0)
				{
					i_variable_value = RandUtils::iRandNumber(-(int32_t)c_number_of_variables.iGetValue(), (int32_t)c_number_of_variables.iGetValue());
				} while (i_variable_value == 0);

				c_error = c_max_sat_problem.eAddVariable(i, j, i_variable_value);
			}//for (uint32_t j = 0; j < c_clause_length.iGetValue() && !c_error; j++)
		}//for (uint32_t i = 0; i < c_number_of_clauses.iGetValue() && !c_error; i++)

		if (!c_error)
		{
			FILE *pf_output = fopen(c_output_file_path.sGetValue(), "w");

			if (pf_output != nullptr)
			{
				c_max_sat_problem.vSaveSettings(pf_output);
				fclose(pf_output);
			}//if (pf_output != nullptr)
			else
			{
				c_error.vSetError(CError::iERROR_CODE_SYSTEM_FILE_NOT_FOUND, c_output_file_path.sGetValue());
			}//else if (pf_output != nullptr)
		}//if (!c_error)
	}//if (!c_error)

	return c_error;
}//CError CGenerateRandomMaxSat::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)

CPlantedMaxSat2ProblemGeneration::CPlantedMaxSat2ProblemGeneration(CLog *pcLog, ostream *psSummary)
	: CCommandExecution(SYSTEM_ARGUMENT_MODE_PLANTED_MAX_SAT_2_PROBLEM_GENERATION, nullptr, pcLog, psSummary)
{

}//CPlantedMaxSat2ProblemGeneration::CGenerateRandomMaxSat(CLog *pcLog, ostream *psSummary)

CError CPlantedMaxSat2ProblemGeneration::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)
{
	CError c_error;

	CUIntCommandParam c_number_of_variables(SYSTEM_ARGUMENT_NUMBER_OF_VARIABLES, 1, UINT16_MAX);
	CFloatCommandParam c_p(SYSTEM_ARGUMENT_P, 0.0f, 1.0f);
	CFloatCommandParam c_r(SYSTEM_ARGUMENT_R, 0.0f, 1.0f);
	CStringCommandParam c_output_file_path(SYSTEM_ARGUMENT_OUTPUT_FILE_PATH);

	pcParser->vRegister(&c_number_of_variables);
	pcParser->vRegister(&c_p);
	pcParser->vRegister(&c_r);
	pcParser->vRegister(&c_output_file_path);

	c_error = pcParser->eParse();

	if (!c_error)
	{
		ProblemTools::CMaxSatProblem c_max_sat_problem;

		uint16_t i_number_of_variables = (uint16_t)c_number_of_variables.iGetValue();

		float f_p = c_p.fGetValue();
		float f_r = c_r.fGetValue();

		uint32_t *pi_planted_solution_0 = new uint32_t[i_number_of_variables];
		uint32_t *pi_planted_solution_1 = new uint32_t[i_number_of_variables];

		uint32_t i_rand_number;

		for (uint16_t i = 0; i < i_number_of_variables; i++)
		{
			i_rand_number = RandUtils::iRandNumber(0, 1);

			*(pi_planted_solution_0 + i) = i_rand_number;
			*(pi_planted_solution_1 + i) = 1 - i_rand_number;
		}//for (uint16_t i = 0; i < i_number_of_variables; i++)

		vector<int32_t*> v_clauses;

		function<void(uint16_t, uint16_t, uint32_t, uint32_t)> f_add_clause = [&](uint16_t i, uint16_t j, uint32_t iL0, uint32_t iL1) -> void
		{
			bool b_add = false;

			bool b_planted_0_satisfies = *(pi_planted_solution_0 + i) == iL0 || *(pi_planted_solution_0 + j) == iL1;
			bool b_planted_1_satisfies = *(pi_planted_solution_1 + i) == iL0 || *(pi_planted_solution_1 + j) == iL1;

			if (b_planted_0_satisfies && b_planted_1_satisfies)
			{
				b_add = RandUtils::bSatisfyProbability(f_p);
			}//if (b_planted_0_satisfies && b_planted_1_satisfies)
			else
			{
				b_add = RandUtils::bSatisfyProbability(f_r);
			}//else if (b_planted_0_satisfies && b_planted_1_satisfies)

			if (b_add)
			{
				int32_t *pi_clause = new int32_t[2];

				i++;
				j++;

				*(pi_clause + 0) = iL0 == 0 ? -i : i;
				*(pi_clause + 1) = iL1 == 0 ? -j : j;

				v_clauses.push_back(pi_clause);
			}//if (b_add)
		};//function<void(uint16_t, uint16_t, uint32_t, uint32_t)> f_add_clause = [&](uint16_t i, uint16_t j, uint32_t iL0, uint32_t iL1) -> void

		for (uint16_t i = 0; i < i_number_of_variables; i++)
		{
			for (uint16_t j = i; j < i_number_of_variables; j++)
			{
				if (i != j)
				{
					f_add_clause(i, j, 1, 1);
					f_add_clause(i, j, 1, 0);
					f_add_clause(i, j, 0, 1);
					f_add_clause(i, j, 0, 0);
				}//if (i != j)
				else
				{
					f_add_clause(i, j, 1, 1);
					f_add_clause(i, j, 1, 0);
					f_add_clause(i, j, 0, 0);
				}//else if (i != j)
			}//for (uint16_t j = i; j < i_number_of_variables; j++)
		}//for (uint16_t i = 0; i < i_number_of_variables; i++)

		c_error = c_max_sat_problem.eConfigure(i_number_of_variables, (int)v_clauses.size());

		for (uint32_t i = 0; i < (uint32_t)v_clauses.size() && !c_error; i++)
		{
			c_error = c_max_sat_problem.eAddClause(i, 2);

			if (!c_error)
			{
				c_max_sat_problem.eAddVariable(i, 0, *(v_clauses.at(i) + 0));
				c_max_sat_problem.eAddVariable(i, 1, *(v_clauses.at(i) + 1));
			}//if (!c_error)
		}//for (uint32_t i = 0; i < (uint32_t)v_clauses.size() && !c_error; i++)

		if (!c_error)
		{
			FILE *pf_output = fopen(c_output_file_path.sGetValue(), "w");

			if (pf_output != nullptr)
			{
				c_max_sat_problem.vSaveSettings(pf_output);
				fclose(pf_output);

				double d_planted_0_value = c_max_sat_problem.dGetFuncValue(0, (int*)pi_planted_solution_0, i_number_of_variables);
				double d_planted_1_value = c_max_sat_problem.dGetFuncValue(0, (int*)pi_planted_solution_1, i_number_of_variables);

				bool b_planted_0_better = d_planted_0_value > d_planted_1_value;

				(*ps_summary) << (b_planted_0_better ? d_planted_0_value : d_planted_1_value) << "\t";

				uint32_t *pi_better_planted = b_planted_0_better ? pi_planted_solution_0 : pi_planted_solution_1;

				for (uint16_t i = 0; i < i_number_of_variables; i++)
				{
					(*ps_summary) << (int)*(pi_better_planted + i);
				}//for (uint16_t i = 0; i < i_number_of_variables; i++)
			}//if (pf_output != nullptr)
			else
			{
				c_error.vSetError(CError::iERROR_CODE_SYSTEM_FILE_NOT_FOUND, c_output_file_path.sGetValue());
			}//else if (pf_output != nullptr)
		}//if (!c_error)

		delete pi_planted_solution_0;
		delete pi_planted_solution_1;

		for (uint32_t i = 0; i < (uint32_t)v_clauses.size(); i++)
		{
			delete v_clauses.at(i);
		}//for (uint32_t i = 0; i < v_clauses.size(); i++)

		v_clauses.clear();
	}//if (!c_error)

	return c_error;
}//CError CPlantedMaxSat2ProblemGeneration::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)


CInjectedPopulationOptimizerRepeatabilityCheckingCommand::CInjectedPopulationOptimizerRepeatabilityCheckingCommand(istream *psSettings, CLog *pcLog, ostream *psSummary)
	: CCommandExecution(SYSTEM_ARGUMENT_MODE_INJECTED_POPULATION_OPTIMIZER_REPEATABILITY_CHECKING, psSettings, pcLog, psSummary)
{

}//CInjectedPopulationOptimizerRepeatabilityCheckingCommand::CInjectedPopulationOptimizerRepeatabilityCheckingCommand(istream *psSettings, CLog *pcLog, ostream *psSummary)

CError CInjectedPopulationOptimizerRepeatabilityCheckingCommand::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)
{
	CError c_error;

	CUIntCommandParam p_number_of_repeats(SYSTEM_ARGUMENT_NUMBER_OF_REPEATS, 1, UINT8_MAX);
	uint8_t i_number_of_repeats = (uint8_t)p_number_of_repeats.iGetValue(ps_settings, &c_error);

	if (!c_error)
	{
		CEvaluation<CRealCoding> *pc_evaluation = EvaluationUtils::pcGetEvaluation<CRealCoding>(ps_settings, &c_error);

		if (!c_error)
		{
			CRealCoding *pc_sample_genotype = pc_evaluation->pcCreateSampleFenotype();

			CRealRandomGeneration c_random_generation(pc_sample_genotype);
			c_error = c_random_generation.eConfigure(nullptr, ps_settings);

			delete pc_sample_genotype;

			if (!c_error)
			{
				CProblem<CRealCoding, CRealCoding> c_problem(pc_evaluation, new CUniformTransformation<CRealCoding>());

				pc_sample_genotype = c_random_generation.pcGenerate();

				CUIntCommandParam p_number_of_injected_genotypes(SYSTEM_ARGUMENT_NUMBER_OF_INJECTED_GENOTYPES, 2, UINT32_MAX);
				uint32_t i_number_of_injected_genotypes = p_number_of_injected_genotypes.iGetValue(ps_settings, &c_error);

				if (!c_error)
				{
					CKMeansClustering c_kmeans(pc_log);
					
					c_error = c_kmeans.eConfigure(ps_settings);

					if (!c_error)
					{
						CDifferentialEvolution<CRealCoding, CRealCoding> *pc_population_optimizer = new CDifferentialEvolution<CRealCoding, CRealCoding>(&c_problem, pc_log, iRandomSeed);

						CRealCoding **ppc_injected_genotypes = new CRealCoding*[i_number_of_injected_genotypes];

						*(ppc_injected_genotypes + 0) = pc_sample_genotype;

						CRealCoding *pc_injected_genotype;

						for (uint32_t i = 1; i < i_number_of_injected_genotypes; i++)
						{
							pc_injected_genotype = new CRealCoding(pc_sample_genotype);

							for (uint16_t j = 0; j < pc_injected_genotype->iGetNumberOfDimensions(); j++)
							{
								*(pc_injected_genotype->pdGetValues() + j) = RandUtils::dRandNumber(*(pc_injected_genotype->pdGetMinValues() + j), 
									*(pc_injected_genotype->pdGetMaxValues() + j));
							}//for (uint16_t j = 0; j < pc_injected_genotype->iGetNumberOfDimensions(); j++)

							pc_injected_genotype->vRepair();

							*(ppc_injected_genotypes + i) = pc_injected_genotype;
						}//for (uint32_t i = 1; i < i_number_of_injected_genotypes; i++)

						c_kmeans.vRun(i_number_of_injected_genotypes, ppc_injected_genotypes);

						pc_log->vPrintLine(c_kmeans.sToString());

						CCluster<CRealCoding> *pc_first_cluster = *(c_kmeans.ppcGetClusters() + 0);

						CString s_first_cluster;
						s_first_cluster.Format("first cluster: %d", pc_first_cluster->iGetSize());
						pc_log->vPrintLine(s_first_cluster);

						CRealCoding *pc_first_cluster_first_point = *(pc_first_cluster->ppcGetPoints() + 1);

						CString s_first_cluster_first_point;
						s_first_cluster_first_point.Format("first cluster first point: %f", *(pc_first_cluster_first_point->pdGetValues() + 0));
						pc_log->vPrintLine(s_first_cluster_first_point);

						//pc_population_optimizer->vInjectGenotypes(i_number_of_injected_genotypes, ppc_injected_genotypes, true);

						pc_population_optimizer->vInjectGenotypes(pc_first_cluster->iGetSize(), pc_first_cluster->ppcGetPoints(), false);

						c_error = pc_population_optimizer->eConfigure(ps_settings);

						if (!c_error)
						{
							CKMeansCluster *pc_cluster;

							for (uint32_t j = 0; j < c_kmeans.iGetNumberOfClusters(); j++)
							{
								pc_cluster = (CKMeansCluster*)*(c_kmeans.ppcGetClusters() + j);

								pc_population_optimizer->vInjectGenotypes(pc_cluster->iGetSize(), pc_cluster->ppcGetPoints(), false);

								//pc_population_optimizer->vSetCenterPoint(pc_cluster->pcGetCenterPoint());

								for (uint32_t k = 0; k < pc_cluster->iGetSize(); k++)
								{
									pc_log->vPrintLine((*(pc_cluster->ppcGetPoints() + k))->sToString());
								}//for (uint32_t k = 0; k < pc_cluster->iGetSize(); k++)

								double **ppd_vectors = new double*[i_number_of_repeats];

								//*ps_summary << " base: " << pc_sample_genotype->sToString();

								*ps_summary << " base: " << pc_cluster->pcGetCenterPoint()->sToString();

								for (uint8_t i = 0; i < i_number_of_repeats; i++)
								{
									pc_population_optimizer->vRun();

									pc_log->vPrintEmptyLine();

									*ps_summary << "\t" << (int)i << ": " << pc_population_optimizer->pcGetBestIndividual()->pcGetGenotype()->sToString()
										<< " (" << pc_population_optimizer->pcGetBestIndividual()->dGetFitnessValue() << ")";

									*(ppd_vectors + i) = new double[pc_sample_genotype->iGetNumberOfDimensions() + 1];

									for (uint16_t j = 0; j < pc_sample_genotype->iGetNumberOfDimensions(); j++)
									{
										*(*(ppd_vectors + i) + j) = *(pc_population_optimizer->pcGetBestIndividual()->pcGetGenotype()->pdGetValues() + j);
									}//for (uint16_t j = 0; j < pc_sample_genotype->iGetNumberOfDimensions(); j++)

									*(*(ppd_vectors + i) + pc_sample_genotype->iGetNumberOfDimensions()) = pc_population_optimizer->pcGetBestIndividual()->dGetFitnessValue();
								}//for (uint8_t i = 0; i < i_number_of_repeats; i++)

								pc_log->vPrintLine("MATRIX");

								CString s_angle_matrix_row;

								for (uint8_t i = 0; i < i_number_of_repeats; i++)
								{
									s_angle_matrix_row.Empty();

									for (uint8_t j = 0; j < i_number_of_repeats; j++)
									{
										if (i != j)
										{
											s_angle_matrix_row.AppendFormat("%f", MathUtils::dComputeAngle(*(ppd_vectors + i), *(ppd_vectors + j),
												pc_sample_genotype->iGetNumberOfDimensions() + 1));
										}//if (i != j)

										if (j < i_number_of_repeats - 1)
										{
											s_angle_matrix_row.Append("\t");
										}//if (j < i_number_of_repeats - 1)
									}//for (uint8_t j = 0; j < i_number_of_repeats; j++)

									pc_log->vPrintLine(s_angle_matrix_row);
								}//for (uint8_t i = 0; i < i_number_of_repeats; i++)

								for (uint8_t i = 0; i < i_number_of_repeats; i++)
								{
									delete *(ppd_vectors + i);
								}//for (uint8_t i = 0; i < i_number_of_repeats; i++)

								delete ppd_vectors;
							}//for (uint32_t j = 0; j < c_kmeans.iGetNumberOfClusters(); j++)
						}//if (!c_error)

						delete pc_population_optimizer;
					}//if (!c_error)
				}//if (!c_error)
			}//if (!c_error)
		}//if (!c_error)
	}//if (!c_error)

	return c_error;
}//CError CInjectedPopulationOptimizerRepeatabilityCheckingCommand::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)


CChangeDetectionCommand::CChangeDetectionCommand(istream *psSettings, CLog *pcLog, uint32_t iRandomSeed, ostream *psSummary)
	: CCommandExecution(SYSTEM_ARGUMENT_MODE_CHANGE_DETECTION, psSettings, pcLog, psSummary)
{
	i_random_seed = iRandomSeed;
}//CChangeDetectionCommand::CChangeDetectionCommand(istream *psSettings, CLog *pcLog, uint32_t iRandomSeed, ostream *psSummary)

CError CChangeDetectionCommand::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)
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
				c_error = e_detect_changes<CBinaryCoding, CBinaryCoding>();
				break;
			}//case PROBLEM_BINARY_BINARY
			case PROBLEM_REAL_REAL:
			{
				c_error = e_detect_changes<CRealCoding, CRealCoding>();
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
}//CError CChangeDetectionCommand::eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed)

template <class TGenotype, class TFenotype>
CError CChangeDetectionCommand::e_detect_changes()
{
	CError c_error;

	CDynamicEvaluation<TFenotype> *pc_dynamic_evaluation = EvaluationUtils::pcGetDynamicEvaluation<TFenotype>(ps_settings, &c_error);

	if (!c_error)
	{
		CProblem<TGenotype, TFenotype> *pc_problem = new CProblem<TGenotype, TFenotype>(pc_dynamic_evaluation, new CUniformTransformation<TGenotype>());
		CChangeDetector<TGenotype, TFenotype> *pc_change_detector = pcGetChangeDetector(pc_problem, pc_log, i_random_seed, ps_settings, &c_error);

		if (!c_error)
		{
			pc_change_detector->vRun();

			CString s_summary;

			s_summary.Format("%d\t%d\t%d\t%d\t%d", pc_change_detector->iGetTruePositiveCount(), pc_change_detector->iGetFalsePositiveCount(),
				pc_change_detector->iGetTrueNegativeCount(), pc_change_detector->iGetFalseNegativeCount(), pc_dynamic_evaluation->iGetFFE());

			cout << s_summary;

			(*ps_summary) << s_summary;
		}//if (!c_error)

		delete pc_change_detector;
		delete pc_problem;
	}//if (!c_error)
	else
	{
		delete pc_dynamic_evaluation;
	}//else if (!c_error)

	return c_error;
}//CError CChangeDetectionCommand::e_detect_changes()