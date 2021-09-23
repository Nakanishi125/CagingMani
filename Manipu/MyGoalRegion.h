// #include <cfloat>

// #include <ompl/base/goals/GoalRegion.h>
// #include <ompl/base/SpaceInformation.h>
// #include <ompl/base/State.h>

// #include "Configuration.h"
// #include "Node.h"

// namespace ompl{
// 	namespace base{
// 		class MyGoalRegion : public GoalRegion
// 		{
// 		private:
// 			State3D<int> Goal;
// 			double epsilon;
// 		public:
// 			MyGoalRegion(const SpaceInformationPtr &si, State3D<int> goal, double eps)
// 				:GoalRegion(si), Goal(goal), epsilon(eps)
// 			{
// 				setThreshold(epsilon);
// 			}
		
// 			// func:isSatisfied is already defined like judging distanceGoal < threshold.

// 			double distanceGoal(const State* st) const
// 			{
// 				Configuration* config_ = Configuration::getInstance();
// 				// compute distance 
// 				double dist = config_->toGoal(Goal);

// //====== print part ==================================================================================
// 				static double nowmin = DBL_MAX;
// 				if(nowmin > dist)	nowmin = dist;
// 				std::cout << std::endl << "now, distance is : ";	std::cout << dist << std::endl;
// 				std::cout << "temporary, minimum distance is : ";	std::cout << nowmin << std::endl << std::endl;
// //====================================================================================================

// 				return dist;
// 			}
					
// 		};
// 	}
// }
