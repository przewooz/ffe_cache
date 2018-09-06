#include "GenePattern.h"

#include <algorithm>

CGenePattern::CGenePattern()
{
	pv_pattern = new vector<uint16_t>();
	pd_differences = new vector<double>();

	pv_nested_patterns = new vector<CGenePattern*>();
}//CGenePattern::CGenePattern()

CGenePattern::CGenePattern(uint16_t iSignificantIndex)
	: CGenePattern()
{
	i_significant_index = iSignificantIndex;
}//CGenePattern::CGenePattern(uint16_t iSignificantIndex)

CGenePattern::~CGenePattern()
{
	pv_pattern->clear();
	delete pv_pattern;

	pd_differences->clear();
	delete pd_differences;

	for (uint32_t i = 0; i < (uint32_t)pv_nested_patterns->size(); i++)
	{
		delete pv_nested_patterns->at(i);
	}//for (uint32_t i = 0; i < (uint32_t)pv_nested_patterns->size(); i++)

	pv_nested_patterns->clear();
	delete pv_nested_patterns;
}//CGenePattern::~CGenePattern()

void CGenePattern::vAdd(uint16_t iIndex, double dDifference)
{
	pv_pattern->push_back(iIndex);
	pd_differences->push_back(dDifference);
}//void CGenePattern::vAdd(uint16_t iIndex, double dDifference)

void CGenePattern::vPopNestedPattern()
{
	delete pv_nested_patterns->at(pv_nested_patterns->size() - 1);
	pv_nested_patterns->pop_back();
}//void CGenePattern::vPopNestedPattern()

#include <iostream>

void CGenePattern::vSortNestedPatterns()
{
	sort(pv_nested_patterns->begin(), pv_nested_patterns->end(), [](CGenePattern *pcGenePattern0, CGenePattern *pcGenePattern1)
	{
		return pcGenePattern0->iGetSize() < pcGenePattern1->iGetSize();
	});//sort(pv_nested_patterns->begin(), pv_nested_patterns->end(), [](CGenePattern *pcGenePattern0, CGenePattern *pcGenePattern1)
}//void CGenePattern::vSortNestedPatterns()

CString CGenePattern::sToString()
{
	CString s_pattern;

	for (uint16_t i = 0; i < (uint16_t)pv_pattern->size(); i++)
	{
		s_pattern.AppendFormat("%d ", pv_pattern->at(i));
	}//for (uint16_t i = 0; i < (uint16_t)pv_pattern->size(); i++)

	return s_pattern;
}//CString CGenePattern::sToString()