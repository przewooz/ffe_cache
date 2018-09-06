#include "Cluster.h"

#include "RealCoding.h"

#include <algorithm>

using namespace Clustering;

template <class TPoint>
uint32_t CCluster<TPoint>::iERROR_PARENT_CCluster = CError::iADD_ERROR_PARENT("CCluster");

template <class TPoint>
CCluster<TPoint>::CCluster(CLog *pcLog)
{
	pc_log = pcLog;
}//CCluster<TPoint>::CCluster(CLog *pcLog)

template <class TPoint>
CCluster<TPoint>::~CCluster()
{
	l_points.clear();
	v_points.clear();
}//CCluster<TPoint>::~CCluster()

template <class TPoint>
CError CCluster<TPoint>::eAddPoint(TPoint *pcPoint)
{
	CError c_error(iERROR_PARENT_CCluster);

	if (l_points.empty())
	{
		l_points.push_back(pcPoint);
	}//if (l_points.empty())
	else
	{
		if (pcPoint->bHasSameParams(l_points.front()))
		{
			l_points.push_back(pcPoint);
		}//if (pcPoint->bHasSameParams(l_points.front()))
		else
		{
			c_error.vSetError(CError::iERROR_CODE_SYSTEM_ARGUMENT_WRONG_VALUE, "the adding point is different than other points within the cluster");
		}//else if (pcPoint->bHasSameParams(l_points.front()))
	}//else if (l_points.empty())

	return c_error;
}//CError CCluster<TPoint>::eAddPoint(TPoint *pcPoint)

template <class TPoint>
void CCluster<TPoint>::vRemovePoint(TPoint *pcPoint)
{
	l_points.remove(pcPoint);
}//void CCluster<TPoint>::vRemovePoint(TPoint *pcPoint)

template <class TPoint>
bool CCluster<TPoint>::bContainsPoint(TPoint *pcPoint)
{
	return find(l_points.begin(), l_points.end(), pcPoint) != l_points.end();
}//bool CCluster<TPoint>::bContainsPoint(TPoint *pcPoint)

template <class TPoint>
void CCluster<TPoint>::vClearPoints()
{
	l_points.clear();
}//void CCluster<TPoint>::vClearPoints()

template <class TPoint>
double CCluster<TPoint>::dComputeMeanDistance()
{
	double d_mean_distance = 0;

	if (!l_points.empty())
	{
		double d_sum_distance = 0;

		for (list<CRealCoding*>::iterator it = l_points.begin(), it_end = l_points.end(); it != it_end; it++)
		{
			d_sum_distance += dComputeDistance(*it);
		}//for (list<CRealCoding*>::iterator it = l_points.begin(), it_end = l_points.end(); it != it_end; it++)

		d_mean_distance = d_sum_distance / (double)l_points.size();
	}//if (!l_points.empty())

	return d_mean_distance;
}//double CCluster<TPoint>::dComputeMeanDistance()

template <class TPoint>
CString CCluster<TPoint>::sToString()
{
	CString s_text;

	TPoint **ppc_points = ppcGetPoints();

	for (uint32_t i = 0; i < iGetSize(); i++)
	{
		s_text.Append((*(ppc_points + i))->sToString());

		if (i < (uint32_t)(iGetSize() - 1))
		{
			s_text.AppendChar('\n');
		}//if (i < (uint32_t)(iGetSize() - 1))
	}//for (uint32_t i = 0; i < iGetSize(); i++)

	return s_text;
}//CString CCluster<TPoint>::sToString()

template <class TPoint>
TPoint **Clustering::CCluster<TPoint>::ppcGetPoints()
{
	v_points.clear();

	for (list<CRealCoding*>::iterator it = l_points.begin(), it_end = l_points.end(); it != it_end; it++)
	{
		v_points.push_back(*it);
	}//for (list<CRealCoding*>::iterator it = l_points.begin(), it_end = l_points.end(); it != it_end; it++)

	return &v_points.front();
}//TPoint **Clustering::CCluster<TPoint>::ppcGetPoints()


template class CCluster<CRealCoding>;