#include "Mutation.h"

#include "BinaryCoding.h"
#include "FloatCommandParam.h"
#include "GenePattern.h"
#include "RealCoding.h"

template <class TGenotype>
CMutation<TGenotype>::CMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)
{
	v_init(bPopulationRequired, bBestGenotypeRequired, bBestGenotypesRequired);
}//CMutation<TGenotype>::CMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)

template <class TGenotype>
CMutation<TGenotype>::CMutation(float fProbability, bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)
{
	v_init(bPopulationRequired, bBestGenotypeRequired, bBestGenotypesRequired);
	f_probability = fProbability;
}//CMutation<TGenotype>::CMutation(float fProbability, bool bPopulationRequired, bBestGenotypeRequired, bool bBestGenotypesRequired)

template <class TGenotype>
CMutation<TGenotype>::~CMutation()
{
	v_clear_population();
	v_clear_best_genotypes();

	delete pc_best_genotype;
}//CMutation<TGenotype>::~CMutation()

template <class TGenotype>
CError CMutation<TGenotype>::eConfigure(CGenePatternList *plGenePatterns, istream * psSettings)
{
	CError c_error;

	CFloatCommandParam p_probability(MUTATION_ARGUMENT_PROBABILITY, DEF_PROBABILITY_MIN, DEF_PROBABILITY_MAX, DEF_PROBABILITY_MAX);
	f_probability = p_probability.fGetValue(psSettings, &c_error);

	return c_error;
}//CError CMutation<TGenotype>::eConfigure(CGenePatternList *plGenePatterns, istream * psSettings)

template <class TGenotype>
void CMutation<TGenotype>::vUpdatePopulation(uint32_t iSize, TGenotype **ppcPopulation)
{
	if (b_population_required)
	{
		v_clear_population();

		i_population_size = iSize;
		ppc_population = new TGenotype*[i_population_size];

		TGenotype *pc_genotype_copy;

		for (uint32_t i = 0; i < i_population_size; i++)
		{
			pc_genotype_copy = new TGenotype(*(ppcPopulation + i));
			*(ppc_population + i) = pc_genotype_copy;
		}//for (uint32_t i = 0; i < i_population_size; i++)
	}//if (b_population_required)
}//void CMutation<TGenotype>::(uint32_t iSize, TGenotype **ppcPopulation)

template <class TGenotype>
void CMutation<TGenotype>::vUpdateBestGenotype(TGenotype *pcBestGenotype)
{
	delete pc_best_genotype;
	pc_best_genotype = new TGenotype(pcBestGenotype);
}//void CMutation<TGenotype>::vUpdateBestGenotype(TGenotype *pcBestGenotype)

template <class TGenotype>
void CMutation<TGenotype>::vUpdateBestGenotypes(uint8_t iCount, TGenotype **ppcBestGenotypes)
{
	if (b_best_genotypes_required)
	{
		v_clear_best_genotypes();

		TGenotype *pc_genotype_copy;

		for (uint8_t i = 0; i < iCount; i++)
		{
			pc_genotype_copy = new TGenotype(*(ppcBestGenotypes + i));
			v_best_genotypes.push_back(pc_genotype_copy);
		}//for (uint8_t i = 0; i < iCount; i++)
	}//if (b_best_genotypes_required)
}//void CMutation<TGenotype>::::vUpdateBestGenotypes(uint8_t iCount, TGenotype **ppcBestGenotypes)

template <class TGenotype>
void CMutation<TGenotype>::v_clear_population()
{
	if (ppc_population)
	{
		for (uint32_t i = 0; i < i_population_size; i++)
		{
			delete *(ppc_population + i);
		}//for (uint32_t i = 0; i < i_population_size; i++)

		delete ppc_population;
	}//if (ppc_population)
}//void CMutation<TGenotype>::v_clear_population()

template <class TGenotype>
void CMutation<TGenotype>::v_clear_best_genotypes()
{
	for (uint32_t i = 0; i < (uint32_t)v_best_genotypes.size(); i++)
	{
		delete v_best_genotypes.at(i);
	}//for (uint32_t i = 0; i < (uint32_t)v_best_genotypes.size(); i++)

	v_best_genotypes.clear();
}//void CMutation<TGenotype>::v_clear_best_genotypes()

template <class TGenotype>
void CMutation<TGenotype>::v_init(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)
{
	b_population_required = bPopulationRequired;
	b_best_genotype_required = bBestGenotypeRequired;
	b_best_genotypes_required = bBestGenotypesRequired;

	ppc_population = nullptr;
	pc_best_genotype = nullptr;
}//void CMutation<TGenotype>::v_init(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)


CBinaryGenePatternBestSwapMutation::CBinaryGenePatternBestSwapMutation()
	: CMutation<CBinaryCoding>(false, false, true)
{

}//CBinaryGenePatternBestSwapMutation::CBinaryGenePatternBestSwapMutation()

CBinaryGenePatternBestSwapMutation::CBinaryGenePatternBestSwapMutation(float fProbability, CGenePatternList *plGenePatterns)
	: CMutation<CBinaryCoding>(fProbability, false, false, true)
{
	pl_gene_patterns = plGenePatterns;
}//CBinaryGenePatternBestSwapMutation::CBinaryGenePatternBestSwapMutation(float fProbability, CGenePatternList *plGenePatterns)

CError CBinaryGenePatternBestSwapMutation::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)
{
	CError c_error = CMutation<CBinaryCoding>::eConfigure(plGenePatterns, psSettings);

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

	return c_error;
}//CError CBinaryGenePatternBestSwapMutation::eConfigure(CGenePatternList *plGenePatterns, istream *psSettings)

bool CBinaryGenePatternBestSwapMutation::bMutate(CBinaryCoding *pcGenotype, uint32_t iIndex)
{
	if (pl_gene_patterns->bAny())
	{
		if (b_apply_mutation(pcGenotype))
		{
			CGenePattern *pc_gene_pattern = pl_gene_patterns->pcGetRandomGenePattern();
			CBinaryCoding *pc_best_genotype = v_best_genotypes.at(RandUtils::iRandIndex((uint32_t)v_best_genotypes.size()));

			uint16_t i_gene_index;

			for (uint16_t i = 0; i < pc_gene_pattern->iGetSize(); i++)
			{
				i_gene_index = *(pc_gene_pattern->piGetPattern() + i);
				*(pcGenotype->piGetBits() + i_gene_index) = RandUtils::iRandNumber(0, 1); //*(pc_best_genotype->piGetBits() + i_gene_index);
			}//for (uint16_t i = 0; i < pc_gene_pattern->iGetSize(); i++)

			return true;
		}//if (b_apply_mutation(pcGenotype))
		else
		{
			return false;
		}//else if (b_apply_mutation(pcGenotype))
	}//if (pl_gene_patterns->bAny())
	else
	{
		return false;
	}//else if (pl_gene_patterns->bAny())
}//bool CBinaryGenePatternBestSwapMutation::bMutate(CBinaryCoding *pcGenotype, uint32_t iIndex)

template class CMutation<CBinaryCoding>;
template class CMutation<CRealCoding>;