#include <iostream>
#include <algorithm>
#include <cfloat>
#include <chrono>
#include <string>
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

#include "Configuration.h"
#include "Circle.h"

namespace bp = boost::property_tree;

Configuration* Configuration::instance = nullptr;

Configuration::Configuration()
	:top(), bottom(), range_x(), range_y(), range_th()
{
	bp::ptree pt;
	read_ini("../config/SpaceConfig.ini", pt);

//set range
	boost::optional<int> carrier = pt.get_optional<int>("range.x");
	range_x = carrier.get();
	carrier = pt.get_optional<int>("range.y");
	range_y = carrier.get();
	carrier = pt.get_optional<int>("range.th");
	range_th = carrier.get();

// set top
	carrier = pt.get_optional<int>("top.x");
	top.x = carrier.get();
	carrier = pt.get_optional<int>("top.y");
	top.y = carrier.get();
	carrier = pt.get_optional<int>("top.th");
	top.th = carrier.get();

// set bottom
	carrier = pt.get_optional<int>("bottom.x");
	bottom.x = carrier.get();
	carrier = pt.get_optional<int>("bottom.y");
	bottom.y = carrier.get();
	carrier = pt.get_optional<int>("bottom.th");
	bottom.th = carrier.get();

// set p_far
	p_far.resize(4);
	boost::optional<int> pfar_x = pt.get_optional<int>("pfar.p1x");
	boost::optional<int> pfar_y = pt.get_optional<int>("pfar.p1y");
	boost::optional<int> pfar_th = pt.get_optional<int>("pfar.p1th");
	State3D<int> p1(pfar_x.get(), pfar_y.get(), pfar_th.get());
	p_far[0] = p1;

	pfar_x = pt.get_optional<int>("pfar.p2x");
	pfar_y = pt.get_optional<int>("pfar.p2y");
	pfar_th = pt.get_optional<int>("pfar.p2th");
	State3D<int> p2(pfar_x.get(), pfar_y.get(), pfar_th.get());
	p_far[1] = p2;

	pfar_x = pt.get_optional<int>("pfar.p3x");
	pfar_y = pt.get_optional<int>("pfar.p3y");
	pfar_th = pt.get_optional<int>("pfar.p3th");
	State3D<int> p3(pfar_x.get(), pfar_y.get(), pfar_th.get());
	p_far[2] = p3;

	pfar_x = pt.get_optional<int>("pfar.p4x");
	pfar_y = pt.get_optional<int>("pfar.p4y");
	pfar_th = pt.get_optional<int>("pfar.p4th");
	State3D<int> p4(pfar_x.get(), pfar_y.get(), pfar_th.get());
	p_far[3] = p4;

	// Create space vertices list
	// Assume that (top - bottom)%range = 0
	int num = ((top.x - bottom.x)/range_x + 1) * ((top.y - bottom.y)/range_y + 1) * ((top.th - bottom.th)/range_th + 1);
	space.resize(num);
	int ord = 0;
	for(int x = bottom.x; x <= top.x; x = x + range_x)
		for(int y = bottom.y; y <= top.y; y = y + range_y)
			for(int th = bottom.th; th <= top.th; th = th + range_th){
				State3D<int> tmp(x,y,th);
				space[ord] = tmp;
				++ord;
			}

}


Configuration* Configuration::getInstance()
{
	if(instance == nullptr)
		{
			instance = new Configuration();
		}
	return instance;
}


