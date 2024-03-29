#pragma once
#include <vector>
#include "Transform.h"

class CCDSolver
{
protected:
	std::vector<Transform> mIKChain;
	unsigned int mNumSteps;
	float mThreshold;
public:
	CCDSolver();
	unsigned int Size();
	void Resize(unsigned int newSize);

	Transform& operator[](unsigned int index);
	Transform GetGlobalTransform(unsigned int index);
	unsigned int GetNumSteps();
	void SetNumSteps(unsigned int numSteps);

	float GetThreshold();
	void SetThreshold(float value);

	bool Solve(const Transform& target);
};