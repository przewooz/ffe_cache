#include "MigrationTopologyUtils.h"

#include "BinaryCoding.h"
#include "EnumCommandParam.h"
#include "GenePatternList.h"
#include "MigrationTopology.h"
#include "RealCoding.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>

template <class TGenotype, class TFenotype>
CMigrationTopology<TGenotype, TFenotype> * MigrationTopologyUtils::pcGetMigrationTopology(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CMigrationTopology<TGenotype, TFenotype> *pc_migration_topology = nullptr;

	unordered_map<CString, EMigrationTopologyType> m_migration_topology_types;
	m_migration_topology_types.insert(pair<const CString, EMigrationTopologyType>(MIGRATION_TOPOLOGY_ARGUMENT_TYPE_RANDOM, MIGRATION_TOPOLOGY_RANDOM));
	m_migration_topology_types.insert(pair<const CString, EMigrationTopologyType>(MIGRATION_TOPOLOGY_ARGUMENT_TYPE_GENE_PATTERN_MIN_DISTANCE, MIGRATION_TOPOLOGY_GENE_PATTERN_MIN_DISTANCE));
	m_migration_topology_types.insert(pair<const CString, EMigrationTopologyType>(MIGRATION_TOPOLOGY_ARGUMENT_TYPE_GENE_PATTERN_MAX_DISTANCE, MIGRATION_TOPOLOGY_GENE_PATTERN_MAX_DISTANCE));
	m_migration_topology_types.insert(pair<const CString, EMigrationTopologyType>(MIGRATION_TOPOLOGY_ARGUMENT_TYPE_RING, MIGRATION_TOPOLOGY_RING));

	CEnumCommandParam<EMigrationTopologyType> p_type(MIGRATION_TOPOLOGY_ARGUMENT_TYPE, &m_migration_topology_types, bIsObligatory);
	EMigrationTopologyType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case MIGRATION_TOPOLOGY_RANDOM:
			{
				pc_migration_topology = new CRandomMigrationTopology<TGenotype, TFenotype>();
				break;
			}//case MIGRATION_TOPOLOGY_RANDOM
			case MIGRATION_TOPOLOGY_GENE_PATTERN_MIN_DISTANCE:
			{
				pc_migration_topology = new CGenePatternDistanceTopology<TGenotype, TFenotype>(true);
				break;
			}// case MIGRATION_TOPOLOGY_GENE_PATTERN_MIN_DISTANCE
			case MIGRATION_TOPOLOGY_GENE_PATTERN_MAX_DISTANCE:
			{
				pc_migration_topology = new CGenePatternDistanceTopology<TGenotype, TFenotype>(false);
				break;
			}// case MIGRATION_TOPOLOGY_GENE_PATTERN_MAX_DISTANCE
			case MIGRATION_TOPOLOGY_RING:
			{
				pc_migration_topology = new CRingMigrationTopology<TGenotype, TFenotype>();
				break;
			}//case MIGRATION_TOPOLOGY_RING
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "migration topology");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_migration_topology->eConfigure(plGenePatterns, psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_migration_topology;
}//CMigrationTopology<TGenotype, TFenotype> * MigrationTopologyUtils::pcGetMigrationTopology(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)

template CMigrationTopology<CBinaryCoding, CBinaryCoding> * MigrationTopologyUtils::pcGetMigrationTopology(CProblem<CBinaryCoding, CBinaryCoding>*, CGenePatternList*, istream*, CError*, bool);
template CMigrationTopology<CRealCoding, CRealCoding> * MigrationTopologyUtils::pcGetMigrationTopology(CProblem<CRealCoding, CRealCoding>*, CGenePatternList*, istream*, CError*, bool);
