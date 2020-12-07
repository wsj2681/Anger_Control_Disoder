#pragma once
#include "framework.h"

ID3D12Resource* CreateBufferResource(
	ID3D12Device* pd3dDevice,
	ID3D12GraphicsCommandList* pd3dCommandList,
	void* pData,
	UINT nBytes,
	D3D12_HEAP_TYPE d3dHeapType,
	D3D12_RESOURCE_STATES d3dResourceStates,
	ID3D12Resource** ppd3dUploadBuffer);