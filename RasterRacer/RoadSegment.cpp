#include "stdafx.h"
#include "RoadSegment.h"
#include <algorithm>


RoadSegment::RoadSegment(float delta, float length)
: mDelta(delta)
, mLength(length)
, mCurrentZ(0)
, mLengthRemaining(0.0f)
{
}


RoadSegment::~RoadSegment()
{
}

void RoadSegment::Appear(float startZ)
{
	mLengthRemaining = mLength;
	mCurrentZ = startZ;
	mZApproach = startZ;
}

void RoadSegment::Update(float distanceTravelledThrough)
{
	mCurrentZ = std::max(mCurrentZ - distanceTravelledThrough, 0.0f);
	mLengthRemaining -= distanceTravelledThrough;
}

float RoadSegment::GetDelta() const
{
	//const float appearDelta = 1.0f - (mCurrentZ / mZApproach);
	//const float delta = ((2.0f * (mDelta * appearDelta)) + (lastDelta * 0.5f)) * 0.5f;
	return mDelta;
}


