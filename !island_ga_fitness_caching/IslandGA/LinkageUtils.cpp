#include "LinkageUtils.h"

#include "BinaryCoding.h"
#include "BinaryDifferentialLinkage.h"
#include "EnumCommandParam.h"
#include "RealCoding.h"

#include <atlstr.h>
#include <unordered_map>
#include <utility>

template <class TGenotype>
CLinkage<TGenotype> * LinkageUtils::pcGetLinkage(istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CLinkage<TGenotype> *pc_linkage = nullptr;

	size_t i_genotype_type_hash_code = typeid(TGenotype).hash_code();

	unordered_map<CString, ELinkageType> m_linkage_types;

	if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		m_linkage_types.insert(pair<const CString, ELinkageType>(LINKAGE_ARGUMENT_TYPE_BINARY_DIFFERENTIAL, LINKAGE_BINARY_DIFFERENTIAL));
	}//if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())

	CEnumCommandParam<ELinkageType> p_type(LINKAGE_ARGUMENT_TYPE, &m_linkage_types, bIsObligatory);
	ELinkageType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case LINKAGE_BINARY_DIFFERENTIAL:
			{
				pc_linkage = (CLinkage<TGenotype>*)new CBinaryDifferentialLinkage();
				break;
			}//case LINKAGE_BINARY_DIFFERENTIAL
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "linkage");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_linkage->eConfigure(psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_linkage;
}//CLinkage<TGenotype> * LinkageUtils::pcGetLinkage(istream *psSettings, CError *pcError, bool bIsObligatory)

template CLinkage<CBinaryCoding> * LinkageUtils::pcGetLinkage(istream*, CError*, bool);
template CLinkage<CRealCoding> * LinkageUtils::pcGetLinkage(istream*, CError*, bool);