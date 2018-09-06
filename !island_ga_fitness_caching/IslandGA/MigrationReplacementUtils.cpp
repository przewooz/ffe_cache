#include "MigrationReplacementUtils.h"

#include "EnumCommandParam.h"
#include "GenePatternList.h"
#include "RandomMigrationReplacement.h"
#include "RealCoding.h"
#include "WorstMigrationReplacement.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>
#include <utility>

template <class TGenotype, class TFenotype>
CMigrationReplacement<TGenotype, TFenotype> * MigrationReplacementUtils::pcGetMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CMigrationReplacement<TGenotype, TFenotype> *pc_migration_replacement = nullptr;

	size_t i_genotype_type_hash_code = typeid(TGenotype).hash_code();

	unordered_map<CString, EMigrationReplacementType> m_migration_replacement_types;
	m_migration_replacement_types.insert(pair<const CString, EMigrationReplacementType>(MIGRATION_REPLACEMENT_ARGUMENT_TYPE_MULTI_DETERMINISTIC, MIGRATION_REPLACEMENT_MULTI_DETERMINISTIC));
	m_migration_replacement_types.insert(pair<const CString, EMigrationReplacementType>(MIGRATION_REPLACEMENT_ARGUMENT_TYPE_MULTI_RANDOMIZED, MIGRATION_REPLACEMENT_MULTI_RANDOMIZED));
	m_migration_replacement_types.insert(pair<const CString, EMigrationReplacementType>(MIGRATION_REPLACEMENT_ARGUMENT_TYPE_WORST, MIGRATION_REPLACEMENT_WORST));
	m_migration_replacement_types.insert(pair<const CString, EMigrationReplacementType>(MIGRATION_REPLACEMENT_ARGUMENT_TYPE_RANDOM, MIGRATION_REPLACEMENT_RANDOM));

	if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		m_migration_replacement_types.insert(pair<const CString, EMigrationReplacementType>(MIGRATION_REPLACEMENT_ARGUMENT_TYPE_BINARY_GENE_PATTERN_RANDOM, MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_RANDOM));
		m_migration_replacement_types.insert(pair<const CString, EMigrationReplacementType>(MIGRATION_REPLACEMENT_ARGUMENT_TYPE_BINARY_GENE_PATTERN_BEST, MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_BEST));
		m_migration_replacement_types.insert(pair<const CString, EMigrationReplacementType>(MIGRATION_REPLACEMENT_ARGUMENT_TYPE_BINARY_GENE_PATTERN_BEST_DETERMINISTIC, MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_BEST_DETERMINISTIC));
		m_migration_replacement_types.insert(pair<const CString, EMigrationReplacementType>(MIGRATION_REPLACEMENT_ARGUMENT_TYPE_BINARY_GENE_PATTERN_WORST, MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_WORST));
	}//if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	
	CEnumCommandParam<EMigrationReplacementType> p_type(MIGRATION_REPLACEMENT_ARGUMENT_TYPE, &m_migration_replacement_types, bIsObligatory);
	EMigrationReplacementType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case MIGRATION_REPLACEMENT_MULTI_DETERMINISTIC:
			{
				pc_migration_replacement = new CMultiDeterministicMigrationReplacement<TGenotype, TFenotype>(pcProblem);
				break;
			}//case MIGRATION_REPLACEMENT_MULTI_DETERMINISTIC
			case MIGRATION_REPLACEMENT_MULTI_RANDOMIZED:
			{
				pc_migration_replacement = new CMultiRandomizedMigrationReplacement<TGenotype, TFenotype>(pcProblem);
				break;
			}//case MIGRATION_REPLACEMENT_MULTI_RANDOMIZED
			case MIGRATION_REPLACEMENT_WORST:
			{
				pc_migration_replacement = new CWorstMigrationReplacement<TGenotype, TFenotype>(pcProblem);
				break;
			}//case MIGRATION_REPLACEMENT_WORST
			case MIGRATION_REPLACEMENT_RANDOM:
			{
				pc_migration_replacement = new CRandomMigrationReplacement<TGenotype, TFenotype>();
				break;
			}//case MIGRATION_REPLACEMENT_RANDOM
			case MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_RANDOM:
			{
				pc_migration_replacement = (CMigrationReplacement<TGenotype, TFenotype>*)new CBinaryGenePatternRandomMigrationReplacement<TFenotype>();
				break;
			}//case MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_RANDOM
			case MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_BEST:
			{
				pc_migration_replacement = (CMigrationReplacement<TGenotype, TFenotype>*)new CBinaryGenePatternBestMigrationReplacement<TFenotype>(false);
				break;
			}//case MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_BEST
			case MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_BEST_DETERMINISTIC:
			{
				pc_migration_replacement = (CMigrationReplacement<TGenotype, TFenotype>*)new CBinaryGenePatternBestMigrationReplacement<TFenotype>(true);
				break;
			}//case MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_BEST_DETERMINISTIC
			case MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_WORST:
			{
				pc_migration_replacement = (CMigrationReplacement<TGenotype, TFenotype>*)new CBinaryGenePatternWorstMigrationReplacement<TFenotype>();
				break;
			}//case MIGRATION_REPLACEMENT_BINARY_GENE_PATTERN_WORST
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "migration replacement");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_migration_replacement->eConfigure(plGenePatterns, psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_migration_replacement;
}//CMigrationReplacement<TGenotype, TFenotype> * MigrationReplacementUtils::pcGetMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)

template CMigrationReplacement<CBinaryCoding, CBinaryCoding> * MigrationReplacementUtils::pcGetMigrationReplacement(CProblem<CBinaryCoding, CBinaryCoding>*, CGenePatternList*, istream*, CError*, bool);
template CMigrationReplacement<CRealCoding, CRealCoding> * MigrationReplacementUtils::pcGetMigrationReplacement(CProblem<CRealCoding, CRealCoding>*, CGenePatternList*, istream*, CError*, bool);