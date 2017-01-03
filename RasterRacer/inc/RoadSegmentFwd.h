#pragma once
#include <list>

class RoadSegment;
class RoadSegmentManager;

typedef std::list<RoadSegment*> RoadSegmentQueue;
typedef RoadSegmentQueue::const_iterator RoadSegmentIterator;