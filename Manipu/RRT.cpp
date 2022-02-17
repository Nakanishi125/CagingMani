#pragma warning(disable : 4996)

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <signal.h>

#include "csv.hpp"
#include "RRT.h"
#include "ProblemFactory.h"

using namespace std::chrono;

Node nownode;

RRT::RRT(Node start)
	:Algorithm(start), range(1.0)
{
	Start = start;
	graph.push_back(start);
	gc = ProblemFactory::create();
} 

bool RRT::Initialize()
{
	Robot* robot = Robot::getInstance();
	Wall* wall = Wall::getInstance();
	Configuration* config_ = Configuration::getInstance();

	robot->Update(Start);
	
	if(robot->rrIntersect()){
		std::cout << "robot-robot Intersection" << std::endl << std::endl;
		return false;
	}
	if(robot->rwIntersect(wall)){
		std::cout << "robot-wall Intersection" << std::endl << std::endl;
		return false;
	}

	config_->get_C_free();
	if(!config_->get_clustered_C_free()){
		std::cout << "cluster false" << std::endl << std::endl;
		return false;
	}
	if(!config_->get_C_free_ICS()){
		std::cout << "ICS false" << std::endl << std::endl;
		return false;
	}

	int num=0;
	for(const auto& cls: config_->clustered_C_free){
		std::cout << num;	std::cout << ":" << std::endl;
		for(int i=0; i<cls.size(); ++i){
			std::cout << "[";	std::cout << cls[i].x;	std::cout << ",";
			std::cout << cls[i].y;	std::cout << ",";	std::cout << cls[i].th;	std::cout << "],";
			if(i>4)	break;
		}
		std::cout << std::endl << std::endl;
		++num;
	}

	std::cout << "Select the cluster:";
	std::cin >> num;
	graph[0].region = config_->clustered_C_free[num];
	return true;
}


void RRT::planning()
{
	auto start = std::chrono::system_clock::now();
	static int all = 0;
	static int real = 0;
	Configuration* config_ = Configuration::getInstance();

	auto seed = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count() % 100000;
	std::srand(seed);
	std::cout << "Seed value is " << seed << std::endl;
	if(!Initialize()){
		std::cout << "Invalid initial value was given." << std::endl;
		return ;
	}

	while(1){
		++all;
		std::cout << "No.";	std::cout << all << std::endl;
		// ランダムに点を打つ
		std::vector<double> Rand(Node::dof);
		if(all%1 == 0){
			// auto seed = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count() % 100000;
			// std::srand(seed);
			for(int i=0; i<Node::dof; i++){
				// auto seed = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count() % 100000;
				// std::srand(seed);	
				Rand[i] = std::rand()%181 - 90;
			}
		}
		else{
		// 	std::cout << "Goal Bias this time" << std::endl;
		// 	Rand = dynamic_cast<GoalAngle*>(gc)->dest;
		}

		nownode.Update(Rand);

		// 最近傍ノードを探索
		double dist = DBL_MAX;
		int nearest_index = -1;
		for(int i=0; i<(int)(graph.size()); ++i)
		{
			double tmp = nownode.distance(graph[i]);
			if(dist > tmp){
				nearest_index = i;
				dist = tmp;
			}
		}
		nownode.parent = nearest_index;

		// 前ノード -> 今ノードのベクトルのノルムをrangeになるように縮尺
		// 新ノードの各値を算出
		for(int i=0; i<nownode.dof; ++i){
			float trans = range*(nownode.node[i] - graph[nownode.parent].node[i])/dist;
			nownode.node[i] = graph[nownode.parent].node[i] + trans;
		}

		for(int i=0; i<nownode.dof; ++i){
			std::cout << nownode.node[i];
			std::cout << ", ";
		}
		std::cout << std::endl;
		
		//　ノードの妥当性判定処理
		Robot* robot = Robot::getInstance();
		Wall* wall = Wall::getInstance();

		robot->Update(nownode);
		
		if(robot->rrIntersect()){
			std::cout << "    robot-robot Intersection" << std::endl << std::endl;
			continue;
		}
		if(robot->rwIntersect(wall)){
			std::cout << "    robot-wall Intersection" << std::endl << std::endl;
			continue;
		}

		config_->get_C_free();
		if(!config_->get_clustered_C_free()){
			std::cout << "    cluster false" << std::endl << std::endl;
			continue;
		}
		if(!config_->get_C_free_ICS()){
			std::cout << "    ICS false" << std::endl << std::endl;
			continue;
		}
		if(!config_->get_C_free_obj(graph[nownode.parent].region)){
			std::cout << "    free_obj false" << std::endl << std::endl;
			continue;
		}
		nownode.region = config_->C_free_obj;
		++real;

		// グラフに今回のノードを追加
		graph.push_back(nownode);
		std::cout << "    The number of nodes is:";	std::cout << graph.size() << std::endl;

		if(gc->judge(real))
			break;
	}

	auto end = std::chrono::system_clock::now();
	auto dur = end - start;
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(dur).count();
	std::cout << "Elapsed time[s]:";	std::cout << sec << std::endl;


	// 経路抽出フェーズ
	int index = graph.size() - 1;
	std::vector<int> route;
	while(1){
		route.push_back(index);
		index = graph[index].parent;
		if(index == -1)	break;
	}
	
	path.clear();
	path.reserve(route.size());
	for(int i=route.size()-1; i>=0; i--){
		path.push_back(graph[route[i]]);
	}
	std::cout << std::endl << "Complete generating path!!" << std::endl;

	std::cout << "Seed value is " << seed << std::endl; 
	std::cout << "all:";	std::cout << all << std::endl;
	std::cout << "real:";	std::cout << real << std::endl;
	std::cout << "Rate is ";	std::cout << (100.0*real)/all;	std::cout << " (%)" << std::endl;

	std::string manilog = "../Manipulation_Log.txt";
	std::ofstream log(manilog, std::ios::app);

	log << getDatetimeStr() << std::endl;	
	log << "    Calculation time -> " << sec << "[s]" << std::endl;	
	log << "    Seed -> " << seed << std::endl;	
	log << "    HitRate -> " << (100.0*real)/all; log << "[%]\n\n\n";
}

