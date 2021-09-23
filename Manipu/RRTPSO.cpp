#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <cstdlib>
#include <fstream>

#include "RRTPSO.h"

using namespace std::chrono;

RRTPSO::RRTPSO(Node start, State3D<int> goal, double epsilon)
	:Algorithm(start, goal, epsilon), range(1.0)
{
	graph.push_back(Start);
}

void RRTPSO::planning()
{
	auto start = std::chrono::system_clock::now();
	static int all = 0;
	static int real = 0;
	while(1){
		++all;
		// ランダムに点を打つ
		std::vector<double> Rand(Node::dof);
		auto seed = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count() % 100000;
		std::srand(seed);
		for(int i=0; i<Node::dof; i++){
			auto seed = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count() % 100000;
			std::srand(seed);	
			Rand[i] = std::rand()%181 - 90;
		}

		Node nownode(Rand);

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
		
		if(!robot->rrIntersect()){
			std::cout << "robot-robot Intersection" << std::endl << std::endl;
			continue;
		}
		if(!robot->rwIntersect(wall)){
			std::cout << "robot-wall Intersection" << std::endl << std::endl;
			continue;
		}

		Configuration* config_ = Configuration::getInstance();

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

		static double mini = DBL_MAX;
		double kyori = config_->toGoal(Goal);
		if(mini > kyori){
			mini = kyori;
		}
		std::cout << "Now, minimum distance is ";	std::cout << mini << std::endl << std::endl;
		if(epsilon > kyori)	break;

		// if(epsilon > config_->toGoal(Goal))	break;
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

	std::cout << "all:";	std::cout << all << std::endl;
	std::cout << "real:";	std::cout << real << std::endl;
	std::cout << "Rate is ";	std::cout << (100.0*real)/all;	std::cout << " (%)" << std::endl;
}

void RRTPSO::WriteToFile()
{
	std::string fn = "../path.csv";
	std::ofstream ofs(fn, std::ios::out);

	for(const auto& node: path){
		for(int i=0; i<Node::dof; i++){
			if(i == Node::dof - 1){
				ofs << node.node[i] << std::endl;
			}
			else{
				ofs << node.node[i];	ofs << ",";
			}
		}
	}

}

void RRTPSO::PSO()
{
    float scope_min = -90;
    float scope_max = 90;

// 粒子の位置，速度の初期化
    std::array<std::vector<double>, number_of_particles> particles_pos;
    std::array<std::vector<double>, number_of_particles> particles_vel;
    for(int i=0; i<number_of_particles; ++i){
        std::vector<double> Ranpos(dim);
        std::vector<double> Ranvel(dim);
        for(int j=0; j<dim; ++j){
        auto seed = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count() % 100000;
        std::srand(seed);	
        Ranpos[j] = std::rand()*(scope_max-scope_min)/RAND_MAX + scope_min;
        Ranvel[j] = 0;
        }
        particles_pos[i] = Ranpos;
        particles_vel[i] = Ranvel;
    }

    

}