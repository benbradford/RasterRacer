#include "stdafx.h"
#include "RoadUpdater.h"
#include "RoadScanLine.h"
#include "RoadSegmentManager.h"
#include "RoadSegment.h"
#include "ZMap.h"
#include <assert.h>

RoadUpdater::RoadUpdater(const int horizonHeight, const int screenHeight, const int screenWidth, const ZMap& zMap)
: mHorizonHeight(horizonHeight)
, mScreenHeight(screenHeight)
, mScreenWidth(screenWidth)
, mScaleZero(zMap.GetScale(0))
, mZMap(zMap)
, mScanLines(new RoadScanLine[mScreenHeight])
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

	float screenY = float(mScreenHeight);
	float previousY = float(mScreenHeight + 1);
	float smallestScreenY = screenY + 1;

	float screenX = float(mScreenWidth) / 2.0f - playerX*mZMap.GetScale(1);
	const float perspectiveDX = (float(mScreenWidth / 2.0f) - screenX) / float(mHorizonHeight);

	auto segmentX = mXSegmentManager->GetFirst();
	auto segmentY = mYSegmentManager->GetFirst();
	float turnDelta = 0.0f;
	float dDY = 0;
	
	mNumScanLines = 0;
	for (int zIndex = 0; zIndex < mHorizonHeight && mNumScanLines < mScreenHeight; ++zIndex)
	{	
		
		if (screenY < smallestScreenY)
		{
			PopulateScanLines(zIndex, mNumScanLines, screenX, screenY, previousY);
		}
		else
		{
			
			mScanLines[mNumScanLines++].draw = false;
		}

		previousY = screenY;

		if (screenY < smallestScreenY)
			smallestScreenY = screenY;

		UpdateDeltas(zIndex, segmentX, segmentY, screenX, turnDelta, dDY, screenY, perspectiveDX);


	}

}

void RoadUpdater::UpdateDeltas(int zIndex, RoadSegmentIterator& segmentX, RoadSegmentIterator& segmentY, float& screenX, float& turnDelta, float& dDY, float& screenY, float perspectiveDX)
{
	const float z = mZMap.GetZ(zIndex);
	const float scale = mZMap.GetScale(zIndex);

	segmentX = mXSegmentManager->GetSegmentForZ(z, segmentX);
	segmentY = mYSegmentManager->GetSegmentForZ(z, segmentY);

	dDY += (*segmentY)->GetDelta() * z * (z / 5.0f) * (mScaleZero - scale);
	screenY += -1 + dDY;

	turnDelta += (*segmentX)->GetDelta() * z * (z / 10.0f) * ((mScaleZero - scale)*1.2f);
	screenX += turnDelta;
	screenX += perspectiveDX;
}

void RoadUpdater::PopulateScanLines(int zIndex, int &numScanLines, float screenX, float screenY, float previousY)
{
	const float z = mZMap.GetZ(zIndex);
	const float scale = mZMap.GetScale(zIndex);
	
	
	if (zIndex > 0 && screenY < int(previousY) - 1)
	{
		// :TODO: create some new scanLines
		const int offsetY = int(previousY) - int(screenY);

		const float lastScreenX = mScanLines[mNumScanLines - 1].screenX;
		const float moveDelta = 1.0f / (offsetY);
		float currentDelta = moveDelta;
		float adjustedZIndex = float(zIndex);// float(zIndex - 1) + moveDelta;
		for (int i = 0; i < offsetY && numScanLines < mScreenHeight; ++i)
		{
			RoadScanLine& road = mScanLines[numScanLines++];
			const float adjustedZ = mZMap.CalculateZ(adjustedZIndex);

			const float currentX = (lastScreenX + ((screenX - lastScreenX) * currentDelta));
			const float currentScale = mZMap.CalculateScale(adjustedZIndex);
			road.draw = true;
			road.screenYInt = int(screenY + i);
			road.screenX = currentX;
			road.scale = currentScale;
			road.roadFrame = 1;
			if ((int(adjustedZ + mRoadTextureOffset) % 4) >= 2)
				road.roadFrame = 0;
			
			currentDelta += moveDelta;
			adjustedZIndex -= moveDelta;
		}
	}		
	else
	{
		RoadScanLine& road = mScanLines[numScanLines++];

		road.draw = true;
		road.screenYInt = int(screenY);
		road.screenX = screenX;
		road.scale = scale;
		road.roadFrame = 1;
		if ((int(z + mRoadTextureOffset) % 4) >= 2)
			road.roadFrame = 0;

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