void Configuration::get_C_free()
{
	auto start = std::chrono::system_clock::now();
	// last_C_free_obj = C_free_obj;

	Robot* robot = Robot::getInstance();
	Shape* shape = Shape::getInstance();
	Wall* wall   = Wall::getInstance();

	static int cnt = 1;

	C_free.resize(space.size());

////////////////////////////////////////////////////////////////////////////
// Confirm robot angle 
// To execute visualize_by_vertex.py, validate below code and output to csv file.
	std::string fn3 = "../robot_vertex.csv";
	std::ofstream logR(fn3, std::ios::out);
	if(cnt % 1 == 0){
		for(int l=0; l<robot->Lhand.size(); ++l){
			for(int i=0; i<robot->Lhand[l].geometry.vertices.size(); ++i){
				logR << robot->Lhand[l].geometry.vertices[i].x;	logR << ",";
				logR << robot->Lhand[l].geometry.vertices[i].y << std::endl;
			}
			logR << robot->Lhand[l].geometry.vertices[0].x; logR << ",";
			logR << robot->Lhand[l].geometry.vertices[0].y << std::endl;
		}
		
		for(int r=0; r<robot->Rhand.size(); ++r){
			for(int i=0; i<robot->Rhand[r].geometry.vertices.size(); ++i){
				logR << robot->Rhand[r].geometry.vertices[i].x;	logR <<  ",";
				logR << robot->Rhand[r].geometry.vertices[i].y << std::endl;
			}
			logR << robot->Rhand[r].geometry.vertices[0].x; logR << ",";
			logR << robot->Rhand[r].geometry.vertices[0].y << std::endl;
		}
	} 
////////////////////////////////////////////////////////////////////////////////

	//for(const auto& sp : space)
	for(int num=0; num <space.size(); num++)
	{
		C_free[num].coord = space[num];
		C_free[num].flag = true;
		bool AnyCollision = true;
		shape->setCenterpoint(space[num]);

		// Intersection judge aginst wall
		if(space[num].y <= shape->getRadius()){
			AnyCollision = shape->Intersection(wall->geometry);
			if(!AnyCollision){
				//std::cout << "The object collide with wall" << std::endl;
				C_free[num].flag = false;
				continue;
			}
		}

		Point2D proj(space[num].x, space[num].y);
		
		// Intersection judge against left hand
		for(int l=0; l<robot->Lhand.size(); ++l)
		{
			double distance = distance_of_centers(proj, robot->Lhand[l]);
			if(distance < (robot->Lhand[l].getRadius() + shape->getRadius()) )
			{
				AnyCollision = shape->Intersection(robot->Lhand[l].geometry);
				if(!AnyCollision){
					//std::cout << "The object collide with left hand" << std::endl;
					C_free[num].flag = false;
				}
			}
		}
		if(!C_free[num].flag)	continue;

		// Intersection judge against right hand
		for(int r=0; r<robot->Rhand.size(); ++r)
		{
			double distance = distance_of_centers(proj, robot->Rhand[r]);
			if(distance < (robot->Rhand[r].getRadius() + shape->getRadius()) )
			{
				AnyCollision = shape->Intersection(robot->Rhand[r].geometry);
				if(!AnyCollision){
					//std::cout << "The object collide with right hand" << std::endl;
					C_free[num].flag = false;
				}
			}
		}
		if(!C_free[num].flag)	continue;
		
	}


	auto end = std::chrono::system_clock::now();
	auto dur = end - start;
	auto sec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	std::cout << "get_C_free [ms]: ";	std::cout << sec <<std::endl;

////////////////////////////////////////////////////////
// "Print C_free"
/*
	if(cnt % 1 == 0){
	std::string fn = "../log/C_freex.txt";
	std::string fn2 = "../log/C_freey.txt";
	std::ofstream log(fn, std::ios::out);
	std::ofstream log2(fn2, std::ios::out);
	int tmpx = -1000;
	int tmpy = -1000;
	for(int i=0; i<C_free.size(); ++i){
		if(tmpx != C_free[i].x || tmpy != C_free[i].y){
			log << C_free[i].x;	log << " ";
			tmpx = C_free[i].x;
			tmpy = C_free[i].y;
		}
	}
	tmpx = -1000;
	tmpy = -1000;
	for(int i=0; i<C_free.size(); ++i){
		if(tmpx != C_free[i].x || tmpy != C_free[i].y){
			log2 << C_free[i].y;	log2 << " ";
			tmpx = C_free[i].x;
			tmpy = C_free[i].y;
		}
	}
	}
	++cnt;
//	for(int i=0; i<C_free.size(); ++i){
//		std::cout << C_free[i].x;	std::cout <<",";
//		std::cout << C_free[i].y << std::endl;
//	}
*/
//////////////////////////////////////////////////////////

}	


