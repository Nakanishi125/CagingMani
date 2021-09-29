#pragma once

#include <array>

#include "Wall.h"
#include "Shape.h"
#include "robot.h"
#include "maniMath.h"

using StateIte = std::vector<std::vector<State3D<int>>::iterator>;

struct spaceinfo{
	State3D<int> coord;
	bool flag;
};

class Configuration
{
public:
	static Configuration* getInstance();

	void get_C_free();
	bool get_clustered_C_free();
	bool get_C_free_ICS();
	bool get_C_free_obj(const std::vector<State3D<int>>& last_C_free_obj);

	double toGoal(State3D<int> goal);
	double toGoal(Node start, Node goal);

	std::vector<State3D<int>> space;
	std::vector<State3D<int>> C_free_obj;

private:
	State3D<int> top;
	State3D<int> bottom;
	int range_x, range_y, range_th;
	std::vector<State3D<int>> p_far;

	static Configuration* instance;
	Configuration();

	std::vector<spaceinfo> C_free;
	std::vector<std::vector<State3D<int>>> clustered_C_free;
	
	void merge();	

// Below, Clustering method
	int labeling3D(int*** label);
	int coord_to_space(int x, int y, int th);
	int search_neighbors(int x, int y, int th, int*** label);
	void modify_label(int num1, int num2, int*** label);
};

static double distance_of_centers(const Point2D point, const Link link);

//static arma::mat vec_to_arma(std::vector<State3D<int>> vec);