void RRT::WriteToFile()
{
	// file setting part
	std::string fp = "../Path/" + getDatetimeStr() + ".csv";
	file.open(fp, std::ios::app);

	bp::ptree pt;
	read_ini("../config/ObjectParameter.ini", pt);
	boost::optional<int> carrier = pt.get_optional<int>("target.shape");
	int sh = carrier.get();
	if(sh == 1)	file << "#,Rectangle" << std::endl;
	if(sh == 2) file << "#,LShape" << std::endl;
	if(sh == 3)	file << "#,Triangle" << std::endl;

	file << "#,Goal," << gc->goal.x << "," << gc->goal.y << "," << gc->goal.th << std::endl;
	file << "#,epsilon," << gc->epsilon << std::endl;


	for(const auto& node: path){
		for(int i=0; i<Node::dof; i++){
			if(i == Node::dof - 1){
				file << node.node[i] << std::endl;
			}
			else{
				file << node.node[i] << ",";
			}
		}
	}

}

void RRT::Debug()
{
	std::string fn = "";
	std::cout << "Input filename for debug -> ";
	std::cin >> fn;
	fn = "../Path/" + fn;
	std::ifstream ifs(fn);
	std::vector<double> Ini;
	std::string val;

	do{
		getline(ifs, val);
	}while(val[0] == '#');
	std::string tmp = "";
	std::istringstream stream(val);
	while(getline(stream, tmp, ',')){
		Ini.push_back(stod(tmp));
	}

	Start = Ini;
	Start.parent = -1;

	static int all = 0;
	static int real = 0;
	Configuration* config_ = Configuration::getInstance();

	Initialize();

	while(getline(ifs, val)){
		std::vector<double> Next;
		std::string tmp = "";
		std::istringstream stream(val);
		while(getline(stream, tmp, ',')){
			Next.push_back(stod(tmp));
		}
		nownode.Update(Next);

		// 最近傍ノードを探索
		double dist = DBL_MAX;
		int nearest_index = -1;
		for(int i=0; i<(int)(graph.size()); ++i)
		{
			double tmp = nownode.distance(graph[i]);
			if(dist > tmp){
				nearest_index = i;
				dist = tmp;
			}
		}
		nownode.parent = nearest_index;

		// Output to console
		for(int i=0; i<nownode.dof; ++i){
			std::cout << nownode.node[i];
			std::cout << ", ";
		}
		std::cout << std::endl;

		//　ノードの妥当性判定処理
		Robot* robot = Robot::getInstance();
		Wall* wall = Wall::getInstance();

		robot->Update(nownode);
		
		if(robot->rrIntersect()){
			std::cout << "robot-robot Intersection" << std::endl << std::endl;
			continue;
		}
		if(robot->rwIntersect(wall)){
			std::cout << "robot-wall Intersection" << std::endl << std::endl;
			continue;
		}

		config_->get_C_free();
		if(!config_->get_clustered_C_free()){
			std::cout << "cluster false" << std::endl << std::endl;
			continue;
		}
		if(!config_->get_C_free_ICS()){
			std::cout << "ICS false" << std::endl << std::endl;
			continue;
		}
		if(!config_->get_C_free_obj(graph[nownode.parent].region)){
			std::cout << "free_obj false" << std::endl << std::endl;
			continue;
		}
		nownode.region = config_->C_free_obj;
		++real;

		// グラフに今回のノードを追加
		graph.push_back(nownode);
		std::cout << "The number of nodes is:";	std::cout << graph.size() << std::endl;

		if(gc->judge(real))
			break;

	}
}

std::string getDatetimeStr() 
{
    time_t t = time(nullptr);
    const tm* localTime = localtime(&t);
    std::stringstream s;
    s << "20" << localTime->tm_year - 100 << "-";
    // setw(),setfill()で0詰め
    s << std::setw(2) << std::setfill('0') << localTime->tm_mon + 1 << "-";
    s << std::setw(2) << std::setfill('0') << localTime->tm_mday << "-";
    s << std::setw(2) << std::setfill('0') << localTime->tm_hour << "-";
    s << std::setw(2) << std::setfill('0') << localTime->tm_min << "-";
    s << std::setw(2) << std::setfill('0') << localTime->tm_sec;
    // std::stringにして値を返す
    return s.str();
}