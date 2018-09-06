#include "GenerationUtils.h"

#include "BinaryCoding.h"
#include "BinaryEvaluation.h"
#include "BinaryGeneration.h"
#include "EnumCommandParam.h"
#include "RealCoding.h"
#include "RealGeneration.h"
#include "RealRandomGeneration.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>
#include <utility>

template <class TGenotype, class TFenotype>
CGeneration<TFenotype> * GenerationUtils::pcGetSampleFenotypeGeneration(CProblem<TGenotype, TFenotype> *pcProblem)
{
	CGeneration<TFenotype> *pc_sample_fenotype_generation = nullptr;

	TFenotype *pc_sample_fenotype = pcProblem->pcGetEvaluation()->pcCreateSampleFenotype();

	size_t i_fenotype_type_hash_code = typeid(TFenotype).hash_code();

	if (i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		pc_sample_fenotype_generation = (CGeneration<TFenotype>*)new CBinaryGeneration((CBinaryCoding*)pc_sample_fenotype);
	}//if (i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	else if (i_fenotype_type_hash_code == typeid(CRealCoding).hash_code())
	{
		pc_sample_fenotype_generation = (CGeneration<TFenotype>*)new CRealGeneration((CRealCoding*)pc_sample_fenotype);
	}//else if (i_fenotype_type_hash_code == typeid(CRealCoding).hash_code())

	delete pc_sample_fenotype;

	return pc_sample_fenotype_generation;
}//CGeneration<TFenotype> * GenerationUtils::pcGetSampleFenotypeGeneration(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CGeneration<TGenotype> * GenerationUtils::pcGetEmptyGeneration(CProblem<TGenotype, TFenotype> *pcProblem)
{
	CGeneration<TGenotype> *pc_empty_generation = nullptr;

	size_t i_genotype_type_hash_code = typeid(TFenotype).hash_code();

	CGeneration<TFenotype> *pc_sample_fenotype_generation = pcGetSampleFenotypeGeneration(pcProblem);

	TFenotype *pc_sample_fenotype = pc_sample_fenotype_generation->pcGenerateEmpty();
	TGenotype *pc_sample_genotype = pcProblem->pcGetTransformation()->pcInverselyTransform(pc_sample_fenotype);

	if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		pc_empty_generation = (CGeneration<TGenotype>*)new CBinaryGeneration((CBinaryCoding*)pc_sample_genotype);
	}//if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	else if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code())
	{
		pc_empty_generation = (CGeneration<TGenotype>*)new CRealGeneration((CRealCoding*)pc_sample_genotype);
	}//else if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code())

	delete pc_sample_fenotype_generation;

	if (pc_sample_genotype != pc_sample_fenotype)
	{
		delete pc_sample_genotype;
	}//if (pc_sample_genotype != pc_sample_fenotype)

	delete pc_sample_fenotype;

	return pc_empty_generation;
}//CGeneration<TGenotype> * GenerationUtils::pcGetEmptyGeneration(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CGeneration<TGenotype> * GenerationUtils::pcGetGeneration(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CGeneration<TGenotype> *pc_generation = nullptr;

	size_t i_genotype_type_hash_code = typeid(TGenotype).hash_code();

	unordered_map<CString, EGenerationType> m_generation_types;

	if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		m_generation_types.insert(pair<const CString, EGenerationType>(GENERATION_ARGUMENT_TYPE_BINARY_RANDOM, GENERATION_BINARY_RANDOM));
		m_generation_types.insert(pair<const CString, EGenerationType>(GENERATION_ARGUMENT_TYPE_BINARY_GENE_PATTERN_RANDOM, GENERATION_BINARY_RANDOM_GENE_PATTERN));
	}//if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	else if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code())
	{
		m_generation_types.insert(pair<const CString, EGenerationType>(GENERATION_ARGUMENT_TYPE_REAL_RANDOM, GENERATION_REAL_RANDOM));
	}//else if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code())

	CEnumCommandParam<EGenerationType> p_type(GENERATION_ARGUMENT_TYPE, &m_generation_types, bIsObligatory);
	EGenerationType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		CGeneration<TFenotype> *pc_sample_fenotype_generation = pcGetSampleFenotypeGeneration(pcProblem);

		TFenotype *pc_sample_fenotype = pc_sample_fenotype_generation->pcGenerateEmpty();
		TGenotype *pc_sample_genotype = pcProblem->pcGetTransformation()->pcInverselyTransform(pc_sample_fenotype);

		switch (e_type)
		{
			case GENERATION_BINARY_RANDOM:
			{
				pc_generation = (CGeneration<TGenotype>*)new CBinaryRandomGeneration((CBinaryCoding*)pc_sample_genotype);
				break;
			}//case GENERATION_BINARY_RANDOM
			case GENERATION_BINARY_RANDOM_GENE_PATTERN:
			{
				pc_generation = (CGeneration<TGenotype>*)new CBinaryRandomGenePatternGeneration((CBinaryCoding*)pc_sample_genotype);
				break;
			}//case GENERATION_BINARY_RANDOM_GENE_PATTERN
			case GENERATION_REAL_RANDOM:
			{
				pc_generation = (CGeneration<TGenotype>*)new CRealRandomGeneration((CRealCoding*)pc_sample_genotype);
				break;
			}//case GENERATION_REAL_RANDOM
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "generation");
				break;
			}//default
		}//switch (e_type)

		if (!*pcError)
		{
			*pcError = pc_generation->eConfigure(plGenePatterns, psSettings);
		}//if (!*pcError)

		delete pc_sample_fenotype_generation;

		if (pc_sample_genotype != pc_sample_fenotype)
		{
			delete pc_sample_genotype;
		}//if (pc_sample_genotype != pc_sample_fenotype)

		delete pc_sample_fenotype;
	}//if (!*pcError && p_type.bHasValue())

	return pc_generation;
}//CGeneration<TGenotype> * GenerationUtils::pcGetGeneration(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)

template CGeneration<CBinaryCoding> * GenerationUtils::pcGetSampleFenotypeGeneration(CProblem<CBinaryCoding, CBinaryCoding>*);
template CGeneration<CRealCoding> * GenerationUtils::pcGetSampleFenotypeGeneration(CProblem<CRealCoding, CRealCoding>*);

template CGeneration<CBinaryCoding> * GenerationUtils::pcGetEmptyGeneration(CProblem<CBinaryCoding, CBinaryCoding>*);
template CGeneration<CRealCoding> * GenerationUtils::pcGetEmptyGeneration(CProblem<CRealCoding, CRealCoding>*);

template CGeneration<CBinaryCoding> * GenerationUtils::pcGetGeneration(CProblem<CBinaryCoding, CBinaryCoding>*, CGenePatternList*, istream*, CError*, bool);
template CGeneration<CRealCoding> * GenerationUtils::pcGetGeneration(CProblem<CRealCoding, CRealCoding>*, CGenePatternList*, istream*, CError*, bool);