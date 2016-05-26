#include "stdafx.h"
#include "character.h"
#include "obstacleavoidance_steering.h"
ObstacleAvoidanceSteering::ObstacleAvoidanceSteering() {}

void ObstacleAvoidanceSteering::Init(Character * ch) {
	Steering::Init(ch);
}

ObstacleAvoidanceSteering::~ObstacleAvoidanceSteering() {
}

void ObstacleAvoidanceSteering::Update(Accelerations & acc, USVec2D target) {
	Character * ch = GetCh();
	mVision = ch->GetLinearVelocity().NormVector() * ch->GetParams().vision_length;

	USVec2D vObsCenter;
	float obsDistance, projDistance;
	for (std::vector<Obstacle>::iterator obIt = ch->GetParams().obstacles.begin();
	obIt != ch->GetParams().obstacles.end(); ++obIt) {
		vObsCenter = USVec2D(obIt->x, obIt->y) - ch->GetLoc();
		obsDistance = vObsCenter.Length();
		projDistance = (mVision - vObsCenter).Length();
		if (projDistance < obIt->r + ch->GetParams().char_radius) {
			std::cout << "COLLISION" << std::endl;
			acc.linearAcc = (mVision - vObsCenter).NormVector() * ch->GetParams().max_acceleration;
		} else {
			std::cout << "no collision" << std::endl;
		}
	}
}

void ObstacleAvoidanceSteering::DrawDebug() {
	Character * ch = GetCh();
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);

	for (std::vector<Obstacle>::iterator obIt = ch->GetParams().obstacles.begin();
	obIt != ch->GetParams().obstacles.end(); ++obIt) {
		gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);
		MOAIDraw::DrawEllipseFill(obIt->x, obIt->y, obIt->r, obIt->r, 64);
		USVec2D obs = USVec2D(obIt->x, obIt->y);
		USVec2D chPos = ch->GetLoc();
		USVec2D vToObs = obs - chPos;
		gfxDevice.SetPenColor(1.0f, 1.0f, 0.0f, 0.5f);
		MOAIDraw::DrawLine(static_cast<USVec2D>(ch->GetLoc()),
			static_cast<USVec2D>(ch->GetLoc() + vToObs));
	}

	//line of sight
	gfxDevice.SetPenColor(0.0f, 1.0f, 1.0f, 0.5f);
	MOAIDraw::DrawLine(ch->GetLoc(), ch->GetLoc() + mVision);
}