#include "Evaluation.h"

#include "BinaryCoding.h"
#include "EvaluationUtils.h"
#include "RealCoding.h"
#include "UIntCommandParam.h"

#include <algorithm>
#include <atlstr.h>
#include <cfloat>
#include <utility>

template <class TFenotype>
uint32_t CEvaluation<TFenotype>::iERROR_PARENT_CEVALUATION = CError::iADD_ERROR_PARENT("CEvaluation");

template <class TFenotype>
CEvaluation<TFenotype>::CEvaluation()
{
	v_init(0, DBL_MAX);
}//CEvaluation<TFenotype>::CEvaluation()

template <class TFenotype>
CEvaluation<TFenotype>::CEvaluation(uint16_t iNumberOfElements, double dMaxValue)
{
	v_init(iNumberOfElements, dMaxValue);
}//CEvaluation<TFenotype>::CEvaluation(uint16_t iNumberOfElements, double dMaxValue)

template <class TFenotype>
CEvaluation<TFenotype>::~CEvaluation()
{

}//CEvaluation<TFenotype>::~CEvaluation()

template <class TFenotype>
double CEvaluation<TFenotype>::dEvaluate(TFenotype *pcFenotype)
{
	i_ffe++;

	return d_evaluate(pcFenotype, 0);
}//double CEvaluation<TFenotype>::dEvaluate(TFenotype *pcFenotype)

template <class TFenotype>
void CEvaluation<TFenotype>::v_init(uint16_t iNumberOfElements, double dMaxValue)
{
	i_ffe = 0;
	i_number_of_elements = iNumberOfElements;
	d_max_value = dMaxValue;
}//void CEvaluation<TFenotype>::v_init(uint16_t iNumberOfElements, double dMaxValue)


template <class TFenotype>
CInversedEvaluation<TFenotype>::CInversedEvaluation(CEvaluation<TFenotype> *pcEvaluation)
	: CEvaluation<TFenotype>(pcEvaluation->iGetNumberOfElements(), -pcEvaluation->dGetMaxValue())
{
	pc_evaluation = pcEvaluation;
}//CInversedEvaluation<TFenotype>::CInversedEvaluation(CEvaluation<TFenotype> *pcEvaluation)

template <class TFenotype>
CInversedEvaluation<TFenotype>::~CInversedEvaluation()
{
	delete pc_evaluation;
}//CInversedEvaluation<TFenotype>::~CInversedEvaluation()

template <class TFenotype>
double CInversedEvaluation<TFenotype>::d_evaluate(TFenotype * pcFenotype, uint16_t iShift)
{
	return -pc_evaluation->d_evaluate(pcFenotype, iShift);
}//double CInversedEvaluation<TFenotype>::d_evaluate(TFenotype * pcFenotype, uint16_t iShift)


template <class TFenotype>
CConcatenationEvaluation<TFenotype>::CConcatenationEvaluation()
	: CEvaluation<TFenotype>(0, 0)
{
	pc_sample_fenotype = nullptr;
}//CConcatenationEvaluation<TFenotype>::CConcatenationEvaluation()

template <class TFenotype>
CConcatenationEvaluation<TFenotype>::~CConcatenationEvaluation()
{
	v_clear();
}//CConcatenationEvaluation<TFenotype>::~CConcatenationEvaluation()

template <class TFenotype>
CError CConcatenationEvaluation<TFenotype>::eConfigure(istream *psSettings)
{
	v_clear();

	CError c_error = CEvaluation<TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_number_of_components(EVALUATION_ARGUMENT_NUMBER_OF_COMPONENTS, 1, UINT16_MAX);
		uint16_t i_number_of_components = p_number_of_components.iGetValue(psSettings, &c_error);

		CEvaluation<TFenotype> *pc_component = nullptr;

		for (uint16_t i = 0; i < i_number_of_components && !c_error; i++)
		{
			CUIntCommandParam p_component_multiplier(EVALUATION_ARGUMENT_COMPONENT_MULTIPLIER, 1, UINT16_MAX);
			uint16_t i_component_multiplier = p_component_multiplier.iGetValue(psSettings, &c_error);

			if (!c_error)
			{
				pc_component = EvaluationUtils::pcGetEvaluation<TFenotype>(psSettings, &c_error);
			}//if (!c_error)

			if (!c_error)
			{
				v_add_component(pc_component, i_component_multiplier);
			}//if (!c_error)
		}//for (uint16_t i = 0; i < i_number_of_components && !c_error; i++)
	}//if (!c_error)

	return c_error;
}//CError CConcatenationEvaluation<TFenotype>::eConfigure(istream *psSettings)

