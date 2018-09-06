#include "SelectionUtils.h"

#include "BinaryCoding.h"
#include "EnumCommandParam.h"
#include "GenePatternList.h"
#include "RealCoding.h"
#include "Selection.h"

#include <atlstr.h>
#include <cstdint>
#include <unordered_map>

template <class TGenotype, class TFenotype>
CSelection<TGenotype, TFenotype> * SelectionUtils::pcGetSelection(CProblem<TGenotype, TFenotype> *pcProblem, CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CSelection<TGenotype, TFenotype> *pc_selection = nullptr;

	unordered_map<CString, ESelectionType> m_selection_types;
	m_selection_types.insert(pair<const CString, ESelectionType>(SELECTION_ARGUMENT_TYPE_TOURNAMENT, SELECTION_TOURNAMENT));
	m_selection_types.insert(pair<const CString, ESelectionType>(SELECTION_ARGUMENT_TYPE_GENE_PATTERN_TOURNAMENT, SELECTION_GENE_PATTERN_TOURNAMENT));

	CEnumCommandParam<ESelectionType> p_type(SELECTION_ARGUMENT_TYPE, &m_selection_types, bIsObligatory);
	ESelectionType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case SELECTION_TOURNAMENT:
			{
				pc_selection = new CTournamentSelection<TGenotype, TFenotype>(pcProblem);
				break;
			}//case SELECTION_TOURNAMENT
			case SELECTION_GENE_PATTERN_TOURNAMENT:
			{
				pc_selection = new CGenePatternTournamentSelection<TGenotype, TFenotype>(pcProblem);
				break;
			}//case SELECTION_GENE_PATTERN_TOURNAMENT
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "selection");
				break;
			}//default
		}//switch (e_type)

		if (!*pcError)
		{
			*pcError = pc_selection->eConfigure(plGenePatterns, psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_selection;
}//CSelection<TGenotype, TFenotype> * SelectionUtils::pcGetSelection(CProblem<TGenotype, TFenotype> *pcProblem,CGenePatternList *plGenePatterns, istream *psSettings, CError *pcError, bool bIsObligatory)

template CSelection<CBinaryCoding, CBinaryCoding> * SelectionUtils::pcGetSelection(CProblem<CBinaryCoding, CBinaryCoding>*, CGenePatternList*, istream*, CError*, bool);
template CSelection<CRealCoding, CRealCoding> * SelectionUtils::pcGetSelection(CProblem<CRealCoding, CRealCoding>*, CGenePatternList*, istream*, CError*, bool);