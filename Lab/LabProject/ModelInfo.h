#pragma once

class Object;
class SkinnedMesh;
class AnimationSets;

class ModelInfo
{
public:
	ModelInfo() { }
	~ModelInfo();

	Object* m_pModelRootObject = nullptr;

	int 							m_nSkinnedMeshes = 0;
	SkinnedMesh** m_ppSkinnedMeshes = nullptr; //[SkinnedMeshes], Skinned Mesh Cache

	AnimationSets* m_pAnimationSets = nullptr;

public:
	void PrepareSkinning();
};