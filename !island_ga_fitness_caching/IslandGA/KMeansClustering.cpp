#include "KMeansClustering.h"

#include "RandUtils.h"
#include "UIntCommandParam.h"

#include <cfloat>
#include <unordered_set>

using namespace Clustering;


CKMeansClustering::CKMeansClustering(CLog *pcLog)
	: CClustering<CRealCoding>(pcLog)
{

}//CKMeansClustering::CKMeansClustering(CLog *pcLog)

CError CKMeansClustering::eConfigure(istream *psSettings)
{
	CError c_error = CClustering<CRealCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_number_of_iterations(K_MEANS_CLUSTERING_ARGUMENT_NUMBER_OF_ITERATIONS, 2, UINT32_MAX);
		i_number_of_iterations = p_number_of_iterations.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_expected_number_of_clusters(K_MEANS_CLUSTERING_ARGUMENT_EXPECTED_NUMBER_OF_CLUSTERS, 1, UINT32_MAX);
		i_expected_number_of_clusters = p_expected_number_of_clusters.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CKMeansClustering::eConfigure(istream *psStream)

void CKMeansClustering::vRun(uint32_t iNumberOfPoints, CRealCoding **ppcPoints)
{
	v_clear();

	CCluster<CRealCoding> **ppc_points_clusters = new CCluster<CRealCoding>*[iNumberOfPoints];

	for (uint32_t i = 0; i < iNumberOfPoints; i++)
	{
		*(ppc_points_clusters + i) = nullptr;
	}//for (uint32_t i = 0; i < iNumberOfPoints; i++)

	unordered_set<uint32_t> s_random_centers_indexes(i_expected_number_of_clusters);

	uint32_t i_random_center_index;

	CKMeansCluster *pc_cluster;

	for (uint32_t i = 0; i < i_expected_number_of_clusters; i++)
	{
		i_random_center_index = RandUtils::iRandUniqueIndex(iNumberOfPoints, &s_random_centers_indexes);
		
		s_random_centers_indexes.insert(i_random_center_index);

		pc_cluster = new CKMeansCluster(pc_log);

		pc_cluster->eAddPoint(*(ppcPoints + i_random_center_index));
		pc_cluster->vUpdateCenterPoint();

		v_clusters.push_back(pc_cluster);

		*(ppc_points_clusters + i_random_center_index) = pc_cluster;
	}//for (uint32_t i = 0; i < i_expected_number_of_clusters; i++)

	CRealCoding *pc_point;

	CCluster<CRealCoding> *pc_old_point_cluster, *pc_new_point_cluster;

	for (uint32_t i = 0; i < i_number_of_iterations; i++)
	{
		for (uint32_t j = 0; j < iNumberOfPoints; j++)
		{
			pc_point = *(ppcPoints + j);

			pc_old_point_cluster = *(ppc_points_clusters + j);
			pc_new_point_cluster = pcClassify(pc_point);

			if (pc_old_point_cluster != pc_new_point_cluster)
			{
				if (pc_old_point_cluster)
				{
					pc_old_point_cluster->vRemovePoint(pc_point);
				}//if (pc_old_point_cluster)

				pc_new_point_cluster->eAddPoint(pc_point);

				*(ppc_points_clusters + j) = pc_new_point_cluster;
			}//if (pc_old_point_cluster != pc_new_point_cluster)
		}//for (uint32_t j = 0; j < iNumberOfPoints; j++)

		pc_log->vPrintLine("iteracja");

		for (uint32_t j = 0; j < (uint32_t)v_clusters.size(); j++)
		{
			((CKMeansCluster*)v_clusters.at(j))->vUpdateCenterPoint();

			CString s_msg;
			s_msg.Format("cluster index: %d; size: %d; mean distance: %f", j, v_clusters.at(j)->iGetSize(), 
				v_clusters.at(j)->dComputeMeanDistance());

			pc_log->vPrintLine(s_msg);
		}//for (uint32_t j = 0; j < (uint32_t)v_clusters.size(); j++)
	}//for (uint32_t i = 0; i < i_number_of_iterations; i++)

	delete ppc_points_clusters;
}//void CKMeansClustering::vRun(uint32_t iNumberOfPoints, CRealCoding **ppcPoints)


CKMeansCluster::CKMeansCluster(CLog *pcLog)
	: CCluster<CRealCoding>(pcLog)
{
	pc_center_point = nullptr;
}//CKMeansCluster::CKMeansCluster(CLog *pcLog)

CKMeansCluster::~CKMeansCluster()
{
	delete pc_center_point;
}//CKMeansCluster::~CKMeansCluster()

double CKMeansCluster::dComputeDistance(CRealCoding *pcPoint)
{
	double d_distance = DBL_MAX;

	if (pc_center_point != nullptr)
	{
		d_distance = pc_center_point->dComputeDistance(pcPoint);
	}//if (pc_center_point != nullptr)

	return d_distance;
}//double CKMeansCluster::dComputeDistance(CRealCoding *pcPoint)

bool CKMeansCluster::bExceededMaxDistance(CRealCoding *pcPoint, double dMaxDistance, double *pdDistance)
{
	double b_exceeded = true;

	if (pc_center_point != nullptr)
	{
		b_exceeded = pc_center_point->bExceededMaxDistance(pcPoint, dMaxDistance, pdDistance);
	}//if (pc_center_point != nullptr)

	return b_exceeded;
}//bool CKMeansCluster::bExceededMaxDistance(CRealCoding *pcPoint, double dMaxDistance, double *pdDistance)

void CKMeansCluster::vUpdateCenterPoint()
{
	if (!l_points.empty())
	{
		if (!pc_center_point || !pc_center_point->bHasSameParams(l_points.front()))
		{
			delete pc_center_point;
			pc_center_point = new CRealCoding(l_points.front());

			for (uint16_t i = 0; i < pc_center_point->iGetNumberOfDimensions(); i++)
			{
				*(pc_center_point->pdGetValues() + i) = 0;
			}//for (uint16_t i = 0; i < pc_center_point->iGetNumberOfDimensions(); i++)
		}//if (!pc_center_point || !pc_center_point->bHasSameParams(l_points.front()))

		for (uint16_t i = 0; i < pc_center_point->iGetNumberOfDimensions(); i++)
		{
			//*(pc_center_point->pdGetValues() + i) = 0;

			for (list<CRealCoding*>::iterator it = l_points.begin(), it_end = l_points.end(); it != it_end; it++)
			{
				*(pc_center_point->pdGetValues() + i) += *((*it)->pdGetValues() + i);
			}//for (list<CRealCoding*>::iterator it = l_points.begin(), it_end = l_points.end(); it != it_end; it++)

			//*(pc_center_point->pdGetValues() + i) /= (double)l_points.size();
			*(pc_center_point->pdGetValues() + i) /= (double)(l_points.size() + 1);
		}//for (uint16_t i = 0; i < pc_center_point->iGetNumberOfDimensions(); i++)
	}//if (!l_points.empty())
	//else
	//{
	//	delete pc_center_point;
	//	pc_center_point = nullptr;
	//}//else if (!l_points.empty())
}//void CKMeansCluster::vUpdateCenterPoint()

CString CKMeansCluster::sToString()
{
	CString s_text;

	s_text.Append("center point: ");

	if (pc_center_point)
	{
		s_text.Append(pc_center_point->sToString());
	}//if (pc_center_point)

	s_text.AppendChar('\n');

	CRealCoding **ppc_points = ppcGetPoints();

	for (uint32_t i = 0; i < iGetSize(); i++)
	{
		s_text.Append((*(ppc_points + i))->sToString());
		s_text.AppendFormat("(%f)", dComputeDistance(*(ppc_points + i)));

		if (i < (uint32_t)(iGetSize() - 1))
		{
			s_text.AppendChar('\n');
		}//if (i < (uint32_t)(iGetSize() - 1))
	}//for (uint32_t i = 0; i < iGetSize(); i++)

	return s_text;
}//CString CKMeansCluster::sToString()