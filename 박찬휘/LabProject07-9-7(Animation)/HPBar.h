#pragma once
//class CGameObject
//{
//public:
//	void SetShader(CShader* pShader);
//
//	virtual void OnPrepareAnimate() { }
//	virtual void Animate(float fTimeElapsed);
//
//	virtual void OnPrepareRender();
//	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
//
//	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
//	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
//	virtual void ReleaseShaderVariables();
//
//	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
//	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, CMaterial* pMaterial);
//
//	virtual void ReleaseUploadBuffers();
//
//	XMFLOAT3 GetPosition();
//	XMFLOAT3 GetLook();
//	XMFLOAT3 GetUp();
//	XMFLOAT3 GetRight();
//
//	void SetPosition(float x, float y, float z);
//	void SetPosition(XMFLOAT3 xmf3Position);
//	void SetScale(float x, float y, float z);
//
//	void MoveStrafe(float fDistance = 1.0f);
//	void MoveUp(float fDistance = 1.0f);
//
//	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
//	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);
//	void Rotate(XMFLOAT4* pxmf4Quaternion);
//
//	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent = NULL);
//};

class UIObject
{
private:
	int	m_nReferences{ 0 };
	char							m_pstrFrameName[64];

	XMFLOAT4X4						m_xmf4x4ToParent;
	XMFLOAT4X4						m_xmf4x4World;
public:
	/*void AddRef();
	void Release();*/
public:
	UIObject() = default;
	UIObject(const XMFLOAT2& position, const float& width = 0.f, const float& height = 0.f);
	~UIObject() = default;
private:
	XMFLOAT2 position{ XMFLOAT2(0.f, 0.f) };
public:
	float width{ 0.0f };
	float height{ 0.0f };
	ID3D12Resource* uiVertexBuffer{};
	D3D12_VERTEX_BUFFER_VIEW uiVertexBufferView[3];
	
public:
	XMFLOAT2& GetPosition();
	void SetPosition(const XMFLOAT2& position);

	virtual void Update();
	virtual void Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap, ID3D12PipelineState* uiPipelineState, D3D12_GPU_DESCRIPTOR_HANDLE srvHandle);
	//void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);


};

class HPBar : public UIObject
{
public:
	HPBar();
	HPBar(const XMFLOAT2& position, const float& width = 0.f, const float& height = 0.f);
	~HPBar();
private:
	unsigned int maxHp{ 100 };
	unsigned int curHp{ 100 };
public:
	unsigned int& GetMaxHP();
	unsigned int& GetCurHP();
	void SetMaxHP(const unsigned int& maxHp);
	void SetCurHP(const unsigned int& curHp);

	virtual void Update();
	virtual void Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap, ID3D12PipelineState* uiPipelineState, D3D12_GPU_DESCRIPTOR_HANDLE srvHandle);
};