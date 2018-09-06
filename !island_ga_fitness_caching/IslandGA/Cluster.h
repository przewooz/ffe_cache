#ifndef CLUSTER_H
#define CLUSTER_H

#include "Error.h"
#include "Log.h"

#include <atlstr.h>
#include <cstdint>
#include <functional>
#include <list>
#include <vector>

using namespace std;

namespace Clustering
{
	template <class TPoint>
	class CCluster
	{
	public:
		static uint32_t iERROR_PARENT_CCluster;

		CCluster(CLog *pcLog);

		virtual ~CCluster();

		CError eAddPoint(TPoint *pcPoint);
		void vRemovePoint(TPoint *pcPoint);
		bool bContainsPoint(TPoint *pcPoint);

		void vClearPoints();

		double dComputeMeanDistance();

		virtual double dComputeDistance(TPoint *pcPoint) = 0;
		virtual bool bExceededMaxDistance(TPoint *pcPoint, double dMaxDistance, double *pdDistance) = 0;

		virtual CString sToString();

		TPoint **ppcGetPoints();
		uint32_t iGetSize() { return (uint32_t)l_points.size(); }

	protected:
		list<TPoint*> l_points;
		CLog *pc_log;

	private:
		vector<TPoint*> v_points;
	};//class CCluster
}//namespace Clustering

#endif//CLUSTER_H

