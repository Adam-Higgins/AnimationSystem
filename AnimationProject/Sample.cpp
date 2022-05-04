#pragma warning(disable : 26451)
#define _CRT_SECURE_NO_WARNINGS
#include "Sample.h"
#include "GLTFLoader.h"
#include "Uniform.h"
#include "Blending.h"
#include "glad.h"

void Sample::Initialize() {
	cgltf_data* gltf = LoadGLTFFile("Woman.gltf");
	mCPUMeshes = LoadMeshes(gltf);
	mSkeleton = LoadSkeleton(gltf);
	mClips = LoadAnimationClips(gltf);
	FreeGLTFFile(gltf);

	mGPUMeshes = mCPUMeshes;
	for (unsigned int i = 0, size = (unsigned int)mGPUMeshes.size(); i < size; ++i) {
		mGPUMeshes[i].UpdateOpenGLBuffers();
	}

	//Load Textures and Shaders
	mStaticShader = new Shader("static.vert", "lit.frag");
	mSkinnedShader = new Shader("skinned.vert", "lit.frag");
	mDiffuseTexture = new Texture("Woman.png");

	mGPUAnimInfo.mAnimatedPose = mSkeleton.GetRestPose();
	mGPUAnimInfo.mPosePalette.resize(mSkeleton.GetRestPose().Size());
	mCPUAnimInfo.mAnimatedPose = mSkeleton.GetRestPose();
	mCPUAnimInfo.mPosePalette.resize(mSkeleton.GetRestPose().Size());

	//Positioning models in world position
	mGPUAnimInfo.mModel.position = vec3(-2, 0, 0);
	mCPUAnimInfo.mModel.position = vec3(2, 0, 0);

	unsigned int numUIClips = (unsigned int)mClips.size();
	for (unsigned int i = 0; i < numUIClips; ++i) {
		if (mClips[i].GetName() == "Walking") {
			mCPUAnimInfo.mClip = i;
		}
		else if (mClips[i].GetName() == "Running") {
			mGPUAnimInfo.mClip = i;
		}
		else if (mClips[i].GetName() == "Lean_Left")
		{
			mCPUAnimInfo.mAdditiveIndex = i;
		}
	}

	//Create and set poses needed for additive blending
	mCPUAnimInfo.mAdditiveBase = MakeAdditivePose(mSkeleton, mClips[mCPUAnimInfo.mAdditiveIndex]);
	mClips[mCPUAnimInfo.mAdditiveIndex].SetLooping(false);
	mCPUAnimInfo.mAddPose = mSkeleton.GetRestPose();

	//initialising Fade controller needed for smooth animation blending
	mFadeController.SetSkeleton(mSkeleton);
	mFadeController.Play(&mClips[0]);
	mFadeController.Update(0.0f);
	mFadeController.GetCurrentPose().GetMatrixPalette(mGPUAnimInfo.mPosePalette);
	mFadeTimer = 3.0f;
}

