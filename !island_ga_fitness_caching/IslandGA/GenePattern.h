#ifndef GENE_PATTERN_H
#define GENE_PATTERN_H

#include <atlstr.h>
#include <cstdint>
#include <vector>

using namespace std;

class CGenePattern
{
public:
	CGenePattern();
	CGenePattern(uint16_t iSignificantIndex);

	~CGenePattern();

	void vAdd(uint16_t iIndex) { vAdd(iIndex, 0); };
	void vAdd(uint16_t iIndex, double dDifference);

	void vAddNestedPattern(CGenePattern *pcNestedPattern) { pv_nested_patterns->push_back(pcNestedPattern); };
	void vPopNestedPattern();
	void vSortNestedPatterns();

	uint16_t iGetSignificantIndex() { return i_significant_index; };

	uint16_t iGetSize() { return (uint16_t)pv_pattern->size(); };
	uint16_t *piGetPattern() { return pv_pattern->data(); };
	double *pdGetDifferences() { return pd_differences->data(); };

	uint32_t iGetNestedPatternsSize() { return (uint32_t)pv_nested_patterns->size(); };
	CGenePattern **ppcGetNestedPatterns() { return pv_nested_patterns->data(); };

	CString sToString();

private:
	uint16_t i_significant_index;

	vector<uint16_t> *pv_pattern;
	vector<double> *pd_differences;
	
	vector<CGenePattern*> *pv_nested_patterns;
};//class CGenePattern

#endif//GENE_PATTERN_H