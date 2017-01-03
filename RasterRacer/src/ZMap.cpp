#include "ZMap.h"

ZMap::ZMap(int horizonHeight)
: mHorizonHeight(horizonHeight)
, mZ(new float[horizonHeight])
, mScale(new float[horizonHeight])
, mScaleCorrection((-1.0f / float((0.28f * float(mHorizonHeight)) - ((float)mHorizonHeight * mFarPlaneDistanceMultiplier))))
{

	for (int i = 0; i < horizonHeight; ++i)
	{
		mZ[i] = CalculateZ((float)i);
		mScale[i] = CalculateScale((float)i);
	}

}


ZMap::~ZMap()
{
	
}

float ZMap::CalculateZ(float zIndex) const
{
	return -1.0f / (float(zIndex) - ((float)mHorizonHeight * mFarPlaneDistanceMultiplier)) * mMaxZDistance;
}

float ZMap::CalculateScale(float zIndex) const
{
	return mScaleCorrection * (1.0f / (-1.0f / float(zIndex - ((float)mHorizonHeight * mFarPlaneDistanceMultiplier))));
}