void Sample::Update(float deltaTime) {

	mCPUAnimInfo.mAdditiveTime += deltaTime * mCPUAnimInfo.mAdditiveDirection;

	if (mCPUAnimInfo.mAdditiveTime < 0.0f) {
		mCPUAnimInfo.mAdditiveTime = 0.0f;
		mCPUAnimInfo.mAdditiveDirection *= -1.0f;
	}

	if (mCPUAnimInfo.mAdditiveTime > 1.0f) {
		mCPUAnimInfo.mAdditiveTime = 1.0f;
		mCPUAnimInfo.mAdditiveDirection *= -1.0f;
	}

	// Sample the animation playing for the walking character
	mCPUAnimInfo.mPlayback = mClips[mCPUAnimInfo.mClip].Sample(mCPUAnimInfo.mAnimatedPose, mCPUAnimInfo.mPlayback + deltaTime);

	//Applies additive blending so that the lean animation is applied to the walking animation so they are played at the same time
	float time = mClips[mCPUAnimInfo.mAdditiveIndex].GetStartTime() + (mClips[mCPUAnimInfo.mAdditiveIndex].GetDuration() * mCPUAnimInfo.mAdditiveTime);
	mClips[mCPUAnimInfo.mAdditiveIndex].Sample(mCPUAnimInfo.mAddPose, time);
	Add(mCPUAnimInfo.mAnimatedPose, mCPUAnimInfo.mAnimatedPose, mCPUAnimInfo.mAddPose, mCPUAnimInfo.mAdditiveBase, -1);

	//Skin the walking model using CPU Skinning
	for (unsigned int i = 0, size = (unsigned int)mCPUMeshes.size(); i < size; ++i) {
		mCPUMeshes[i].CPUSkin(mSkeleton, mCPUAnimInfo.mAnimatedPose);
	}

	//update fade controller for current anim sampling
	mFadeController.Update(deltaTime);

	//change animation to a new random animation every 3 seconds
	mFadeTimer -= deltaTime;
	if (mFadeTimer < 0.0f) {
		mFadeTimer = 3.0f;

		unsigned int clip = mGPUAnimInfo.mClip;
		while (clip == mGPUAnimInfo.mClip) {
			clip = rand() % mClips.size();
		}
		mGPUAnimInfo.mClip = clip;

		mFadeController.FadeTo(&mClips[clip], 0.5f);
	}

	mFadeController.GetCurrentPose().GetMatrixPalette(mGPUAnimInfo.mPosePalette);
}

void Sample::Render(float inAspectRatio) {
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(vec3(0, 5, 7), vec3(0, 3, 0), vec3(0, 1, 0));
	mat4 model;

	// CPU Skinned Mesh
	model = transformToMat4(mCPUAnimInfo.mModel);
	mStaticShader->Bind();
	Uniform<mat4>::Set(mStaticShader->GetUniform("model"), model);
	Uniform<mat4>::Set(mStaticShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mStaticShader->GetUniform("projection"), projection);
	Uniform<vec3>::Set(mStaticShader->GetUniform("light"), vec3(1, 1, 1));

	mDiffuseTexture->Set(mStaticShader->GetUniform("tex0"), 0);
	for (unsigned int i = 0, size = (unsigned int)mCPUMeshes.size(); i < size; ++i) {
		mCPUMeshes[i].Bind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
		mCPUMeshes[i].Draw();
		mCPUMeshes[i].UnBind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
	}
	mDiffuseTexture->UnSet(0);
	mStaticShader->UnBind();

	// GPU Skinned Mesh
	model = transformToMat4(mGPUAnimInfo.mModel);
	mSkinnedShader->Bind();
	Uniform<mat4>::Set(mSkinnedShader->GetUniform("model"), model);
	Uniform<mat4>::Set(mSkinnedShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mSkinnedShader->GetUniform("projection"), projection);
	Uniform<vec3>::Set(mSkinnedShader->GetUniform("light"), vec3(1, 1, 1));

	Uniform<mat4>::Set(mSkinnedShader->GetUniform("pose"), mGPUAnimInfo.mPosePalette);
	Uniform<mat4>::Set(mSkinnedShader->GetUniform("invBindPose"), mSkeleton.GetInvBindPose());

	mDiffuseTexture->Set(mSkinnedShader->GetUniform("tex0"), 0);
	for (unsigned int i = 0, size = (unsigned int)mGPUMeshes.size(); i < size; ++i) {
		mGPUMeshes[i].Bind(mSkinnedShader->GetAttribute("position"), mSkinnedShader->GetAttribute("normal"), mSkinnedShader->GetAttribute("texCoord"), mSkinnedShader->GetAttribute("weights"), mSkinnedShader->GetAttribute("joints"));
		mGPUMeshes[i].Draw();
		mGPUMeshes[i].UnBind(mSkinnedShader->GetAttribute("position"), mSkinnedShader->GetAttribute("normal"), mSkinnedShader->GetAttribute("texCoord"), mSkinnedShader->GetAttribute("weights"), mSkinnedShader->GetAttribute("joints"));
	}
	mDiffuseTexture->UnSet(0);
	mSkinnedShader->UnBind();
}

void Sample::Shutdown() {
	delete mStaticShader;
	delete mDiffuseTexture;
	delete mSkinnedShader;
	mClips.clear();
	mCPUMeshes.clear();
	mGPUMeshes.clear();
}