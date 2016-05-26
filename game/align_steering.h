#ifndef _ALIGN_STEERING_H_
#define _ALIGN_STEERING_H_

#include "steering.h"

class AlignSteering: public Steering {
public:
	AlignSteering();
	virtual ~AlignSteering();

	virtual void Init(Character * ch);

	virtual void DrawDebug();

	virtual void Update(Accelerations &acc, USVec2D target);
private:
};

#endif //!_ALIGN_STEERING_H_