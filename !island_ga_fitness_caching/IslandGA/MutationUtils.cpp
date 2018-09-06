#include "MutationUtils.h"

#include "BinaryNegationMutation.h"
#include "EnumCommandParam.h"
#include "RealMutation.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>

template <class TGenotype>
CMutation<TGenotype> * MutationUtils::pcGetMutation(CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CMutation<TGenotype> *pc_mutation = nullptr;

	size_t i_genotype_type_hash_code = typeid(TGenotype).hash_code();

	unordered_map<CString, EMutationType> m_mutation_types;

	if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		m_mutation_types.insert(pair<const CString, EMutationType>(MUTATION_ARGUMENT_TYPE_BINARY_NEGATION, MUTATION_BINARY_NEGATION));
		m_mutation_types.insert(pair<const CString, EMutationType>(MUTATION_ARGUMENT_TYPE_BINARY_GENE_PATTERN_BEST_SWAP, MUTATION_BINARY_GENE_PATTERN_BEST_SWAP));
	}//if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	else if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code())
	{
		m_mutation_types.insert(pair<const CString, EMutationType>(MUTATION_ARGUMENT_TYPE_REAL_RANDOM, MUTATION_REAL_RANDOM));
		m_mutation_types.insert(pair<const CString, EMutationType>(MUTATION_ARGUMENT_TYPE_REAL_TARGET_TO_BEST, MUTATION_REAL_TARGET_TO_BEST));
		m_mutation_types.insert(pair<const CString, EMutationType>(MUTATION_ARGUMENT_TYPE_REAL_RAND_1, MUTATION_REAL_RAND_1));
		m_mutation_types.insert(pair<const CString, EMutationType>(MUTATION_ARGUMENT_TYPE_REAL_NRAND_1, MUTATION_REAL_NRAND_1));
	}//else if (i_genotype_type_hash_code == typeid(CRealCoding).hash_code())

	CEnumCommandParam<EMutationType> p_type(MUTATION_ARGUMENT_TYPE, &m_mutation_types, bIsObligatory);
	EMutationType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case MUTATION_BINARY_NEGATION:
			{
				pc_mutation = (CMutation<TGenotype>*)new CBinaryNegationMutation();
				break;
			}//case MUTATION_BINARY_NEGATION
			case MUTATION_BINARY_GENE_PATTERN_BEST_SWAP:
			{
				pc_mutation = (CMutation<TGenotype>*)new CBinaryGenePatternBestSwapMutation();
				break;
			}//case MUTATION_BINARY_GENE_PATTERN_BEST_SWAP
			case MUTATION_REAL_RANDOM:
			{
				pc_mutation = (CMutation<TGenotype>*)new CRealRandomMutation();
				break;
			}//case MUTATION_REAL_RANDOM
			case MUTATION_REAL_TARGET_TO_BEST:
			{
				pc_mutation = (CMutation<TGenotype>*)new CRealTargetToBestMutation();
				break;
			}//case MUTATION_REAL_TARGET_TO_BEST
			case MUTATION_REAL_RAND_1:
			{
				pc_mutation = (CMutation<TGenotype>*)new CRealRand1Mutation();
				break;
			}//case MUTATION_REAL_RAND_1
			case MUTATION_REAL_NRAND_1:
			{
				pc_mutation = (CMutation<TGenotype>*)new CRealNRand1Mutation();
				break;
			}//case MUTATION_REAL_NRAND_1
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "mutation");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_mutation->eConfigure(plGenePatterns, psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_mutation;
}//CMutation<TGenotype> * MutationUtils::pcGetMutation(CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)


template CMutation<CBinaryCoding> * MutationUtils::pcGetMutation(CGenePatternList*, istream*, CError*, bool);
template CMutation<CRealCoding> * MutationUtils::pcGetMutation(CGenePatternList*, istream*, CError*, bool);