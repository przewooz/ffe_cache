#include "GenePatternList.h"

#include "RandUtils.h"

CGenePatternList::CGenePatternList(uint32_t iMaxSize, CGenePatternReplacement *pcGenePatternReplacement)
{
	i_max_size = iMaxSize;
	pc_gene_pattern_replacement = pcGenePatternReplacement;

	i_number_of_gene_patterns = 0;
	ppc_gene_patterns = new CGenePattern*[iMaxSize];

	*(ppc_gene_patterns + 0) = nullptr;
}//CGenePatternList::CGenePatternList(uint32_t iNumberOfGenePatterns, CGenePatternReplacement *pcGenePatternReplacement)

CGenePatternList::~CGenePatternList()
{
	for (uint32_t i = 0; i < i_number_of_gene_patterns; i++)
	{
		delete *(ppc_gene_patterns + i);
	}//for (uint32_t i = 0; i < i_number_of_gene_patterns; i++)

	delete ppc_gene_patterns;
	delete pc_gene_pattern_replacement;
}//CGenePatternList::~CGenePatternList()

#include <iostream>
using namespace std;

void CGenePatternList::vAdd(CGenePattern *pcGenePattern)
{
	if (i_number_of_gene_patterns < i_max_size)
	{
		*(ppc_gene_patterns + i_number_of_gene_patterns) = pcGenePattern;
		i_number_of_gene_patterns++;
	}//if (i_number_of_gene_patterns < i_max_size)
	else 
	{
		pc_gene_pattern_replacement->vReplace(pcGenePattern, i_number_of_gene_patterns, ppc_gene_patterns);
	}//else if (i_number_of_gene_patterns < i_max_size)

		/*cout << endl;

	for (int j = 0; j < i_number_of_gene_patterns; j++)
	{
		pcGenePattern = *(ppc_gene_patterns + j);

		cout << endl;

			for (int i = 0; i < pcGenePattern->iGetSize(); i++)
			{
				cout << *(pcGenePattern->piGetPattern() + i) << " ";
			}

			cout << endl;
	}



	cout << endl;
	cout << i_number_of_gene_patterns << endl << endl;*/

}//void CGenePatternList::vAdd(CGenePattern *pcGenePattern)

CGenePattern * CGenePatternList::pcGetRandomGenePattern()
{
	uint32_t i_random_index;

	return pcGetRandomGenePattern(&i_random_index);
}//CGenePattern * CGenePatternList::pcGetRandomGenePattern()

CGenePattern * CGenePatternList::pcGetRandomGenePattern(uint32_t *piRandomIndex)
{
	*piRandomIndex = RandUtils::iRandIndex(i_number_of_gene_patterns);

	//cout << endl;

	//CGenePattern *pc_gene_pattern = *(ppc_gene_patterns + *piRandomIndex);

	//for (int i = 0; i < pc_gene_pattern->iGetSize(); i++)
	//{
	//	cout << *(pc_gene_pattern->piGetPattern() + i) << " ";
	//}

	//cout << endl;

	return *(ppc_gene_patterns + *piRandomIndex);
}//CGenePattern * CGenePatternList::pcGetRandomGenePattern(uint32_t *piRandomIndex)