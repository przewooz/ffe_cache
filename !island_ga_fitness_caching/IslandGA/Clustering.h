#ifndef CLUSTERING_H
#define CLUSTERING_H

#include "Cluster.h"
#include "Error.h"
#include "Log.h"

#include <atlstr.h>
#include <cstdint>
#include <istream>
#include <vector>

using namespace std;

namespace Clustering
{
	template <class TPoint>
	class CClustering
	{
	public:
		CClustering(CLog *pcLog);

		virtual ~CClustering();

		virtual CError eConfigure(istream *psSettings) { return CError(); };

		virtual void vRun(uint32_t iNumberOfPoints, TPoint **ppcPoints) = 0;

		CCluster<TPoint> *pcClassify(TPoint *pcPoint);

		virtual CString sToString();

		CCluster<TPoint> **ppcGetClusters() { return v_clusters.data(); }
		uint32_t iGetNumberOfClusters() { return (uint32_t)v_clusters.size(); }

	protected:
		void v_clear();

		vector<CCluster<TPoint>*> v_clusters;
		CLog *pc_log;
	};//class CClustering
}//namespace Clustering

#endif//CLUSTERING_H