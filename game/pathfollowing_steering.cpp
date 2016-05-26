#include <vector>

#include "stdafx.h"
#include "character.h"
#include "pathfollowing_steering.h"
#include "seek_steering.h"

PathFollowingSteering::PathFollowingSteering() {
	mSeekDelegate = new SeekSteering();
}

PathFollowingSteering::~PathFollowingSteering() {
}

void PathFollowingSteering::Init(Character * ch) {
	Steering::Init(ch);
	mSeekDelegate->Init(ch);
}

void PathFollowingSteering::Update(Accelerations & acc, USVec2D target) {
	std::vector<float> distances;
	USVec2D a, b, v, segment;
	float segmentSize, projection;
	//find nearest segment -> number
	Character * ch;
	ch = GetCh();
	for (uint16_t i = 0; i < GetCh()->GetParams().numSegments - 1; ++i) {
		a = USMetaVec2D<float>(GetCh()->GetParams().points[i].x, GetCh()->GetParams().points[i].y);
		b = USMetaVec2D<float>(GetCh()->GetParams().points[i+1].x, GetCh()->GetParams().points[i+1].y);
		v = GetCh()->GetLoc() - a;
		segment = b - a;
		segmentSize = segment.Length();
		projection = v.Dot(segment.NormVector()) + GetCh()->GetParams().look_ahead;
		float distance;
		if (projection > segmentSize) {
			distance = (GetCh()->GetLoc() - b).Length();
		} else if (projection < 0) {
			distance = (GetCh()->GetLoc() - a).Length();
		} else {
			distance = (GetCh()->GetLoc() - (a + (segment.NormVector() * projection))).Length();
		}
		if (distance <= 0.001) {
			distance = 0.f;
		}
		distances.push_back(distance);
	}
	uint16_t nearestPos = 0;
	float nearestValue = *distances.begin();
	for (std::vector<float>::iterator itr = distances.begin(); itr != distances.end(); ++itr) {
		if ((*itr) <= nearestValue) {
			nearestValue = *itr;
			nearestPos = itr - distances.begin();
			mCurrentSegment = nearestPos;
		}
	}

	//calc projection in this segment
	a = USMetaVec2D<float>(GetCh()->GetParams().points[nearestPos].x,
		GetCh()->GetParams().points[nearestPos].y);
	b = USMetaVec2D<float>(GetCh()->GetParams().points[nearestPos + 1].x,
		GetCh()->GetParams().points[nearestPos + 1].y);
	v = GetCh()->GetLoc() - a;
	segment = b - a;
	segmentSize = segment.Length();
	projection = v.Dot(segment.NormVector()) + GetCh()->GetParams().look_ahead;

	if (mCurrentSegment == GetCh()->GetParams().points.size() - 2) {
		a = USMetaVec2D<float>(GetCh()->GetParams().points[GetCh()->GetParams().points.size() - 2].x,
			GetCh()->GetParams().points[GetCh()->GetParams().points.size() - 2].y);
		b = USMetaVec2D<float>(GetCh()->GetParams().points[GetCh()->GetParams().points.size() - 1].x,
			GetCh()->GetParams().points[GetCh()->GetParams().points.size() - 1].y);
		v = GetCh()->GetLoc() - a;
		segment = b - a;
		segmentSize = segment.Length();
		projection = v.Dot(segment.NormVector()) + GetCh()->GetParams().look_ahead;
	} else if (projection > segmentSize) {
		//float offset = projection - segmentSize;
		a = USMetaVec2D<float>(GetCh()->GetParams().points[nearestPos + 1].x,
			GetCh()->GetParams().points[nearestPos + 1].y);
		b = USMetaVec2D<float>(GetCh()->GetParams().points[nearestPos + 2].x,
			GetCh()->GetParams().points[nearestPos + 2].y);
		v = GetCh()->GetLoc() - a;
		segment = b - a;
		segmentSize = segment.Length();
		projection = v.Dot(segment.NormVector()) + GetCh()->GetParams().look_ahead;
	}

	USVec2D newTarget;

	if (projection > segmentSize) {
		newTarget = b;
	} else if (projection < 0) {
		newTarget = a;
	} else {
		newTarget = a + (segment.NormVector() * projection);
	}

	mTarget = newTarget;
	GetCh()->GetParams().target_position = newTarget;
	mSeekDelegate->Update(acc, newTarget);
}

void PathFollowingSteering::DrawDebug() {
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);

	//path
	/*for (uint8_t i = 0; i < GetCh()->GetParams().numSegments; i++) {
		MOAIDraw::DrawLine(
			USVec2D(GetCh()->GetParams().points[i].x, GetCh()->GetParams().points[i].y),
			USVec2D(GetCh()->GetParams().points[i+1].x, GetCh()->GetParams().points[i+1].y));
	}*/

	gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);
	MOAIDraw::DrawLine(static_cast<USVec2D>(GetCh()->GetLoc()), mTarget);
}