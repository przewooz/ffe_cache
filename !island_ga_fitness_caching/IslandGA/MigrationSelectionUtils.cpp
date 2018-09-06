#include "MigrationSelectionUtils.h"

#include "BestMigrationSelection.h"
#include "BinaryCoding.h"
#include "EnumCommandParam.h"
#include "RealCoding.h"
#include "RandomMigrationSelection.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>

template<class TGenotype, class TFenotype>
CMigrationSelection<TGenotype, TFenotype> * MigrationSelectionUtils::pcGetMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CMigrationSelection<TGenotype, TFenotype> *pc_migration_selection = nullptr;

	unordered_map<CString, EMigrationSelectionType> m_migration_selection_types;
	m_migration_selection_types.insert(pair<const CString, EMigrationSelectionType>(MIGRATION_SELECTION_ARGUMENT_TYPE_BEST, MIGRATION_SELECTION_BEST));
	m_migration_selection_types.insert(pair<const CString, EMigrationSelectionType>(MIGRATION_SELECTION_ARGUMENT_TYPE_RANDOM, MIGRATION_SELECTION_RANDOM));
	m_migration_selection_types.insert(pair<const CString, EMigrationSelectionType>(MIGRATION_SELECTION_ARGUMENT_TYPE_WORST, MIGRATION_SELECTION_WORST));

	CEnumCommandParam<EMigrationSelectionType> p_type(MIGRATION_SELECTION_ARGUMENT_TYPE, &m_migration_selection_types, bIsObligatory);
	EMigrationSelectionType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case MIGRATION_SELECTION_BEST:
			{
				pc_migration_selection = new CBestMigrationSelection<TGenotype, TFenotype>(pcProblem);
				break;
			}//case MIGRATION_SELECTION_BEST
			case MIGRATION_SELECTION_RANDOM:
			{
				pc_migration_selection = new CRandomMigrationSelection<TGenotype, TFenotype>();
				break;
			}//case MIGRATION_SELECTION_RANDOM
			case MIGRATION_SELECTION_WORST:
			{
				pc_migration_selection = new CWorstMigrationSelection<TGenotype, TFenotype>(pcProblem);
				break;
			}//case MIGRATION_SELECTION_WORST
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "migration selection");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_migration_selection->eConfigure(psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_migration_selection;
}//CMigrationSelection<TGenotype, TFenotype> * MigrationSelectionUtils::pcGetMigrationSelection(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)

template CMigrationSelection<CBinaryCoding, CBinaryCoding> * MigrationSelectionUtils::pcGetMigrationSelection(CProblem<CBinaryCoding, CBinaryCoding>*, CGenePatternList*, istream*, CError*, bool);
template CMigrationSelection<CRealCoding, CRealCoding> * MigrationSelectionUtils::pcGetMigrationSelection(CProblem<CRealCoding, CRealCoding>*, CGenePatternList*, istream*, CError*, bool);