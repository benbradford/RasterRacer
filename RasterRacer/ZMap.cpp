#include "stdafx.h"
#include "ZMap.h"


std::unique_ptr<ZMap> ZMap::sInstance(nullptr);
const ZMap& ZMap::GetInstance()
{
	return *sInstance;
}

void ZMap::Initialise(int screenHeight)
{
	sInstance.reset(new ZMap(screenHeight));
}

void ZMap::Terminate()
{
	sInstance.reset(nullptr);
}


ZMap::ZMap(int horizonHeight)
: mHorizonHeight(horizonHeight)
, mZ(new float[horizonHeight])
, mScale(new float[horizonHeight])
{
	const float farPlaneDistanceMultiplier = 1.04f;
	const float maxZDistance = 1200.0f;
	for (int i = 0; i < horizonHeight; ++i)
	{
		mZ[i] = -1.0f / float(float(i) - ((float)mHorizonHeight * farPlaneDistanceMultiplier)) * maxZDistance;
		mScale[i] = 1.0f / (-1.0f / float(float(i) - ((float)mHorizonHeight * farPlaneDistanceMultiplier)));
	}
	const int normalisedScalePercent = int(0.28f * float(mHorizonHeight));
	const float correct = 1.0f / mScale[normalisedScalePercent];
	for (int i = 0; i < horizonHeight; ++i)
	{
		mScale[i] *= correct;
		
	}


}


ZMap::~ZMap()
{
	
}
