// #include <ompl/base/SpaceInformation.h>
// #include <ompl/base/StateValidityChecker.h>
// #include <ompl/base/State.h>

// #include "Configuration.h"
// #include "Robot.h"

// namespace ob = ompl::base;

// class ValidityChecker : public ob::StateValidityChecker
// {
// public:
// 	ValidityChecker(const ob::SpaceInformationPtr& si)
// 		:ob::StateValidityChecker(si)
// 	{
// 	}

// 	bool isValid(const ob::State* state) const
// 	{
// 		const ob::RealVectorStateSpace::StateType* ss = 
// 			state->as<ob::RealVectorStateSpace::StateType>();

// 		std::vector<double> left ={ ss->values[0],
// 																ss->values[1],
// 																ss->values[2]};
// 		std::vector<double> right={	ss->values[3],
// 																ss->values[4],
// 																ss->values[5]};
// 		std::vector<double> org= {};

// 		Node node_(left,right,org);	
		
// 		Robot* robot = Robot::getInstance();
// 		Wall* wall = Wall::getInstance();

// 		robot->Update(node_);
// 		if(!robot->rrIntersect()){
// 			std::cout << "robot-robot Intersection" << std::endl << std::endl;
// 			return false;
// 		}
// 		if(!robot->rwIntersect(wall)){
// 			std::cout << "robot-wall Intersection" << std::endl << std::endl;
// 			return false;
// 		}
		
// 		Configuration* config_ = Configuration::getInstance();

// //=== Confirmation part by using std::cout =======================
// 		static int count;
// 		count++;
// 		std::cout << "No. ";	std::cout << count << std::endl;
// 		for(int i=0;i<left.size();++i){
// 			std::cout << left[i];	std::cout << " , ";
// 		}
// 		for(int i=0;i<right.size();++i){
// 			std::cout << right[i];	std::cout << " , ";
// 		}
// 		std::cout << std::endl;

// //================================================================

// 		// whether C_free_obj exists or not
// 		config_->get_C_free();

// 		if(!config_->get_clustered_C_free()){
// 			std::cout << "cluster false" << std::endl << std::endl;
// 			return false;
// 		}

// 		if(!config_->get_C_free_ICS()){
// 			std::cout << "ICS false" << std::endl << std::endl;
// 			return false;
// 		}

// 		if(!config_->get_C_free_obj()){
// 			std::cout << "C_free_obj false" << std::endl << std::endl;
// 			return false;
// 		}

// 		return true;
// 	}

// 	double clearance(const ob::State* state) const
// 	{
// 		return 5.0;
// 	}

// };
