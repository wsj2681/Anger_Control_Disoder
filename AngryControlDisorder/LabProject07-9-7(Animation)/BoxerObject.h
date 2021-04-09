#pragma once
#include "Object.h"
class BoxerObject : public Object
{
private:
	XMFLOAT3 wayPoints[10];

public:
	BoxerObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, int nAnimationTracks);
	virtual ~BoxerObject();

	UINT nowState = STATE_IDLE;

	UINT GetNowState() { return nowState; }
	void SetNowState(UINT state) { nowState = state; };
	bool IsMove() { return nowState == STATE_MOVE; }
	void Update(float fTimeElapsed);
	void SetTarget(XMFLOAT3 target);
};