#ifndef _OBSTACLE_PATH_H_
#define _OBSTACLE_PATH_H_

#include <vector>

#include "steering.h"

class PathFollowingSteering;
class ObstacleAvoidanceSteering;

class ObstaclePathSteering: public Steering {
public:
	ObstaclePathSteering();
	virtual ~ObstaclePathSteering();

	virtual void Init(Character * ch);

	virtual void Update(Accelerations &acc, USVec2D target);

	virtual void DrawDebug();
protected:
	ObstacleAvoidanceSteering * mObsAvoid;
	PathFollowingSteering * mPathFollow;
};

#endif //!_OBSTACLE_PATH_H_