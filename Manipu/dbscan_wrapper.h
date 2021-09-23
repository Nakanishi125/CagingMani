#pragma once

#include "dbscan.h"
#include "Configuration.h"

class dbscan{
private:
	DBSCAN* myScan;


public:
	dbscan(std::vector<State3D<int>>& C_free, unsigned int minPts, float eps)
	{
		std::vector<Point> points;
		for(int i=0; i<C_free.size(); ++i)
		{
			Point tmp;
			tmp.x = C_free[i].x;	tmp.y = C_free[i].y;  tmp.z = C_free[i].th;
			tmp.clusterID = UNCLASSIFIED;

			points.push_back(tmp);
		}
	
		this->myScan = new DBSCAN(minPts, eps, points);		
	}

	int run(){
		int r = myScan->run();

		for(int i=0; i<myScan->m_points.size(); ++i)
		{
			std::cout << "x:";	std::cout << myScan->m_points[i].x;
			std::cout << " y:";	std::cout << myScan->m_points[i].y;
			std::cout << " z:";	std::cout << myScan->m_points[i].z;
			std::cout << " ID:"; std::cout << myScan->m_points[i].clusterID << std::endl;
		}


	}

	void dataFitting(std::vector<std::vector<State3D<int>>>& cluster)
	{
		// bubble sort
		for(int i=0; i < myScan->getTotalPointSize()-1; ++i){
			for(int j=myScan->getTotalPointSize()-1; j > i; --j){
				if(myScan->m_points[j].clusterID < myScan->m_points[j-1].clusterID){
					auto tmp = myScan->m_points[j-1];
					myScan->m_points[j-1] = myScan->m_points[j];
					myScan->m_points[j] = tmp;
				}
			}
		}

		int ref = myScan->m_points[0].clusterID;
		std::vector<State3D<int>> vectmp;

		for(int i=0; i < myScan->getTotalPointSize(); ++i)
		{
			int id = myScan->m_points[i].clusterID;
			if(id == -1)	continue;

			if( id == ref ){
				State3D<int> temp(static_cast<int>(myScan->m_points[i].x),
											 	  static_cast<int>(myScan->m_points[i].y),
													static_cast<int>(myScan->m_points[i].z) );

				vectmp.push_back(temp);
			}
			else{
				ref = id;
				cluster.push_back(vectmp);
				vectmp.clear();
			}
		}

	}

};
