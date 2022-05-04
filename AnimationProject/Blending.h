#pragma once
#include "Pose.h"
#include "Skeleton.h"
#include "Clip.h"

bool IsInHierarchy(Pose& pose, unsigned int root, unsigned int search);
void Blend(Pose& output, Pose& a, Pose& b, float t, int root);

Pose MakeAdditivePose(Skeleton& skeleton, Clip& clip);
void Add(Pose& output, Pose& inPose, Pose& addPose, Pose& additiveBasePose, int blendroot);