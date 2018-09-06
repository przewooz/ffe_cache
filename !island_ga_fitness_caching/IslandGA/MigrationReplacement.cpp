#include "MigrationReplacement.h"

#include "CommandParam.h"
#include "GenePattern.h"
#include "MigrationReplacementUtils.h"
#include "RandUtils.h"
#include "RealCoding.h"
#include "UIntCommandParam.h"

#include <algorithm>
#include <functional>

using namespace std;

template <class TGenotype, class TFenotype>
CMigrationReplacement<TGenotype, TFenotype>::CMigrationReplacement()
{
	v_init(nullptr);
}//CMigrationReplacement<TGenotype, TFenotype>::CMigrationReplacement()

template <class TGenotype, class TFenotype>
CMigrationReplacement<TGenotype, TFenotype>::CMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem)
{
	v_init(pcProblem);
}//CMigrationReplacement<TGenotype, TFenotype>::CMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CMigrationReplacement<TGenotype, TFenotype>::~CMigrationReplacement()
{

}//CMigrationReplacement<TGenotype, TFenotype>::~CMigrationReplacement()

template <class TGenotype, class TFenotype>
void CMigrationReplacement<TGenotype, TFenotype>::v_init(CProblem<TGenotype, TFenotype> *pcProblem)
{
	pc_problem = pcProblem;
}//void CMigrationReplacement<TGenotype, TFenotype>::v_init(CProblem<TGenotype, TFenotype> *pcProblem)


template <class TGenotype, class TFenotype>
CMultiMigrationReplacement<TGenotype, TFenotype>::CMultiMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem)
	: CMigrationReplacement<TGenotype, TFenotype>(pcProblem)
{

}//CMultiMigrationReplacement<TGenotype, TFenotype>::CMultiMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CMultiMigrationReplacement<TGenotype, TFenotype>::~CMultiMigrationReplacement()
{
	v_clear();
}//CMultiMigrationReplacement<TGenotype, TFenotype>::~CMultiMigrationReplacement()

template <class TGenotype, class TFenotype>
CError CMultiMigrationReplacement<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)
{
	v_clear();

	CError c_error = CMigrationReplacement<TGenotype, TFenotype>::eConfigure(plGenePatterns, psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_number_of_migration_replacements(MIGRATION_REPLACEMENT_ARGUMENT_NUMBER_OF_MIGRATION_REPLACEMENTS, 1, UINT8_MAX);
		uint8_t i_number_of_migration_replacements = p_number_of_migration_replacements.iGetValue(psSettings, &c_error);

		if (!c_error)
		{
			v_migration_replacements.reserve(i_number_of_migration_replacements);

			for (uint8_t i = 0; i < i_number_of_migration_replacements && !c_error; i++)
			{
				v_migration_replacements.push_back(MigrationReplacementUtils::pcGetMigrationReplacement(pc_problem, plGenePatterns, psSettings, &c_error));
			}//for (uint8_t i = 0; i < i_number_of_migration_replacements && !c_error; i++)
		}//if (!c_error)
	}//if (!c_error)

	return c_error;
}//CError CMultiMigrationReplacement<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)

template <class TGenotype, class TFenotype>
void CMultiMigrationReplacement<TGenotype, TFenotype>::v_clear()
{
	for (uint8_t i = 0; i < (uint8_t)v_migration_replacements.size(); i++)
	{
		delete v_migration_replacements.at(i);
	}//for (uint8_t i = 0; i < (uint8_t)v_migration_replacements.size(); i++)

	v_migration_replacements.clear();
}//void CMultiMigrationReplacement<TGenotype, TFenotype>::v_clear()


template <class TGenotype, class TFenotype>
CMultiDeterministicMigrationReplacement<TGenotype, TFenotype>::CMultiDeterministicMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem)
	: CMultiMigrationReplacement<TGenotype, TFenotype>(pcProblem)
{
	i_current_migration_replacement_index = 0;
}//CMultiDeterministicMigrationReplacement<TGenotype, TFenotype>::CMultiDeterministicMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CError CMultiDeterministicMigrationReplacement<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)
{
	i_current_migration_replacement_index = 0;

	CError c_error;

	CBoolCommandParam p_each_different(MIGRATION_REPLACEMENT_ARGUMENT_EACH_DIFFERENT);
	b_each_different = p_each_different.bGetValue(psSettings, &c_error);

	if (!c_error)
	{
		c_error = CMultiMigrationReplacement<TGenotype, TFenotype>::eConfigure(plGenePatterns, psSettings);
	}//if (!c_error)

	return c_error;
}//CError CMultiDeterministicMigrationReplacement<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)

