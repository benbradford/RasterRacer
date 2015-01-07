#pragma once

#include <memory>


class ZMap
{
public:
	~ZMap();

	const int mHorizonHeight;

	static const ZMap&	GetInstance();
	static void			Initialise(int screenHeight);
	static void			Terminate();

	float GetZ(int screenY) const { return mZ[screenY]; }
	float GetScale(int screenY) const { return mScale[screenY]; }
	float GetMaxZ() const { return mZ[mHorizonHeight - 1]; }
private:
	ZMap(int horizonHeight);
	
	const std::unique_ptr<float[]> mZ;
	const std::unique_ptr<float[]> mScale;

	static std::unique_ptr<ZMap> sInstance;
};