bool Configuration::get_clustered_C_free()
{
	auto start = std::chrono::system_clock::now();
	
	int widx = (top.x-bottom.x)/range_x + 1;
	int widy = (top.y-bottom.y)/range_y + 1;
	int width = (top.th-bottom.th)/range_th + 1;
	
	int ***label = new int**[widx];
	for(int i=0; i<widx; i++){
		label[i] = new int*[widy];
		for(int j=0; j<widy; j++){
			label[i][j] = new int[width];
		}
	}

	int clust = labeling3D(label);
	clustered_C_free.resize(clust);
	for(int i=0; i<clust; i++)	clustered_C_free[i].reserve(space.size());
	for(int i=0; i<widx; i++){
		for(int j=0; j<widy; j++){
			for(int k=0; k<width; k++){
				int area = label[i][j][k];
				if(area != 0){
					clustered_C_free[area-1].push_back(space[coord_to_space(i,j,k)]);
				}
			}
		}
	}

	for(int i=0; i<widx; i++){
		for(int j=0; j<widy; j++){
			delete[] label[i][j];
		}
		delete[] label[i];
	}
	delete[] label;


// Below, clustering by using DBSCAN of MLPACK===============================================
/*
	const double epsilon = 15;
	const size_t minPoints = 1;

	arma::mat C_free_arma = vec_to_arma(C_free);
	arma::Row<size_t> list;

	mldb::DBSCAN<>* myScan = new mldb::DBSCAN<>(epsilon, minPoints);
	
	myScan->Cluster(C_free_arma, list);

	delete myScan;

	int i = 0;
	while(1)
	{
		std::vector<State3D<int>> tmpvec;
		for(int j=0; j<list.n_elem; ++j){
			if(list(j) == i){
				State3D<int> tmp(static_cast<int>(C_free_arma(0, j)), 
												 static_cast<int>(C_free_arma(1, j)), 
												 static_cast<int>(C_free_arma(2, j)) );

				tmpvec.push_back(tmp);
			}
		}
		if(tmpvec.size() == 0)	break;
		clustered_C_free.push_back(tmpvec);
		++i;
	}
*/ 
// ===========================================================================================================-

	if(clustered_C_free.size() == 0){
		clustered_C_free.clear();
		C_free.clear();
		return false;
	}

	auto end = std::chrono::system_clock::now();
	auto dur = end - start;
	auto sec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	std::cout << "get_clustered_C_free [ms]: ";	std::cout << sec <<std::endl;

	return true;

}

bool Configuration::get_C_free_ICS()
{
	auto start = std::chrono::system_clock::now();
	
	for(auto itr=clustered_C_free.begin(); itr != clustered_C_free.end(); )
	{
		int erase_flag = 0;
		for(int i=0; i<p_far.size(); ++i){
			auto result = std::find( itr->begin(), itr->end(), p_far[i] );
			if(result == itr->end()){
				// Not found
				continue;
			}
			else{
				// found
				erase_flag = 1;
				break;
			}
		}

		if(erase_flag == 1){
			itr = clustered_C_free.erase(itr);
		}
		else{
			++itr;
		}

	}

	// marge 0 with 360 deg
//	int tmpx = -50;
//	int tmpy = -50;
//	std::cout << std::endl << "clustered_C_free:" << std::endl;
//	for(int i=0; i<clustered_C_free.size(); ++i){
//		for(int j=0; j<clustered_C_free[i].size(); j++){
//			if(tmpx!=clustered_C_free[i][j].x || tmpy!=clustered_C_free[i][j].y){
//				std::cout << clustered_C_free[i][j].x;	std::cout << ",";
//				std::cout << clustered_C_free[i][j].y << std::endl;
//				tmpx = clustered_C_free[i][j].x;	tmpy = clustered_C_free[i][j].y;
//			}
//		}
//		std::cout << std::endl;
//	}


	auto end = std::chrono::system_clock::now();
	auto dur = end - start;
	auto sec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	std::cout << "get_C_free_ICS [ms]: ";	std::cout << sec <<std::endl;

	if( clustered_C_free.size() == 0){
		clustered_C_free.clear();
		C_free.clear();
		return false;
	}

	merge();
	return true;

}