template <class TGenotype, class TFenotype>
void CMultiDeterministicMigrationReplacement<TGenotype, TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)
{
	if (b_each_different)
	{
		v_set_next_index();
	}//if (b_each_different)

	v_migration_replacements.at(i_current_migration_replacement_index)->vReplace(iPopulationSize, ppcPopulation, iNumberOfMigratingIndividuals, ppcMigratingIndividuals);
}//void CMultiDeterministicMigrationReplacement<TGenotype, TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)

template <class TGenotype, class TFenotype>
void CMultiDeterministicMigrationReplacement<TGenotype, TFenotype>::vUpdateParams()
{
	if (!b_each_different)
	{
		v_set_next_index();
	}//if (!b_each_different)
}//void CMultiDeterministicMigrationReplacement<TGenotype, TFenotype>::vUpdateParams()

template <class TGenotype, class TFenotype>
void CMultiDeterministicMigrationReplacement<TGenotype, TFenotype>::v_set_next_index()
{
	i_current_migration_replacement_index = (i_current_migration_replacement_index + 1) % (uint8_t)v_migration_replacements.size();
}//void CMultiDeterministicMigrationReplacement<TGenotype, TFenotype>::v_set_next_index()


template <class TGenotype, class TFenotype>
CMultiRandomizedMigrationReplacement<TGenotype, TFenotype>::CMultiRandomizedMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem)
	: CMultiMigrationReplacement<TGenotype, TFenotype>(pcProblem)
{

}//CMultiRandomizedMigrationReplacement<TGenotype, TFenotype>::CMultiRandomizedMigrationReplacement(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CError CMultiRandomizedMigrationReplacement<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)
{
	CError c_error;

	CBoolCommandParam p_each_different(MIGRATION_REPLACEMENT_ARGUMENT_EACH_DIFFERENT);
	b_each_different = p_each_different.bGetValue(psSettings, &c_error);

	if (!c_error)
	{
		c_error = CMultiMigrationReplacement<TGenotype, TFenotype>::eConfigure(plGenePatterns, psSettings);
	}//if (!c_error)

	if (!c_error)
	{
		v_set_random_index();
	}//if (!c_error)

	return c_error;
}//CError CMultiRandomizedMigrationReplacement<TGenotype, TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)

template <class TGenotype, class TFenotype>
void CMultiRandomizedMigrationReplacement<TGenotype, TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)
{
	if (b_each_different)
	{
		v_set_random_index();
	}//if (b_each_different)

	v_migration_replacements.at(i_current_migration_replacement_index)->vReplace(iPopulationSize, ppcPopulation, iNumberOfMigratingIndividuals, ppcMigratingIndividuals);
}//void CMultiRandomizedMigrationReplacement<TGenotype, TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<TGenotype, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<TGenotype, TFenotype> **ppcMigratingIndividuals)

template <class TGenotype, class TFenotype>
void CMultiRandomizedMigrationReplacement<TGenotype, TFenotype>::vUpdateParams()
{
	if (!b_each_different)
	{
		v_set_random_index();
	}//if (!b_each_different)
}//void CMultiRandomizedMigrationReplacement<TGenotype, TFenotype>::vUpdateParams()

template  <class TGenotype, class TFenotype>
void CMultiRandomizedMigrationReplacement<TGenotype, TFenotype>::v_set_random_index()
{
	i_current_migration_replacement_index = (uint8_t)RandUtils::iRandIndex((uint8_t)v_migration_replacements.size());;
}//void CMultiRandomizedMigrationReplacement<TGenotype, TFenotype>::v_set_random_index()


template <class TFenotype>
CBinaryGenePatternMigrationReplacement<TFenotype>::CBinaryGenePatternMigrationReplacement()
{
	pc_gene_pattern_selection = nullptr;
}//CBinaryGenePatternMigrationReplacement<TFenotype>::CBinaryGenePatternMigrationReplacement()

