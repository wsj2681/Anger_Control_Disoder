#pragma once
class ResourceHelper final
{
public:

	ResourceHelper() = default;
	ResourceHelper(const ResourceHelper&) = delete;
	ResourceHelper& operator=(const ResourceHelper&) = delete;
	~ResourceHelper() = default;

private:

	UINT gnCbvSrvDescriptorIncrementSize = 0;
	UINT gnRtvDescriptorIncrementSize = 0;
	UINT gnDsvDescriptorIncrementSize = 0;

public:

	void SetCbvSrvDescriptorIncrementSize(UINT size);
	UINT GetCbvSrvDescriptorIncrementSize() const;

	void SetRtvDescriptorIncrementSize(UINT size);
	UINT GetRtvDescriptorIncrementSize() const;

	void SetDsvDescriptorIncrementSize(UINT size);
	UINT SetDsvDescriptorIncrementSize() const;

	void SynchronizeResourceTransition(ID3D12GraphicsCommandList* pd3dCommandList, ID3D12Resource* pd3dResource, D3D12_RESOURCE_STATES d3dStateBefore, D3D12_RESOURCE_STATES d3dStateAfter);
	ID3D12Resource* CreateBufferResource(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource** ppd3dUploadBuffer = nullptr);
	ID3D12Resource* CreateTextureResourceFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	ID3D12Resource* CreateTextureResourceFromWICFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	ID3D12Resource* CreateTexture2DResource(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue);

};

