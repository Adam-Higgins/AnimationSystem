#ifndef _H_SAMPLE_
#define _H_SAMPLE_

#include "Application.h"
#include <vector>
#include "Pose.h"
#include "Clip.h"
#include "Skeleton.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include <vector>
#include "CrossFadeController.h"
#include "CrossFadeTarget.h"

struct AnimationInstance {
	Pose mAnimatedPose;
	std::vector <mat4> mPosePalette;
	unsigned int mClip;
	float mPlayback;
	Transform mModel;

	Pose mAddPose;
	Pose mAdditiveBase;
	float mAdditiveTime;
	float mAdditiveDirection;
	unsigned int mAdditiveIndex;

	inline AnimationInstance() : mClip(0), mPlayback(0.0f), mAdditiveTime(0.0f), mAdditiveDirection(1.0f), mAdditiveIndex(0) { }
};

class Sample : public Application {
protected:
	Texture* mDiffuseTexture;
	Shader* mStaticShader;
	Shader* mSkinnedShader;
	std::vector<Mesh> mCPUMeshes;
	std::vector<Mesh> mGPUMeshes;
	Skeleton mSkeleton;
	std::vector<Clip> mClips;
	float temp = -10.0f;

	AnimationInstance mGPUAnimInfo;
	AnimationInstance mCPUAnimInfo;

	CrossFadeController mFadeController;
	float mFadeTimer;

public:
	void Initialize();
	void Update(float deltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
};

#endif