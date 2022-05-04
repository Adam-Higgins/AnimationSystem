#pragma once
#include <vector>
#include "Transform.h"

class FABRIKSolver
{
protected:
	std::vector<Transform> mIKChain;
	unsigned int mNumSteps;
	float mThreshold;
	std::vector<vec3> mWorldChain;
	std::vector<float> mLengths;

	void IKChainToWorld();
	void IterateForward(const vec3& goal);
	void IterateBackward(const vec3& base);
	void WorldToIKChain();

public:
	FABRIKSolver();

	unsigned int Size();
	void Resize(unsigned int newSize);
	
	unsigned int GetNumSteps();
	void SetNumSteps(unsigned int numSteps);

	float GetThreshold();
	void SetThreshold(float value);

	Transform GetLocalTransform(unsigned int index);
	void SetLocalTransform(unsigned int index, const Transform& t);
	Transform GetGlobalTransform(unsigned int index);

	bool Solve(const Transform& target);

	void ApplyBallSocketConstraint(int i, float limit);
	void ApplyHingeSocketConstraint(int i, vec3 axis);
};






