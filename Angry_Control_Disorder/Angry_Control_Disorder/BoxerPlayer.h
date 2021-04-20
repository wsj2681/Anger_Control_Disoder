#pragma once
#include "Player.h"
class BoxerPlayer : public Player
{
public:

	BoxerPlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext = nullptr);
	virtual ~BoxerPlayer();

public:
	virtual Camera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);
	virtual void Update(float fTimeElapsed);
};

