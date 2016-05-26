#include "stdafx.h"
#include "character.h"
#include "seek_steering.h"

SeekSteering::SeekSteering() {

}

void SeekSteering::Init(Character * ch) {
	Steering::Init(ch);
}

SeekSteering::~SeekSteering() {

}

void SeekSteering::Update(Accelerations &acc, USVec2D target) {
	USVec2D chLoc = GetCh()->GetLoc();
	USVec2D desiredVelocity = target - chLoc;
	USVec2D wishAcc = desiredVelocity - GetCh()->GetLinearVelocity();
	float accFactor = wishAcc.Norm();
	acc.linearAcc = wishAcc * accFactor;
	mLastLinearAcc = acc.linearAcc;
}

void SeekSteering::DrawDebug() {
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);

	//LinearAcc
	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(static_cast<USVec2D>(GetCh()->GetLoc()),
		static_cast<USVec2D>(GetCh()->GetLoc()) + mLastLinearAcc);

	gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);

	MOAIDraw::DrawPoint(GetCh()->GetParams().target_position);
	MOAIDraw::DrawEllipseOutline(GetCh()->GetParams().target_position.mX,
		GetCh()->GetParams().target_position.mY, GetCh()->GetParams().arrive_radius,
		GetCh()->GetParams().arrive_radius, 64);
}