template <class TFenotype>
TFenotype * CConcatenationEvaluation<TFenotype>::pcCreateSampleFenotype()
{
	if (!pc_sample_fenotype)
	{
		if (!v_components.empty())
		{
			pc_sample_fenotype = v_components.front()->pcCreateSampleFenotype();

			TFenotype *pc_other_sample_fenotype;

			for (uint16_t i = 1; i < (uint16_t)v_components.size(); i++)
			{
				pc_other_sample_fenotype = v_components.at(i)->pcCreateSampleFenotype();
				pc_sample_fenotype->vMerge(pc_other_sample_fenotype);
				delete pc_other_sample_fenotype;
			}//for (uint16_t i = 1; i < (uint16_t)v_components.size(); i++)
		}//if (!v_components.empty())
	}//if (!pc_sample_fenotype)

	return pc_sample_fenotype ? new TFenotype(pc_sample_fenotype) : nullptr;
}//TFenotype * CConcatenationEvaluation<TFenotype>::pcCreateSampleFenotype()

template <class TFenotype>
double CConcatenationEvaluation<TFenotype>::d_evaluate(TFenotype *pcFenotype, uint16_t iShift)
{
	double d_fitness_value = 0;

	CEvaluation<TFenotype> *pc_component;

	for (uint16_t i = 0; i < (uint16_t)v_components.size(); i++)
	{
		pc_component = v_components.at(i);

		d_fitness_value += pc_component->d_evaluate(pcFenotype, iShift);
		iShift += pc_component->iGetNumberOfElements();
	}//for (uint16_t i = 0; i < (uint16_t)v_components.size(); i++)

	return d_fitness_value;
}//double CConcatenationEvaluation<TFenotype>::d_evaluate(TFenotype *pcFenotype, uint16_t iShift)

template <class TFenotype>
void CConcatenationEvaluation<TFenotype>::v_clear_sample_fenotype()
{
	delete pc_sample_fenotype;
	pc_sample_fenotype = nullptr;
}//void CConcatenationEvaluation<TFenotype>::v_clear_sample_fenotype()

template <class TFenotype>
void CConcatenationEvaluation<TFenotype>::v_clear()
{
	if (!v_components.empty())
	{
		CEvaluation<TFenotype> *pc_previous_component = v_components.at(0);

		for (uint16_t i = 1; i < (uint16_t)v_components.size(); i++)
		{
			if (pc_previous_component != v_components.at(i))
			{
				delete pc_previous_component;
				pc_previous_component = v_components.at(i);
			}//if (pc_previous_component != v_components.at(i))
		}//for (uint16_t i = 1; i < (uint16_t)v_components.size(); i++)

		delete pc_previous_component;
	}//if (!v_components.empty())

	v_components.clear();

	v_clear_sample_fenotype();
}//void CConcatenationEvaluation<TFenotype>::v_clear()

template <class TFenotype>
void CConcatenationEvaluation<TFenotype>::v_add_component(CEvaluation<TFenotype> *pcComponent, uint16_t iMultiplier)
{
	i_number_of_elements += iMultiplier * pcComponent->iGetNumberOfElements();
	d_max_value += iMultiplier * pcComponent->dGetMaxValue();

	for (uint16_t i = 0; i < iMultiplier; i++)
	{
		v_components.push_back(pcComponent);
	}//for (uint16_t i = 0; i < iMultiplier; i++)

	v_clear_sample_fenotype();
}//void CConcatenationEvaluation<TFenotype>::v_add_component(CEvaluation<TFenotype> *pcComponent, uint16_t iMultiplier)


