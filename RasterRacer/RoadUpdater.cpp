#include "stdafx.h"
#include "RoadUpdater.h"
#include "RoadScanLine.h"
#include "RoadSegmentManager.h"
#include "RoadSegment.h"
#include "ZMap.h"

RoadUpdater::RoadUpdater(const int horizonHeight, const int screenHeight, const int screenWidth, const ZMap& zMap)
: mHorizonHeight(horizonHeight)
, mScreenHeight(screenHeight)
, mScreenWidth(screenWidth)
, mZMap(zMap)
, mScanLines(new RoadScanLine[screenHeight])
, mXSegmentManager(new RoadSegmentManager(zMap.GetMaxZ()))
, mYSegmentManager(new RoadSegmentManager(zMap.GetMaxZ()))
, mRoadTextureOffset(0.0f)
, mNumScanLines(0)
{

	mXSegmentManager->AddSegment(0.0f, 60.0f);
	mXSegmentManager->AddSegment(0.002f, 60.0f);
	mXSegmentManager->AddSegment(0.0f, 10.0f);
	mXSegmentManager->AddSegment(-0.001f, 20.0f);
	mXSegmentManager->AddSegment(0.001f, 50.0f);
	mXSegmentManager->AddSegment(0.0f, 120.0f);
	mXSegmentManager->AddSegment(0.001f, 80.0f);
	mXSegmentManager->AddSegment(0.0008f, 20.0f);
	mXSegmentManager->AddSegment(0.001f, 80.0f);
	mXSegmentManager->AddSegment(0.0f, 200.0f);
	mXSegmentManager->AddSegment(-0.0022f, 30.0f);
	mXSegmentManager->AddSegment(0.0f, 20.0f);
	mXSegmentManager->AddSegment(-0.0015f, 120.0f);
	mXSegmentManager->AddSegment(0.003f, 20.0f);
	mXSegmentManager->AddSegment(-0.003f, 20.0f);

	mXSegmentManager->UpdateSegmentQueue();

	mYSegmentManager->AddSegment(0.0f, 60.0f);
	mYSegmentManager->AddSegment(0.0005f, 20.0f);
	mYSegmentManager->AddSegment(-0.0005f, 90.0f);
	mYSegmentManager->AddSegment(0.0f, 300.0f);
	mYSegmentManager->AddSegment(0.0002f, 100.0f);
	mYSegmentManager->AddSegment(-0.0005f, 300.0f);
	mYSegmentManager->AddSegment(0.0003f, 10.0f);
	mYSegmentManager->AddSegment(0.0f, 300.0f);

	mYSegmentManager->UpdateSegmentQueue();
}

RoadUpdater::~RoadUpdater()
{

}

void RoadUpdater::Update(float movementSpeed, float playerX)
{
	UpdateTextureOffset(movementSpeed);
	mXSegmentManager->Update(movementSpeed);
	mYSegmentManager->Update(movementSpeed);

	//' initial vertical position of drawing of road
	float screenY = float(mScreenHeight);
	float previousY = float(mScreenHeight + 1);
	float smallestScreenY = screenY + 1;

	float screenX = float(mScreenWidth) / 2.0f - playerX*mZMap.GetScale(1);
	float perspectiveDX = (float(mScreenWidth / 2.0f) - screenX) / float(mHorizonHeight);

	auto segmentX = mXSegmentManager->GetFirst();
	auto segmentY = mYSegmentManager->GetFirst();
	float turnDelta = 0.0f;
	float dDY = 0;
	const float scaleZero = mZMap.GetScale(0);
	mNumScanLines = 0;
	for (int i = 0; i < mHorizonHeight; ++i, ++mNumScanLines)
	{
		
		RoadScanLine& road = mScanLines[mNumScanLines];

		const float z = ZMap::GetInstance().GetZ(i);
		const float scale = ZMap::GetInstance().GetScale(i);
		
		segmentX = mXSegmentManager->GetSegmentForZ(z, segmentX);
		segmentY = mYSegmentManager->GetSegmentForZ(z, segmentY);

		if (screenY < smallestScreenY)
		{
			road.draw = true;
			road.screenYInt = int(screenY);
			road.screenX = screenX;
			road.scale = mZMap.GetScale(i);
			road.offsetY = 1;
			if (screenY < int(previousY) - 1)
				road.offsetY = int(previousY) - int(screenY);
			road.roadFrame = 1;
			if ((int(z + mRoadTextureOffset) % 4) >= 2)
				road.roadFrame = 0;
		}
		else
		{
			road.draw = false;
		}

		previousY = screenY;

		if (screenY < smallestScreenY)
			smallestScreenY = screenY;

		dDY += (*segmentY)->GetDelta() * z * (z / 5.0f) * (scaleZero - scale);
		screenY += -1 + dDY;

		turnDelta += (*segmentX)->GetDelta() * z * (z / 10.0f) * ((scaleZero - scale)*1.2f);
		screenX += turnDelta;
		screenX += perspectiveDX;

	}

}

void RoadUpdater::UpdateTextureOffset(float speed)
{
	mRoadTextureOffset += speed;
	if (mRoadTextureOffset > 4.0f)
	{
		mRoadTextureOffset -= 4.0f;
	}
}
