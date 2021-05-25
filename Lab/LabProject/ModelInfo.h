#pragma once

class Object;
class SkinnedMesh;
class AnimationSets;

class ModelInfo final
{
public:
	ModelInfo() = default;
	ModelInfo(const ModelInfo&) = delete;
	ModelInfo& operator=(const ModelInfo&) = delete;
	ModelInfo(ModelInfo&&) = delete;
	ModelInfo& operator=(ModelInfo&&) = delete;
	~ModelInfo();
public:
	void PrepareSkinning();
private:
	Object* m_pModelRootObject = nullptr;
	int m_nSkinnedMeshes = 0;
	SkinnedMesh** m_ppSkinnedMeshes = nullptr; //[SkinnedMeshes], Skinned Mesh Cache
	AnimationSets* m_pAnimationSets = nullptr;
};