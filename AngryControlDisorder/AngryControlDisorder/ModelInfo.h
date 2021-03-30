#pragma once

class Object;
class SkinnedMesh;
class AnimationSets;

class ModelInfo final
{
public:

	ModelInfo();
	~ModelInfo();

public:

	Object* modelRootObject = nullptr;
	int skinnedMeshCount = 0;
	SkinnedMesh** skinnedMeshs = nullptr;

	AnimationSets* animationSets = nullptr;

public:

	void PrepareSkinning();

};