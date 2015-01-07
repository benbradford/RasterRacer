#pragma once

#include <vector>
#include <memory>
#include <list>
class RoadSegment;

class RoadSegmentManager
{
public:
	typedef std::list<RoadSegment*> SegmentQueue;
	typedef SegmentQueue::const_iterator RoadSegmentIterator;
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
	SegmentQueue	mSegmentQueue;
	unsigned int mIndexOfNextSegment;
};

