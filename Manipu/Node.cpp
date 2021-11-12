#include "Node.h"

int Node::Ldof = 0;
int Node::Rdof = 0;
int Node::dof = 0;

Node::Node(std::vector<double> Linit, std::vector<double> Rinit)
    :parent(-1)
{
    Ldof = Linit.size();
    Rdof = Rinit.size();
    dof = Ldof + Rdof;
    for(int i=0; i<Ldof; i++)   node.push_back(Linit[i]);
    for(int i=0; i<Rdof; i++)   node.push_back(Rinit[i]);
    // State3D<int> aa(150, 180, 310);
	// State3D<int> bb(160, 180, 310);
	// State3D<int> cc(170, 190, 320);
	// std::vector<State3D<int>> C = {aa, bb, cc};
    // region = C;
}

Node::Node(std::vector<double> node_)
    :node(node_),parent()
{
}

Node::Node()
    :node(), parent()
{}

void Node::Update(std::vector<double> node_)
{
    node = node_;
    parent = 0;
    region.clear();
}

double Node::distance(Node other)
{
    double dist = 0.0;
    for(int i=0; i<(int)(this->node.size()); ++i){
        dist += (this->node[i] - other.node[i])*(this->node[i] - other.node[i]);
    }
    return std::sqrt(dist);
}