bool Configuration::get_C_free_obj(const std::vector<State3D<int>>& last_C_free_obj)
{
	std::string fn = "../C_free_obj.csv";
	//=================================================================================================
	std::ofstream ofs(fn, std::ios::out);
	for(const auto& last: last_C_free_obj){
		ofs << last.x;	ofs << ",";	ofs << last.y;	ofs << ",";		ofs << last.th << std::endl;
	}
	ofs << " " << "," << " " << "," << " " << std::endl;
	for(const auto& cc: clustered_C_free){
		for(const auto& last: cc){
			ofs << last.x;	ofs << ",";	ofs << last.y;	ofs << ",";		ofs << last.th << std::endl;
		}
		ofs << " " << "," << " " << "," << " " << std::endl;
	}
	
	//================================================================================================


	auto start = std::chrono::system_clock::now();
	for(auto itr=clustered_C_free.begin(); itr != clustered_C_free.end();)
	{
		int erase_flag = 1;
		for(int i=0; i<last_C_free_obj.size(); ++i){
			auto result = std::find( itr->begin(), itr->end(), last_C_free_obj[i] );
			if(result == itr->end()){
				// not found
				continue;
			}
			else{
				// found
				erase_flag = 0;
				break;
			}
		}

		// Finally, Not found -> erase this cluster
		if(erase_flag == 1){
			itr = clustered_C_free.erase(itr);
		}
		else{
			++itr;
		}
	
	}

	// for(const auto& cls: clustered_C_free){
	// 	for(const auto& obj: cls){
	// 	ofs << obj.x;	ofs << ",";	ofs << obj.y;	ofs << ",";		ofs << obj.th << std::endl;
	// 	}
	// }

	auto end = std::chrono::system_clock::now();
	auto dur = end - start;
	auto sec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	std::cout << "get_clustered_C_obj [ms] : ";	std::cout << sec << std::endl;

	if(clustered_C_free.size() == 0){
		std::cout << "The number of cluster is 0" <<std::endl;
		clustered_C_free.clear();
		C_free.clear();
		return false;
	}
	else if(clustered_C_free.size() >= 2){
		std::cout << "The number of cluster is ";	std::cout << clustered_C_free.size() << std::endl;
		clustered_C_free.clear();
		C_free.clear();
		return false;
	}
	else{
		C_free_obj.clear();
		C_free_obj = clustered_C_free[0];
		clustered_C_free.clear();
		C_free.clear();
	}

	return true;

}

void Configuration::merge()
{
	// when ICS process ends, this function is executed.

	// To speed up, prepare new vector which contains the values at theta = 0[deg] or 360[deg]
	// So, empty element may be created
	int N = clustered_C_free.size();
	std::vector<std::vector<State3D<int>>> cluster_mirror(N);
	std::vector<int> cluster_index(N);
	for(int i=0; i<N; ++i){
		cluster_index[i] = i;
		for(int j=0; j<clustered_C_free[i].size(); ++j){
			if(clustered_C_free[i][j].th == 0){
				cluster_mirror[i].push_back(clustered_C_free[i][j]);
			}
			if(clustered_C_free[i][j].th == 360){
				cluster_mirror[i].push_back(clustered_C_free[i][j]);
			}
		}
	}


	// If some elements is empty, erase the element
	auto inditr = cluster_index.begin();
	for(auto itr = cluster_mirror.begin(); itr != cluster_mirror.end();)
		if(itr->size() == 0){
			itr = cluster_mirror.erase(itr);
			inditr = cluster_index.erase(inditr);
		}
		else{
			itr++;
			inditr++;
		}

	// If cluster don't contain the values at theta = 0 of 360[deg], 
	// there is no need to merge, and so, function is returned.
	if(cluster_mirror.size() == 0)	return;


	// decide which cluster is merged with which cluster
	int	mirrorN = cluster_mirror.size();
	std::vector<std::array<int,2>> merge_set;
	merge_set.reserve(mirrorN*(mirrorN-1)/2);
	int count = 0;
	for(int i=0; i<mirrorN-1; i++){
	for(int j=i+1; j<mirrorN; j++){
		
		int exflg = 0;
		for(const auto& clsi: cluster_mirror[i]){
		for(const auto& clsj: cluster_mirror[j]){
			if(clsi.x == clsj.x && clsi.y == clsj.y && clsi.th != clsj.th){
				std::array<int,2> tmpset = {cluster_index[i], cluster_index[j]};
				merge_set.push_back(tmpset);
				++count;
				exflg = 1;
				break;
			}
		}
		if(exflg == 1)	break;
		}

	}
	}


	// merge here
	for(int i=(int)(merge_set.size())-1; i>=0; i--){
		clustered_C_free[merge_set[i][0]].insert(clustered_C_free[merge_set[i][0]].end(), 
		clustered_C_free[merge_set[i][1]].begin(), clustered_C_free[merge_set[i][1]].end());
		clustered_C_free.erase(clustered_C_free.begin() + merge_set[i][1]);
	}

	return ;
}


