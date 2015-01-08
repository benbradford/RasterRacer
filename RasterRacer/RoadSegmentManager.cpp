#include "stdafx.h"
#include "RoadSegmentManager.h"
#include "RoadSegment.h"
#include <assert.h>

RoadSegmentManager::RoadSegmentManager(float maxZ)
: mMaxZ(maxZ)
, mIndexOfNextSegment(1)
{
	std::unique_ptr<RoadSegment> segment(new RoadSegment(0.0f, 0.1f));
	mSegments.push_back(std::move(segment)); 
	mSegmentQueue.push_back(mSegments[0].get());	
}


RoadSegmentManager::~RoadSegmentManager()
{
}

void RoadSegmentManager::AddSegment(float delta, float length)
{
	std::unique_ptr<RoadSegment> segment (new RoadSegment(delta, length));
	mSegments.push_back(std::move(segment));
}

void RoadSegmentManager::Update(float distanceMoved)
{
	for (auto segmentIterator = mSegmentQueue.begin(); segmentIterator != mSegmentQueue.end(); ++segmentIterator)
	{
		(*segmentIterator)->Update(distanceMoved);
	}

	// ensure enough segments are in view
	UpdateSegmentQueue();
}

RoadSegmentIterator RoadSegmentManager::GetSegmentForZ(float z, RoadSegmentIterator last) const
{
	RoadSegmentIterator next = last;
	++next;
	if (next != mSegmentQueue.end() && z > (*next)->GetCurrentZ())
	{
		return GetSegmentForZ(z, next);
	}
	return last;
}

void RoadSegmentManager::UpdateSegmentQueue()
{
	auto currentSegment = mSegmentQueue.begin();
	auto nextSegment = ++mSegmentQueue.begin();
	auto lastSegment = mSegmentQueue.back();
	
	if (lastSegment->HasExpired())
	{
		RoadSegment* newSegment = &*mSegments[mIndexOfNextSegment];
		mSegmentQueue.push_back(newSegment);
		newSegment->Appear(mMaxZ);
		++mIndexOfNextSegment;
		if (mIndexOfNextSegment >= mSegments.size())
			mIndexOfNextSegment = 0;
	}

	if (nextSegment != mSegmentQueue.end() && (*nextSegment)->GetCurrentZ() <= 0.001f)
	{
		mSegmentQueue.pop_front();
	}

}