template <class TFenotype>
CBinaryGenePatternMigrationReplacement<TFenotype>::CBinaryGenePatternMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent)
{
	pl_gene_patterns = plGenePatterns;
	b_gene_pattern_each_different = bGenePatternEachDifferent;
	pc_gene_pattern_selection = nullptr;
}//CBinaryGenePatternMigrationReplacement<TFenotype>::CBinaryGenePatternMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent)

template <class TFenotype>
CBinaryGenePatternMigrationReplacement<TFenotype>::~CBinaryGenePatternMigrationReplacement()
{
	delete pc_gene_pattern_selection;
}//CBinaryGenePatternMigrationReplacement<TFenotype>::~CBinaryGenePatternMigrationReplacement()

template <class TFenotype>
CError CBinaryGenePatternMigrationReplacement<TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)
{
	delete pc_gene_pattern_selection;
	pc_gene_pattern_selection = nullptr;

	CError c_error = CMigrationReplacement<CBinaryCoding, CBinaryCoding>::eConfigure(plGenePatterns, psSettings);

	if (!c_error)
	{
		if (plGenePatterns)
		{
			pl_gene_patterns = plGenePatterns;
		}//if (plGenePatterns)
		else
		{
			c_error.vSetError(CError::iERROR_CODE_SYSTEM_MISSING_ARGUMENT, "gene patterns");
		}//else if (plGenePatterns)
	}//if (!c_error)

	if (!c_error)
	{
		CBoolCommandParam p_each_different(MIGRATION_REPLACEMENT_ARGUMENT_GENE_PATTERN_EACH_DIFFERENT);
		b_gene_pattern_each_different = p_each_different.bGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		pc_gene_pattern_selection = pcGetGenePatternSelection(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CBinaryGenePatternMigrationReplacement<TFenotype>::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)


template <class TFenotype>
CBinaryGenePatternRandomMigrationReplacement<TFenotype>::CBinaryGenePatternRandomMigrationReplacement()
{

}//CBinaryGenePatternRandomMigrationReplacement<TFenotype>::CBinaryGenePatternRandomMigrationReplacement()

template <class TFenotype>
CBinaryGenePatternRandomMigrationReplacement<TFenotype>::CBinaryGenePatternRandomMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent)
	: CBinaryGenePatternMigrationReplacement<TFenotype>(plGenePatterns, bGenePatternEachDifferent)
{

}//CBinaryGenePatternRandomMigrationReplacement<TFenotype>::CBinaryGenePatternRandomMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent)

#include <iostream>

using namespace std;

template <class TFenotype>
void CBinaryGenePatternRandomMigrationReplacement<TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<CBinaryCoding, CBinaryCoding> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<CBinaryCoding, CBinaryCoding> **ppcMigratingIndividuals)
{
	if (pl_gene_patterns->bAny())
	{
		CGenePattern *pc_gene_pattern = nullptr;

		if (!b_gene_pattern_each_different)
		{
			pc_gene_pattern = pl_gene_patterns->pcGetRandomGenePattern();
		}//if (!b_gene_pattern_each_different)

		uint16_t i_gene_index;

		CBinaryCoding *pc_genotype_population, *pc_genotype_migration;

		random_shuffle(ppcPopulation + 0, ppcPopulation + iPopulationSize);

		for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
		{
			if (b_gene_pattern_each_different)
			{
				pc_gene_pattern = pl_gene_patterns->pcGetRandomGenePattern();
			}//if (b_gene_pattern_each_different)

			//cout << endl;

			//cout << pl_gene_patterns->iGetNumberOfGenePatterns() << endl << endl;

			//for (uint16_t j = 0; j < pc_gene_pattern->iGetSize(); j++)
			//{
			//	cout << *(pc_gene_pattern->piGetPattern() + j) << " ";
			//}//for (uint16_t j = 0; j < pc_gene_pattern->iGetSize(); j++)

			//cout << endl << endl;

			pc_genotype_population = (*(ppcPopulation + i))->pcGetGenotype();
			pc_genotype_migration = (*(ppcMigratingIndividuals + i))->pcGetGenotype();

			//cout << endl << "before" << endl << endl;
			//cout << pc_genotype_population << endl << endl;
			//cout << pc_genotype_migration << endl << endl;

			for (uint16_t j = 0; j < pc_gene_pattern->iGetSize(); j++)
			{
				i_gene_index = *(pc_gene_pattern->piGetPattern() + j);
				*(pc_genotype_population->piGetBits() +  i_gene_index) = *(pc_genotype_migration->piGetBits() + i_gene_index);
			}//for (uint16_t j = 0; j < pc_gene_pattern->iGetSize(); j++)

			(*(ppcPopulation + i))->vIsEvaluated(false);

			//cout << "after" << endl << endl;
			//cout << pc_genotype_population << endl << endl;
		}//for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
	}//if (pl_gene_patterns->bAny())

	for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
	{
		delete *(ppcMigratingIndividuals + i);
	}//for (uint32_t i = 0; i < iPopulationSize; i++)
}//void CBinaryGenePatternRandomMigrationReplacement<TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<CBinaryCoding, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<CBinaryCoding, TFenotype> **ppcMigratingIndividuals)


