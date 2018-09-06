#include "Clustering.h"

#include "RealCoding.h"

using namespace Clustering;

template <class TPoint>
CClustering<TPoint>::CClustering(CLog *pcLog)
{
	pc_log = pcLog;
}//CClustering<TPoint>::CClustering(CLog *pcLog)

template <class TPoint>
CClustering<TPoint>::~CClustering()
{
	v_clear();
}//CClustering<TPoint>::~CClustering()

template <class TPoint>
CCluster<TPoint> * CClustering<TPoint>::pcClassify(TPoint *pcPoint)
{
	CCluster<TPoint> *pc_cluster = nullptr;

	if (!v_clusters.empty())
	{
		pc_cluster = v_clusters.front();

		double d_distance = pc_cluster->dComputeDistance(pcPoint);

		for (uint32_t i = 1; i < (uint32_t)v_clusters.size(); i++)
		{
			if (!v_clusters.at(i)->bExceededMaxDistance(pcPoint, d_distance, &d_distance))
			{
				pc_cluster = v_clusters.at(i);
			}//if (!v_clusters.at(i)->bExceededMaxDistance(pcPoint, d_distance, &d_distance))
		}//for (uint32_t i = 1; i < (uint32_t)v_clusters.size(); i++)
	}//if (!v_clusters.empty())

	return pc_cluster;
}//CCluster<TPoint> * CClustering<TPoint>::pcClassify(TPoint *pcPoint)

template <class TPoint>
CString CClustering<TPoint>::sToString()
{
	CString s_text;

	for (uint32_t i = 0; i < (uint32_t)v_clusters.size(); i++)
	{
		s_text.AppendFormat("cluster %d\n", i + 1);
		s_text.Append(v_clusters.at(i)->sToString());

		if (i < (uint32_t)(v_clusters.size() - 1))
		{
			s_text.AppendChar('\n');
		}//if (i < (uint32_t)(v_clusters.size() - 1))
	}//for (uint32_t i = 0; i < (uint32_t)v_clusters.size(); i++)

	return s_text;
}//CString CClustering<TPoint>::sToString()

template <class TPoint>
void CClustering<TPoint>::v_clear()
{
	for (uint32_t i = 0; i < (uint32_t)v_clusters.size(); i++)
	{
		delete v_clusters.at(i);
	}//for (uint32_t i = 0; i < (uint32_t)v_clusters.size(); i++)

	v_clusters.clear();
}//void CClustering<TPoint>::v_clear()


template class CClustering<CRealCoding>;