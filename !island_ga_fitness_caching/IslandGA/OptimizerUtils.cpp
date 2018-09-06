#include "OptimizerUtils.h"

#include "BinaryCoding.h"
#include "DifferentialEvolution.h"
#include "DSMGA2.h"
#include "DummyOptimizer.h"
#include "EnumCommandParam.h"
#include "LTGA.h"
#include "GeneticAlgorithm.h"
#include "IncrementalP3.h"
#include "IslandCoevolutionDSMGA2.h"
#include "IslandModelGeneticAlgorithm.h"
#include "IslandModelP3.h"
#include "MultiDifferentWithGlobalP3.h"
#include "MultiOptimizer.h"
#include "MultiPopulationGeneticAlgorithm.h"
#include "P3.h"
#include "ParticleSwarmOptimization.h"
#include "RealCoding.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>
#include <utility>



template <class TGenotype, class TFenotype>
COptimizer<TGenotype, TFenotype> * OptimizerUtils::pcGetOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	COptimizer<TGenotype, TFenotype> *pc_optimizer = nullptr;

	size_t i_genotype_type_hash_code = typeid(TGenotype).hash_code();
	size_t i_fenotype_type_hash_code = typeid(TFenotype).hash_code();

	unordered_map<CString, EOptimizerType> m_optimizer_types;
	m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_MULTI, OPTIMIZER_MULTI));
	m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_GA, OPTIMIZER_GA));
	m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_MGA, OPTIMIZER_MGA));
	m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_ISLAND_GA, OPTIMIZER_ISLAND_GA));
	m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_DE, OPTIMIZER_DE));

	if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code() && i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_P3, OPTIMIZER_P3));
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_ISLAND_P3, OPTIMIZER_ISLAND_P3));
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_INCREMENTAL_P3, OPTIMIZER_INCREMENTAL_P3));
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_MULTI_DIFFERENT_WITH_GLOBAL_P3, OPTIMIZER_MULTI_DIFFERENT_WITH_GLOBAL_P3));
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_LTGA, OPTIMIZER_LTGA));
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_DSMGA2, OPTIMIZER_DSMGA2));
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_ISLAND_COEVOLUTION_DSMGA2, OPTIMIZER_ISLAND_COEVOLUTION_DSMGA2));
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_DUMMY, OPTIMIZER_DUMMY));
	}//if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code() && i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())

	if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code() && i_fenotype_type_hash_code == typeid(CRealCoding).hash_code())
	{
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_PSO, OPTIMIZER_PSO));
	}//if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code() && i_fenotype_type_hash_code == typeid(CRealCoding).hash_code())

	CEnumCommandParam<EOptimizerType> p_type(OPTIMIZER_ARGUMENT_TYPE, &m_optimizer_types, bIsObligatory);
	EOptimizerType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case OPTIMIZER_MULTI:
			{
				pc_optimizer = new CMultiOptimizer<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_MULTI
			case OPTIMIZER_GA:
			{
				pc_optimizer = new CGeneticAlgorithm<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_GA
			case OPTIMIZER_MGA:
			{
				pc_optimizer = new CMultiPopulationGeneticAlgorithm<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_MGA
			case OPTIMIZER_ISLAND_GA:
			{
				pc_optimizer = new CIslandModelGeneticAlgorithm<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_ISLAND_GA
			case OPTIMIZER_DE:
			{
				pc_optimizer = new CDifferentialEvolution<TGenotype, TFenotype>(pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_DE
			case OPTIMIZER_PSO:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CParticleSwarmOptimization((CProblem<CRealCoding, CRealCoding>*)pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_PSO
			case OPTIMIZER_P3:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CP3((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_P3
			case OPTIMIZER_ISLAND_P3:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CIslandModelP3((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_ISLAND_P3
			case OPTIMIZER_INCREMENTAL_P3:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CIncrementalP3((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_INCREMENTAL_P3
			case OPTIMIZER_MULTI_DIFFERENT_WITH_GLOBAL_P3:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CMultiDifferentWithGlobalP3((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_MULTI_DIFFERENT_WITH_GLOBAL_P3
			case OPTIMIZER_LTGA:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CLTGA((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_LTGA
			case OPTIMIZER_DSMGA2:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CDSMGA2((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_DSMGA2
			case OPTIMIZER_ISLAND_COEVOLUTION_DSMGA2:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CIslandCoevolutionDSMGA2((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_ISLAND_COEVOLUTION_DSMGA2
			case OPTIMIZER_DUMMY:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CDummyOptimizer((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog, iRandomSeed);
				break;
			}//case OPTIMIZER_DUMMY
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "optimizer");
				break;
			}//default
		}//switch (e_type)
	}//if (!*pcError && p_type.bHasValue())

	if (!*pcError)
	{
		*pcError = pc_optimizer->eConfigure(psSettings);
	}//if (!*pcError)

	return pc_optimizer;
}//COptimizer<TGenotype, TFenotype> * OptimizerUtils::pcGetOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed, istream *psSettings, COptimizerParams<TGenotype, TFenotype> **ppcParams, CError *pcError, bool bIsObligatory)

template<class TGenotype, class TFenotype>
CPopulationOptimizer<TGenotype, TFenotype>* OptimizerUtils::pcGetPopulationOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	COptimizer<TGenotype, TFenotype> *pc_optimizer = pcGetOptimizer(pcProblem, pcLog, iRandomSeed, psSettings, pcError, bIsObligatory);

	if (!*pcError && pc_optimizer)
	{
		if (!dynamic_cast<CPopulationOptimizer<TGenotype, TFenotype>*>(pc_optimizer))
		{
			pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "optimizer instead of population optimizer");
		}//if (!dynamic_cast<CPopulationOptimizer<TGenotype, TFenotype>*>(pc_optimizer))
	}//if (!*pcError && pc_optimizer)

	return (CPopulationOptimizer<TGenotype, TFenotype>*)pc_optimizer;
}//COptimizer<TGenotype, TFenotype>* OptimizerUtils::pcGetPopulationOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, uint32_t iRandomSeed, istream *psSettings, CError *pcError, bool bIsObligatory)

template COptimizer<CBinaryCoding, CBinaryCoding> * OptimizerUtils::pcGetOptimizer(CProblem<CBinaryCoding, CBinaryCoding>*, CLog*, uint32_t, istream*, CError*, bool);
template COptimizer<CRealCoding, CRealCoding> * OptimizerUtils::pcGetOptimizer(CProblem<CRealCoding, CRealCoding>*, CLog*, uint32_t, istream*, CError*, bool);

template CPopulationOptimizer<CBinaryCoding, CBinaryCoding> * OptimizerUtils::pcGetPopulationOptimizer(CProblem<CBinaryCoding, CBinaryCoding>*, CLog*, uint32_t, istream*, CError*, bool);
template CPopulationOptimizer<CRealCoding, CRealCoding> * OptimizerUtils::pcGetPopulationOptimizer(CProblem<CRealCoding, CRealCoding>*, CLog*, uint32_t, istream*, CError*, bool);