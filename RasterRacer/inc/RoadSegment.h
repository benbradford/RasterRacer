#pragma once
class RoadSegment
{
public:

	RoadSegment(float delta, float length);
	~RoadSegment();

	void Update(float distanceTravelledThrough);
	void Appear(float startZ);
	float GetDelta() const;		// amount of delta to apply
	bool HasExpired() const { return mLengthRemaining <= 0.0f; }
	float GetCurrentZ() const { return mCurrentZ; }
	
private:
	const float mDelta;
	const float mLength;
	
	float mZApproach;
	float mCurrentZ;
	float mLengthRemaining;

};