template <class TFenotype>
CCyclicFenotypeEvaluation<TFenotype>::~CCyclicFenotypeEvaluation()
{
	v_clear();
}//CCyclicFenotypeEvaluation<TFenotype>::~CCyclicFenotypeEvaluation()

template <class TFenotype>
CError CCyclicFenotypeEvaluation<TFenotype>::eConfigure(istream *psSettings)
{
	v_clear();

	CError c_error = CEvaluation<TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		pc_actual_evaluation = EvaluationUtils::pcGetEvaluation<TFenotype>(psSettings, &c_error);

		if (!c_error)
		{
			i_number_of_elements = pc_actual_evaluation->i_number_of_elements;
			d_max_value = pc_actual_evaluation->d_max_value;

			pc_evaluation_fenotype = pc_actual_evaluation->pcCreateSampleFenotype();
		}//if (!c_error)
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_shift(EVALUATION_ARGUMENT_SHIFT, 1, i_number_of_elements - 1);
		i_cyclic_shift = p_shift.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CCyclicFenotypeEvaluation<TFenotype>::eConfigure(istream *psSettings)

template <class TFenotype>
double CCyclicFenotypeEvaluation<TFenotype>::d_evaluate(TFenotype *pcFenotype, uint16_t iShift)
{
	v_prepare_evaluation_fenotype(pcFenotype, iShift);
	return pc_actual_evaluation->d_evaluate(pc_evaluation_fenotype, iShift);
}//double CCyclicFenotypeEvaluation<TFenotype>::d_evaluate(TFenotype *pcFenotype, uint16_t iShift)

template <class TFenotype>
void CCyclicFenotypeEvaluation<TFenotype>::v_clear()
{
	delete pc_actual_evaluation;
	pc_actual_evaluation = nullptr;

	delete pc_evaluation_fenotype;
	pc_evaluation_fenotype = nullptr;
}//void CCyclicFenotypeEvaluation<TFenotype>::v_clear()


template <class TFenotype>
CCooperativeCoevolutionEvaluation<TFenotype>::CCooperativeCoevolutionEvaluation(CEvaluation<TFenotype> *pcActualEvaluation)
{
	i_number_of_elements = 0;
	d_max_value = pcActualEvaluation->d_max_value;

	pc_evaluation_fenotype = pcActualEvaluation->pcCreateSampleFenotype();
	pc_complementary_fenotype = nullptr;

	pc_actual_evaluation = pcActualEvaluation;
}//CCooperativeCoevolutionEvaluation<TFenotype>::CCooperativeCoevolutionEvaluation(CEvaluation<TFenotype> *pcActualEvaluation)

template <class TFenotype>
CCooperativeCoevolutionEvaluation<TFenotype>::~CCooperativeCoevolutionEvaluation()
{
	delete pc_evaluation_fenotype;
	vSetComplementaryFenotype(nullptr);
}//CCooperativeCoevolutionEvaluation<TFenotype>::~CCooperativeCoevolutionEvaluation()

template <class TFenotype>
void CCooperativeCoevolutionEvaluation<TFenotype>::vAddGeneIndex(uint16_t iGeneIndex)
{
	v_genes_indexes.push_back(iGeneIndex);
	i_number_of_elements++;
}//void CCooperativeCoevolutionEvaluation<TFenotype>::vAddGeneIndex(uint16_t iGeneIndex)

template <class TFenotype>
bool CCooperativeCoevolutionEvaluation<TFenotype>::bRemoveGeneIndex(uint16_t iGeneIndex)
{
	bool b_removed = false;

	vector<uint16_t>::iterator i_end = v_genes_indexes.end();

	vector<uint16_t>::iterator i_it = find(v_genes_indexes.begin(), i_end, iGeneIndex);

	if (i_it != i_end)
	{
		v_genes_indexes.erase(i_it);
		i_number_of_elements--;
		b_removed = true;
	}//if (i_it != i_end)

	return b_removed;
}//bool CCooperativeCoevolutionEvaluation<TFenotype>::bRemoveGeneIndex(uint16_t iGeneIndex)

template <class TFenotype>
void CCooperativeCoevolutionEvaluation<TFenotype>::vClearGenesIndexes()
{
	v_genes_indexes.clear();
	i_number_of_elements = 0;
}//void CCooperativeCoevolutionEvaluation<TFenotype>::vClearGenesIndexes()

template <class TFenotype>
void CCooperativeCoevolutionEvaluation<TFenotype>::vSetComplementaryFenotype(TFenotype *pcComplementaryFenotype)
{
	delete pc_complementary_fenotype;

	if (pcComplementaryFenotype)
	{
		pc_complementary_fenotype = new TFenotype(pcComplementaryFenotype);
	}//if (pcComplementaryFenotype)
	else
	{
		pc_complementary_fenotype = nullptr;
	}//else if (pcComplementaryFenotype)
}//void CCooperativeCoevolutionEvaluation<TFenotype>::vSetComplementaryFenotype(TFenotype *pcComplementaryFenotype)

template <class TFenotype>
double CCooperativeCoevolutionEvaluation<TFenotype>::d_evaluate(TFenotype *pcFenotype, uint16_t iShift)
{
	v_prepare_evaluation_fenotype(pcFenotype);
	return pc_actual_evaluation->d_evaluate(pc_evaluation_fenotype, iShift);
}//double CCooperativeCoevolutionEvaluation<TFenotype>::d_evaluate(TFenotype *pcFenotype, uint16_t iShift)


template <class TFenotype>
CDynamicEvaluation<TFenotype>::CDynamicEvaluation()
{
	i_current_state_index = 0;
}//CDynamicEvaluation<TFenotype>::CDynamicEvaluation()

template <class TFenotype>
CDynamicEvaluation<TFenotype>::~CDynamicEvaluation()
{
	v_clear();
}//CDynamicEvaluation<TFenotype>::~CDynamicEvaluation()

template <class TFenotype>
CError CDynamicEvaluation<TFenotype>::eConfigure(istream *psSettings)
{
	v_clear();

	CError c_error = CEvaluation<TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_number_of_states(EVALUATION_ARGUMENT_NUMBER_OF_STATES, 1, UINT16_MAX);
		uint16_t i_number_of_states = p_number_of_states.iGetValue(psSettings, &c_error);

		v_states.reserve(i_number_of_states);

		CEvaluation<TFenotype> *pc_evaluation = nullptr;
		CEvaluation<TFenotype> *pc_previous_evaluation = nullptr;

		for (uint16_t i = 0; i < i_number_of_states && !c_error; i++)
		{
			pc_evaluation = EvaluationUtils::pcGetEvaluation<TFenotype>(psSettings, &c_error);
			v_states.push_back(pc_evaluation);

			if (!c_error && pc_previous_evaluation)
			{
				if (pc_evaluation->iGetNumberOfElements() != pc_previous_evaluation->iGetNumberOfElements())
				{
					CString s_message;
					s_message.Format("index: %u; expected number of elements: %d; found number of elements: %d",
						i, pc_previous_evaluation->iGetNumberOfElements(), pc_evaluation->iGetNumberOfElements());

					c_error.vSetError(CError::iERROR_CODE_SYSTEM_ARGUMENT_WRONG_NUMBER_OF_EVALUATION_ELEMENTS, s_message);
				}//if (pc_evaluation->iGetNumberOfElements() != pc_previous_evaluation->iGetNumberOfElements())
				
				if (!c_error)
				{
					if (typeid(TFenotype).hash_code() == typeid(CRealCoding).hash_code())
					{
						CRealCoding *pc_sample_fenotype = (CRealCoding*)pc_evaluation->pcCreateSampleFenotype();
						CRealCoding *pc_previous_sample_fenotype = (CRealCoding*)pc_previous_evaluation->pcCreateSampleFenotype();

						for (uint16_t j = 0; j < pc_sample_fenotype->iGetNumberOfDimensions() && !c_error; j++)
						{
							if (*(pc_sample_fenotype->pdGetMinValues() + j) != *(pc_previous_sample_fenotype->pdGetMinValues() + j))
							{
								CString s_message;
								s_message.Format("evaluation index: %u; dimension index: %u; expected min value: %f; found min value: %f",
									i, j, *(pc_sample_fenotype->pdGetMinValues() + j), *(pc_previous_sample_fenotype->pdGetMinValues() + j));

								c_error.vSetError(CError::iERROR_CODE_SYSTEM_ARGUMENT_WRONG_VALUE, s_message);
							}//if (*(pc_sample_fenotype->pdGetMinValues() + j) != *(pc_previous_sample_fenotype->pdGetMinValues() + j))

							if (*(pc_sample_fenotype->pdGetMaxValues() + j) != *(pc_previous_sample_fenotype->pdGetMaxValues() + j))
							{
								CString s_message;
								s_message.Format("evaluation index: %u; dimension index: %u; expected max value: %f; found max value: %f",
									i, j, *(pc_sample_fenotype->pdGetMaxValues() + j), *(pc_previous_sample_fenotype->pdGetMaxValues() + j));

								c_error.vSetError(CError::iERROR_CODE_SYSTEM_ARGUMENT_WRONG_VALUE, s_message);
							}//if (*(pc_sample_fenotype->pdGetMaxValues() + j) != *(pc_previous_sample_fenotype->pdGetMaxValues() + j))
						}//for (uint16_t j = 0; j < pc_sample_fenotype->iGetNumberOfDimensions() && !c_error; j++)

						delete pc_sample_fenotype;
						delete pc_previous_sample_fenotype;
					}//if (typeid(TFenotype).hash_code() == typeid(CRealCoding).hash_code())
				}//if (!c_error)

				pc_previous_evaluation = pc_evaluation;
			}//if (!c_error && pc_previous_evaluation)
		}//for (uint16_t i = 0; i < i_number_of_states && !c_error; i++)

		if (!c_error)
		{
			v_max_values.reserve(v_states.size());

			for (uint16_t i = 0; i < (uint16_t)v_states.size(); i++)
			{
				v_max_values.push_back(v_states.at(i)->dGetMaxValue());
			}//for (uint16_t i = 0; i < (uint16_t)v_states.size(); i++)

			d_max_value = v_max_values.front();
			i_number_of_elements = v_states.front()->iGetNumberOfElements();
		}//if (!c_error)
	}//if (!c_error)

	return c_error;
}//CError CDynamicEvaluation<TFenotype>::eConfigure(istream *psSettings)

