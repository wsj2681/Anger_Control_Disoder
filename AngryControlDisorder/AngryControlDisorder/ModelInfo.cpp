#include "framework.h"
#include "ModelInfo.h"
#include "Object.h"
#include "AnimationSets.h"

ModelInfo::ModelInfo()
{
}

ModelInfo::~ModelInfo()
{
	DELETE_ARRAY(skinnedMeshs);
}

void ModelInfo::PrepareSkinning()
{
	int skinnedMesh = 0;
	this->skinnedMeshs = new SkinnedMesh * [this->skinnedMeshCount];
	modelRootObject->FindAndSetSkinnedMesh(this->skinnedMeshs, &skinnedMesh);

	for (int i = 0; i < skinnedMeshCount; ++i)
	{
		skinnedMeshs[i]->PrepareSkinning(modelRootObject);
	}
}
