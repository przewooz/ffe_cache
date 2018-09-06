#ifndef K_MEANS_CLUSTERING_H
#define K_MEANS_CLUSTERING_H

#define K_MEANS_CLUSTERING_ARGUMENT_NUMBER_OF_ITERATIONS "number_of_iterations"
#define K_MEANS_CLUSTERING_ARGUMENT_EXPECTED_NUMBER_OF_CLUSTERS "expected_number_of_clusters"

#include "Cluster.h"
#include "Clustering.h"
#include "Error.h"
#include "Log.h"
#include "RealCoding.h"

#include <atlstr.h>
#include <cstdint>
#include <istream>

using namespace std;

namespace Clustering
{
	class CKMeansClustering : public CClustering<CRealCoding>
	{
	public:
		CKMeansClustering(CLog *pcLog);

		virtual CError eConfigure(istream *psSettings);

		virtual void vRun(uint32_t iNumberOfPoints, CRealCoding **ppcPoints);

	private:
		uint32_t i_number_of_iterations;
		uint32_t i_expected_number_of_clusters;
	};//class CKMeansClustering : public CClustering<CRealCoding>


	class CKMeansCluster : public CCluster<CRealCoding>
	{
	public:
		CKMeansCluster(CLog *pcLog);

		virtual ~CKMeansCluster();

		virtual double dComputeDistance(CRealCoding *pcPoint);
		virtual bool bExceededMaxDistance(CRealCoding *pcPoint, double dMaxDistance, double *pdDistance);

		void vUpdateCenterPoint();

		virtual CString sToString();

		CRealCoding *pcGetCenterPoint() { return pc_center_point; };

	private:
		CRealCoding *pc_center_point;
	};//class CKMeansCluster : public CCluster<CRealCoding>
}//namespace Clustering

#endif//K_MEANS_CLUSTERING_H