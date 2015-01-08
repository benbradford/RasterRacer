#pragma once

#include <memory>


class ZMap
{
public:
	ZMap(int horizonHeight);
	~ZMap();

	
	float GetZ(int screenY) const { return mZ[screenY]; }
	float GetScale(int screenY) const { return mScale[screenY]; }
	float GetMaxZ() const { return mZ[mHorizonHeight - 1]; }
	float CalculateZ(float zIndex) const;
	float CalculateScale(float zIndex) const;
private:
	const int mHorizonHeight;
	const float mFarPlaneDistanceMultiplier = 1.04f;
	const float mMaxZDistance = 1200.0f;
	const float mScaleCorrection;

	
	const std::unique_ptr<float[]> mZ;
	const std::unique_ptr<float[]> mScale;

};