template <class TFenotype>
void CDynamicEvaluation<TFenotype>::vChange()
{
	i_current_state_index = (i_current_state_index + 1) % (uint16_t)v_states.size();
	d_max_value = v_max_values.at(i_current_state_index);
}//void CDynamicEvaluation<TFenotype>::vChange()

template <class TFenotype>
double CDynamicEvaluation<TFenotype>::d_evaluate(TFenotype *pcFenotype, uint16_t iShift)
{
	return v_states.at(i_current_state_index)->d_evaluate(pcFenotype, iShift);
}//double CDynamicEvaluation<TFenotype>::d_evaluate(TFenotype *pcFenotype, uint16_t iShift)

template <class TFenotype>
void CDynamicEvaluation<TFenotype>::v_clear()
{
	for (uint16_t i = 0; i < (uint16_t)v_states.size(); i++)
	{
		delete v_states.at(i);
	}//for (uint16_t i = 0; i < (uint16_t)v_states.size(); i++)

	v_states.clear();
	v_max_values.clear();
}//void CDynamicEvaluation<TFenotype>::v_clear()


template class CEvaluation<CBinaryCoding>;
template class CEvaluation<CRealCoding>;

template class CInversedEvaluation<CBinaryCoding>;
template class CInversedEvaluation<CRealCoding>;

template class CConcatenationEvaluation<CBinaryCoding>;
template class CConcatenationEvaluation<CRealCoding>;

template class CCyclicFenotypeEvaluation<CBinaryCoding>;
template class CCyclicFenotypeEvaluation<CRealCoding>;

template class CCooperativeCoevolutionEvaluation<CBinaryCoding>;
template class CCooperativeCoevolutionEvaluation<CRealCoding>;

template class CDynamicEvaluation<CBinaryCoding>;
template class CDynamicEvaluation<CRealCoding>;