#pragma once
#include <memory>
#include "RoadSegmentFwd.h"

class RoadSegmentManager;
struct RoadScanLine;
class ZMap;

class RoadUpdater
{
public:

	const int mHorizonHeight;
	const int mScreenHeight;
	const int mScreenWidth;
	const float mScaleZero;

	RoadUpdater(const int horizonHeight, const int screenHeight, const int screenWidth, const ZMap& zMap);
	~RoadUpdater();
	
	void Update(float movementSpeed, float playerX);

	RoadScanLine const*	GetScanLines(int& maxLines) const { maxLines = mNumScanLines; return mScanLines.get(); }
private:
	const ZMap&									mZMap;
	const std::unique_ptr<RoadScanLine[]>		mScanLines;
	const std::unique_ptr<RoadSegmentManager>	mXSegmentManager;
	const std::unique_ptr<RoadSegmentManager>	mYSegmentManager;
	float										mRoadTextureOffset;
	int											mNumScanLines;

	void PopulateScanLines(int zIndex, int &numScanLines, float screenX, float screenY, float previousY);
	void UpdateTextureOffset(float speed);
	void UpdateDeltas(int zIndex, RoadSegmentIterator& segmentX, RoadSegmentIterator& segmentY, float& screenX, float& turnDelta, float& dDY, float& screenY, float perspectiveDX);
};