template <class TFenotype>
CBinaryGenePatternBestMigrationReplacement<TFenotype>::CBinaryGenePatternBestMigrationReplacement(bool bDeterministic)
{
	b_deterministic = bDeterministic;
}//CBinaryGenePatternBestMigrationReplacement<TFenotype>::CBinaryGenePatternBestMigrationReplacement(bool bDeterministic)

template <class TFenotype>
CBinaryGenePatternBestMigrationReplacement<TFenotype>::CBinaryGenePatternBestMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent, bool bDeterministic)
	: CBinaryGenePatternMigrationReplacement<TFenotype>(plGenePatterns, bGenePatternEachDifferent)
{
	b_deterministic = bDeterministic;
}//CBinaryGenePatternRandomMigrationReplacement<TFenotype>::CBinaryGenePatternBestMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent, bool bDeterministic)

template <class TFenotype>
void CBinaryGenePatternBestMigrationReplacement<TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<CBinaryCoding, CBinaryCoding> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<CBinaryCoding, CBinaryCoding> **ppcMigratingIndividuals)
{
	if (pl_gene_patterns->bAny())
	{
		CGenePattern *pc_gene_pattern = nullptr;

		if (!b_gene_pattern_each_different)
		{
			pc_gene_pattern = pl_gene_patterns->pcGetRandomGenePattern();
		}//if (!b_gene_pattern_each_different)

		uint16_t i_gene_index;

		CBinaryCoding *pc_genotype_population, *pc_genotype_migration;

		sort(ppcPopulation + 0, ppcPopulation + iPopulationSize, [&](CIndividual<CBinaryCoding, CBinaryCoding> *pcIndividual0, CIndividual<CBinaryCoding, CBinaryCoding> *pcIndividual1)
		{
			return pc_problem->bIsBetterIndividual(pcIndividual0, pcIndividual1);
		});

		if (!b_deterministic)
		{
			random_shuffle(ppcMigratingIndividuals + 0, ppcMigratingIndividuals + iNumberOfMigratingIndividuals);
		}//if (!b_deterministic)

		for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
		{
			if (b_gene_pattern_each_different)
			{
				pc_gene_pattern = pl_gene_patterns->pcGetRandomGenePattern();
			}//if (b_gene_pattern_each_different)

			pc_genotype_population = (*(ppcPopulation + i))->pcGetGenotype();
			pc_genotype_migration = (*(ppcMigratingIndividuals + i))->pcGetGenotype();

			for (uint16_t j = 0; j < pc_gene_pattern->iGetSize(); j++)
			{
				i_gene_index = *(pc_gene_pattern->piGetPattern() + j);
				*(pc_genotype_population->piGetBits() +  i_gene_index) = *(pc_genotype_migration->piGetBits() + i_gene_index);
			}//for (uint16_t j = 0; j < pc_gene_pattern->iGetSize(); j++)

			(*(ppcPopulation + i))->vIsEvaluated(false);
		}//for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
	}//if (pl_gene_patterns->bAny())

	for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
	{
		delete *(ppcMigratingIndividuals + i);
	}//for (uint32_t i = 0; i < iPopulationSize; i++)
}//void CBinaryGenePatternBestMigrationReplacement<TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<CBinaryCoding, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<CBinaryCoding, TFenotype> **ppcMigratingIndividuals)


template <class TFenotype>
CBinaryGenePatternWorstMigrationReplacement<TFenotype>::CBinaryGenePatternWorstMigrationReplacement()
{

}//CBinaryGenePatternWorstMigrationReplacement<TFenotype>::CBinaryGenePatternWorstMigrationReplacement()

