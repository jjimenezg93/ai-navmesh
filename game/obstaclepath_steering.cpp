#include "stdafx.h"
#include "character.h"
#include "pathfollowing_steering.h"
#include "obstacleavoidance_steering.h"
#include "obstaclepath_steering.h"

ObstaclePathSteering::ObstaclePathSteering() {}

ObstaclePathSteering::~ObstaclePathSteering() {
}

void ObstaclePathSteering::Init(Character * ch) {
	Steering::Init(ch);
	mObsAvoid = new ObstacleAvoidanceSteering();
	mObsAvoid->Init(ch);
	mPathFollow = new PathFollowingSteering();
	mPathFollow->Init(ch);
}


void ObstaclePathSteering::Update(Accelerations & acc, USVec2D target) {
	Accelerations localAccPath;
	Accelerations localAccObsAv;

	mPathFollow->Update(localAccPath, target);
	mObsAvoid->Update(localAccObsAv, target);

	if (roundf(localAccObsAv.linearAcc.Length()) == 0) {
		acc.linearAcc = localAccPath.linearAcc;
	} else {
		acc.linearAcc = localAccPath.linearAcc * 0.4 + localAccObsAv.linearAcc * 0.6;
	}
}

void ObstaclePathSteering::DrawDebug() {
	mObsAvoid->DrawDebug();
	mPathFollow->DrawDebug();
}