double Configuration::toGoal(State3D<int> goal)
{
	Shape* rect = Shape::getInstance();
	const double symmetric_range = rect->symmetric_angle;  
	double dist = DBL_MIN;
	double tmpx,tmpy,tmpth;
	for(auto itr=C_free_obj.begin(); itr != C_free_obj.end(); ++itr)
	{
		double x_diff2  = (itr->x  - goal.x)*(itr->x  - goal.x);
		double y_diff2  = (itr->y  - goal.y)*(itr->y  - goal.y);

	// For theta, I have to consider geometric symmetry
		// Goal increasing along symmetric angle
		std::vector<int> goal_theta;
		goal_theta.push_back(goal.th);
		int tmpg = goal.th;
		tmpg -= symmetric_range;
		while(tmpg >= 0){
			goal_theta.push_back(tmpg);
			tmpg -= symmetric_range;
		}
		tmpg = goal.th;
		tmpg += symmetric_range;
		while(tmpg <= 360){
			goal_theta.push_back(tmpg);
			tmpg += symmetric_range;
		}

		// calculate minimum
		double th_diff2 = DBL_MAX;
		for(int i=0; i<goal_theta.size(); ++i){
			double tmp = (itr->th - goal_theta[i])*(itr->th - goal_theta[i]);
			if(th_diff2 > tmp)	th_diff2 = tmp;
		}

		double tmpdst = std::sqrt(x_diff2 + y_diff2 + th_diff2);

		// dist will be max value.
		if(dist < tmpdst){
			dist = tmpdst;
			tmpx = itr->x;	tmpy = itr->y;	tmpth = itr->th;
		}

	}
	
	return dist;
}

double Configuration::toGoal(Node start, Node goal)
{
	double d = 0.0;
	for(int i=0; i<Node::dof; i++){
		d += (start.node[i]-goal.node[i])*(start.node[i]-goal.node[i]);
	}

	return std::sqrt(d);
}



int Configuration::labeling3D(int*** label)
{
	// Initialize
	for(int x = bottom.x; x <= top.x; x = x + range_x)
		for(int y = bottom.y; y <= top.y; y = y + range_y)
			for(int th = bottom.th; th <= top.th; th = th + range_th){
				int indx = (x-bottom.x)/range_x;
				int indy = (y-bottom.y)/range_y;
				int indth = (th-bottom.th)/range_th;
				label[indx][indy][indth] = 0;
			}
		
	
	// Labeling
	int count = 0;
	for(int x = bottom.x; x <= top.x; x = x + range_x)
		for(int y = bottom.y; y <= top.y; y = y + range_y)
			for(int th = bottom.th; th <= top.th; th = th + range_th){
				int indx = (x-bottom.x)/range_x;
				int indy = (y-bottom.y)/range_y;
				int indth = (th-bottom.th)/range_th;
				if(C_free[coord_to_space(indx,indy,indth)].flag == true && label[indx][indy][indth] == 0){
					int clust = search_neighbors(indx, indy, indth, label);
					if(clust == 0)	// New area
						label[indx][indy][indth] = ++count;
					else
						label[indx][indy][indth] = clust;
				}
			}
		
	// Integrating	
	int new_count = 0;
	if(count > 0){
		for(int x = bottom.x; x <= top.x; x = x + range_x)
			for(int y = bottom.y; y <= top.y; y = y + range_y)
				for(int th = bottom.th; th <= top.th; th = th + range_th){
					int indx = (x-bottom.x)/range_x;
					int indy = (y-bottom.y)/range_y;
					int indth = (th-bottom.th)/range_th;
					if(label[indx][indy][indth] != 0){
						int clust = search_neighbors(indx, indy, indth, label);
						if(clust > label[indx][indy][indth])
							modify_label(clust, label[indx][indy][indth], label);
					}
				}
				
		// Ordering	
		for(int x = bottom.x; x <= top.x; x = x + range_x)
			for(int y = bottom.y; y <= top.y; y = y + range_y)
				for(int th = bottom.th; th <= top.th; th = th + range_th){
					int indx = (x-bottom.x)/range_x;
					int indy = (y-bottom.y)/range_y;
					int indth = (th-bottom.th)/range_th;
					if(label[indx][indy][indth] > new_count){
						new_count++;
						modify_label(label[indx][indy][indth], new_count, label);
					}
				}
			
		
	}

	return new_count;
}


// x,y,th is not coordinate but index
int Configuration::coord_to_space(int x, int y, int th)
{	
	int index = 0;

	index += x*((top.y-bottom.y)/range_y + 1)*((top.th-bottom.th)/range_th + 1);
	index += y*((top.th-bottom.th)/range_th + 1);
	index += th;

	return index;
}

