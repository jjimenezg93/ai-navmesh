#ifndef _PATH_FOLLOWING_H_
#define _PATH_FOLLOWING_H_

#include "steering.h"
class SeekSteering;

class PathFollowingSteering: public Steering {
public:
	PathFollowingSteering();
	virtual ~PathFollowingSteering();
	
	virtual void Init(Character * ch);

	virtual void Update(Accelerations &acc, USVec2D target);
	
	virtual void DrawDebug();
protected:
	uint16_t mCurrentSegment;
	USVec2D mCurrentTarget;
	SeekSteering * mSeekDelegate;
	USVec2D mTarget;
};

#endif //!_PATH_FOLLOWING_H_