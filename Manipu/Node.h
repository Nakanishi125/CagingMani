#pragma once

#include <vector>
#include <array>
#include <cmath>

#include "maniMath.h"

class Node
{
public:
	static int Ldof;
	static int Rdof;
	static int dof;

	std::vector<double> node;
	std::vector<State3D<int>> region;

	int parent;		// 添え字を保持する

	//　コンストラクタ
	Node(std::vector<double> Linit, std::vector<double> Rinit);
	Node(std::vector<double> node_);


	double distance(Node other);

};

