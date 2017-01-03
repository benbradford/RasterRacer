#pragma once

#include <vector>
#include <memory>
#include <list>
#include "RoadSegmentFwd.h"

class RoadSegmentManager
{
public:

	const float mMaxZ;
	RoadSegmentManager(float maxZ);
	~RoadSegmentManager();

	void AddSegment(float delta, float length);
	void UpdateSegmentQueue();
	void Update(float distanceMoved);
	RoadSegmentIterator GetSegmentForZ(float zPosition, RoadSegmentIterator last) const;
	RoadSegmentIterator GetFirst() const { return mSegmentQueue.begin(); }
	
private:
	std::vector<std::unique_ptr<RoadSegment>>	mSegments;
	RoadSegmentQueue	mSegmentQueue;
	unsigned int mIndexOfNextSegment;
};

