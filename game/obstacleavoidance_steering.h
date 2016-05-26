#ifndef _OBSTACLE_AVOIDANCE_H_
#define _OBSTACLE_AVOIDANCE_H_

#include <vector>

#include "steering.h"

class ObstacleAvoidanceSteering: public Steering {
public:
	ObstacleAvoidanceSteering();
	virtual ~ObstacleAvoidanceSteering();

	virtual void Init(Character * ch);

	virtual void Update(Accelerations &acc, USVec2D target);

	virtual void DrawDebug();
protected:
	uint16_t mCurrentSegment;
	USVec2D mVision;
};

#endif //!_OBSTACLE_AVOIDANCE_H_