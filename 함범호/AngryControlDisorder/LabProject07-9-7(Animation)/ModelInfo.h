#pragma once

class Object;
class SkinnedMesh;
class AnimationSets;

class ModelInfo
{
public:
	ModelInfo() { }
	~ModelInfo();

	Object* m_pModelRootObject = NULL;

	int 							m_nSkinnedMeshes = 0;
	SkinnedMesh** m_ppSkinnedMeshes = NULL; //[SkinnedMeshes], Skinned Mesh Cache

	AnimationSets* m_pAnimationSets = NULL;

public:
	void PrepareSkinning();
};