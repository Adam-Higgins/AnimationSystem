#include "Skeleton.h"

//Calculates the inverse bind pose using the current global transform
void Skeleton::UpdateInverseBindPose()
{
	unsigned int size = mBindPose.Size();
	mInvBindPose.resize(size);

	for (unsigned int i = 0; i < size; ++i)
	{
		Transform world = mBindPose.GetGlobalTransform(i);
		mInvBindPose[i] = inverse(transformToMat4(world));
	}
}

Skeleton::Skeleton()
{
}

Skeleton::Skeleton(const Pose & rest, const Pose & bind, const std::vector<std::string>& names)
{
	Set(rest, bind, names);
}

//fills the skeleton object with data for the rest pose, bind pose and names of each joints
void Skeleton::Set(const Pose & rest, const Pose & bind, const std::vector<std::string>& names)
{
	mRestPose = rest;
	mBindPose = bind;
	mJointNames = names;
	UpdateInverseBindPose();
}

// GETTERS AND SETTERS


Pose & Skeleton::GetBindPose()
{
	return mBindPose;
}

Pose & Skeleton::GetRestPose()
{
	return mRestPose;
}

std::vector<mat4>& Skeleton::GetInvBindPose()
{
	return mInvBindPose;
}

std::vector<std::string>& Skeleton::GetJointNames()
{
	return mJointNames;
}

std::string & Skeleton::GetJointName(unsigned int index)
{
	return mJointNames[index];
}