template <class TFenotype>
CBinaryGenePatternWorstMigrationReplacement<TFenotype>::CBinaryGenePatternWorstMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent)
	: CBinaryGenePatternMigrationReplacement<TFenotype>(plGenePatterns, bGenePatternEachDifferent)
{

}//CBinaryGenePatternWorstMigrationReplacement<TFenotype>::CBinaryGenePatternWorstMigrationReplacement(CGenePatternList *plGenePatterns, bool bGenePatternEachDifferent)

template <class TFenotype>
void CBinaryGenePatternWorstMigrationReplacement<TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<CBinaryCoding, CBinaryCoding> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<CBinaryCoding, CBinaryCoding> **ppcMigratingIndividuals)
{
	if (pl_gene_patterns->bAny())
	{
		CGenePattern *pc_gene_pattern = nullptr;

		if (!b_gene_pattern_each_different)
		{
			pc_gene_pattern = pl_gene_patterns->pcGetRandomGenePattern();
		}//if (!b_gene_pattern_each_different)

		uint16_t i_gene_index;

		CBinaryCoding *pc_genotype_population, *pc_genotype_migration;

		sort(ppcPopulation + 0, ppcPopulation + iPopulationSize, [&](CIndividual<CBinaryCoding, CBinaryCoding> *pcIndividual0, CIndividual<CBinaryCoding, CBinaryCoding> *pcIndividual1)
		{
			return pc_problem->bIsBetterIndividual(pcIndividual1, pcIndividual0);
		});

		random_shuffle(ppcMigratingIndividuals + 0, ppcMigratingIndividuals + iNumberOfMigratingIndividuals);

		for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
		{
			if (b_gene_pattern_each_different)
			{
				pc_gene_pattern = pl_gene_patterns->pcGetRandomGenePattern();
			}//if (b_gene_pattern_each_different)

			pc_genotype_population = (*(ppcPopulation + i))->pcGetGenotype();
			pc_genotype_migration = (*(ppcMigratingIndividuals + i))->pcGetGenotype();

			for (uint16_t j = 0; j < pc_gene_pattern->iGetSize(); j++)
			{
				i_gene_index = *(pc_gene_pattern->piGetPattern() + j);
				*(pc_genotype_population->piGetBits() +  i_gene_index) = *(pc_genotype_migration->piGetBits() + i_gene_index);
			}//for (uint16_t j = 0; j < pc_gene_pattern->iGetSize(); j++)

			(*(ppcPopulation + i))->vIsEvaluated(false);
		}//for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
	}//if (pl_gene_patterns->bAny())

	for (uint32_t i = 0; i < iNumberOfMigratingIndividuals; i++)
	{
		delete *(ppcMigratingIndividuals + i);
	}//for (uint32_t i = 0; i < iPopulationSize; i++)
}//void CBinaryGenePatternWorstMigrationReplacement<TFenotype>::vReplace(uint32_t iPopulationSize, CIndividual<CBinaryCoding, TFenotype> **ppcPopulation, uint32_t iNumberOfMigratingIndividuals, CIndividual<CBinaryCoding, TFenotype> **ppcMigratingIndividuals)


template class CMigrationReplacement<CBinaryCoding, CBinaryCoding>;
template class CMigrationReplacement<CRealCoding, CRealCoding>;


template class CMultiMigrationReplacement<CBinaryCoding, CBinaryCoding>;
template class CMultiMigrationReplacement<CRealCoding, CRealCoding>;


template class CMultiDeterministicMigrationReplacement<CBinaryCoding, CBinaryCoding>;
template class CMultiDeterministicMigrationReplacement<CRealCoding, CRealCoding>;


template class CMultiRandomizedMigrationReplacement<CBinaryCoding, CBinaryCoding>;
template class CMultiRandomizedMigrationReplacement<CRealCoding, CRealCoding>;


template class CBinaryGenePatternRandomMigrationReplacement<CBinaryCoding>;
template class CBinaryGenePatternRandomMigrationReplacement<CRealCoding>;


template class CBinaryGenePatternBestMigrationReplacement<CBinaryCoding>;
template class CBinaryGenePatternBestMigrationReplacement<CRealCoding>;


template class CBinaryGenePatternWorstMigrationReplacement<CBinaryCoding>;
template class CBinaryGenePatternWorstMigrationReplacement<CRealCoding>;