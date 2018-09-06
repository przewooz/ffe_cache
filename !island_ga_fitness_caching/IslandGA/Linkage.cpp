#include "Linkage.h"

#include "BinaryCoding.h"
#include "RealCoding.h"

template <class TGenotype>
CLinkage<TGenotype>::~CLinkage()
{

}//CLinkage<TGenotype, TFenotype>::~CLinkage()

template class CLinkage<CBinaryCoding>;
template class CLinkage<CRealCoding>;