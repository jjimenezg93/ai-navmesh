#ifndef _SEEK_STEERING_H_
#define _SEEK_STEERING_H_

#include "steering.h"

class SeekSteering:	public Steering {
public:
	SeekSteering();
	virtual ~SeekSteering();

	virtual void Init(Character * ch);

	void DrawDebug();

	virtual void Update(Accelerations &acc, USVec2D target);
private:
	USVec2D mLastLinearAcc;
};

#endif //!_SEEK_STEERING_H_