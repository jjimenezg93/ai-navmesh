#include "character.h"
#include "stdafx.h"
#include "steering.h"

Steering::Steering() {
}

Steering::~Steering() {
}

void Steering::Init(Character * ch) {
	mCh = ch;
}

void Steering::Update(Accelerations &acc, USVec2D target) {
}