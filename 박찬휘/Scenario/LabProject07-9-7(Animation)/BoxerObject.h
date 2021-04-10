#pragma once
#include "Object.h"
#include "WayPoint.h"
class BoxerObject : public Object
{
private:
	
public:
	BoxerObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, int nAnimationTracks);
	virtual ~BoxerObject();

	virtual void UpdateWayPoints();
};