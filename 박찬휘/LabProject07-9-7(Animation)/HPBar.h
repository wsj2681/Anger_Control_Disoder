#pragma once
class UIObject
{
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
public:
	XMFLOAT2& GetPosition();
	void SetPosition(const XMFLOAT2& position);

	virtual void Update();
	virtual void Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap, ID3D12PipelineState* uiPipelineState);
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
	virtual void Render(ID3D12GraphicsCommandList* commandList);
};