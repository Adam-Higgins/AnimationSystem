#pragma once

#ifndef _H_GLTFLOADER_
#define _H_GLTFLOADER_

#include "cgltf.h"
#include "Pose.h"
#include "Clip.h"
#include "Skeleton.h"
#include "Mesh.h"

cgltf_data* LoadGLTFFile(const char* path);
Pose LoadRestPose(cgltf_data* data);
Pose LoadBindPose(cgltf_data* data);
Skeleton LoadSkeleton(cgltf_data* data);
std::vector<Mesh> LoadMeshes(cgltf_data* data);
std::vector<std::string> LoadJointNames(cgltf_data* data);
std::vector<Clip> LoadAnimationClips(cgltf_data* data);
void FreeGLTFFile(cgltf_data* handle);

#endif