int Configuration::search_neighbors(int x, int y, int th, int*** label)
{
	int max = 0;
	// search neighbor 26 area
	int topx = (top.x-bottom.x)/range_x;
	int topy = (top.y-bottom.y)/range_y;
	int topth = (top.th-bottom.th)/range_th;

// ======================================================================================= 
	if(x-1 >= 0){
		if(y-1 >= 0){
			if(th-1 >= 0 && label[x-1][y-1][th-1]  > max)
				max = label[x-1][y-1][th-1];
			if(label[x-1][y-1][th] > max)
				max = label[x-1][y-1][th];
			if(th+1 <= topth && label[x-1][y-1][th+1] > max)
				max = label[x-1][y-1][th+1];
		}

		if(th-1 >= 0 && label[x-1][y][th-1] > max)
			max = label[x-1][y][th-1];
		if(label[x-1][y][th] > max)
			max = label[x-1][y][th];
		if(th+1 <= topth && label[x-1][y][th+1] > max)
			max = label[x-1][y][th+1];

		if(y+1 <= topy){
			if(th-1 >= 0 && label[x-1][y+1][th-1]  > max)
				max = label[x-1][y+1][th-1];
			if(label[x-1][y+1][th] > max)
				max = label[x-1][y+1][th];
			if(th+1 <= topth && label[x-1][y+1][th+1] > max)
				max = label[x-1][y+1][th+1];
		}
	}

//=========================================================================================
	if(y-1 >= 0){
		if(th-1 >= 0 && label[x][y-1][th-1] > max)
			max = label[x][y-1][th-1];
		if(label[x][y-1][th] > max)
			max = label[x][y-1][th];
		if(th+1 <= topth && label[x][y-1][th+1] > max)
			max = label[x][y-1][th+1];
	}

	if(th-1 >= 0 && label[x][y][th-1] > max)
		max = label[x][y][th-1];
//	here center	
	if(th+1 <= topth && label[x][y][th+1] > max)
		max = label[x][y][th+1];

	if(y+1 <= topy){
		if(th-1 >= 0 && label[x][y+1][th-1] > max)
			max = label[x][y+1][th-1];
		if(label[x][y+1][th] > max)
			max = label[x][y+1][th]; 
		if(th+1 <= topth && label[x][y+1][th+1] > max)
			max = label[x][y+1][th+1];
	}

//==================================================================================================
	if(x+1 <= topx){
		if(y-1 >= 0){
			if(th-1 >= 0 && label[x+1][y-1][th-1] > max)
				max = label[x+1][y-1][th-1];
			if(label[x+1][y-1][th] > max)
				max = label[x+1][y-1][th]; 
			if(th+1 <= topth && label[x+1][y-1][th+1] > max)
				max = label[x+1][y-1][th+1];
		}

		if(th-1 >= 0 && label[x+1][y][th-1] > max)
			max = label[x+1][y][th-1];
		if(label[x+1][y][th] > max)
			max = label[x+1][y][th]; 
		if(th+1 <= topth && label[x+1][y][th+1] > max)
			max = label[x+1][y][th+1];

		if(y+1 <= topy){
			if(th-1 >= 0 && label[x+1][y+1][th-1] > max)
				max = label[x+1][y+1][th-1];
			if(label[x+1][y+1][th] > max)
				max = label[x+1][y+1][th]; 
			if(th+1 <= topth && label[x+1][y+1][th+1] > max)
				max = label[x+1][y+1][th+1];
		}
	}

	return max;
}

void Configuration::modify_label(int num1, int num2, int*** label)
{	
	for(int x = bottom.x; x <= top.x; x = x + range_x)
		for(int y = bottom.y; y <= top.y; y = y + range_y)
			for(int th = bottom.th; th <= top.th; th = th + range_th){
				int indx = (x-bottom.x)/range_x;
				int indy = (y-bottom.y)/range_y;
				int indth = (th-bottom.th)/range_th;
				if(label[indx][indy][indth] == num1)	label[indx][indy][indth] = num2;
			}
}

double distance_of_centers(const Point2D point, const Link link)
{
	double rad = deg_to_rad(link.AbsAngle);
	Point2D cen(link.refPoint.x + ( link.width*std::cos(rad) - link.height*std::sin(rad))/2 ,
							link.refPoint.y + ( link.width*std::sin(rad) + link.height*std::cos(rad))/2);
	return cen.dist(point);
}


// arma::mat vec_to_arma(std::vector<State3D<int>> vec)
// {	
// 	arma::mat arma(3, vec.size(), arma::fill::zeros);
// 	for(size_t i=0; i<vec.size(); ++i)
// 	{
// 		arma(0, i) = vec[i].x;
// 		arma(1, i) = vec[i].y;
// 		arma(2, i) = vec[i].th;
// 	}
	
// 	return arma;
// }


