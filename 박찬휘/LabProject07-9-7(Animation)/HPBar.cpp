#include "stdafx.h"
#include "HPBar.h"

UIObject::UIObject(const XMFLOAT2& position, const float& width, const float& height)
{
	this->SetPosition(position);
	this->width = width;
	this->height = height;
}

XMFLOAT2& UIObject::GetPosition()
{
	return position;
}

void UIObject::SetPosition(const XMFLOAT2& position)
{
	this->position = position;
}

void UIObject::Update()
{
	// Update
}

void UIObject::Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap, ID3D12PipelineState* uiPipelineState)
{
	// Render
	commandList->ClearDepthStencilView(descriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	commandList->SetPipelineState(uiPipelineState);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	commandList->IASetVertexBuffers(0, 1, &uiVertexBufferView[frameIndex]);
}

HPBar::HPBar()
{
	UIObject::UIObject();
}

HPBar::HPBar(const XMFLOAT2& position, const float& width, const float& height)
{
	UIObject::UIObject(position, width, height);
}

HPBar::~HPBar()
{
	UIObject::~UIObject();
}

unsigned int& HPBar::GetMaxHP()
{
	return maxHp;
}

unsigned int& HPBar::GetCurHP()
{
	return curHp;
}

void HPBar::SetMaxHP(const unsigned int& maxHp)
{
	this->maxHp = maxHp;
}

void HPBar::SetCurHP(const unsigned int& curHp)
{
	this->curHp = curHp;
}

void HPBar::Update()
{
	UIObject::Update();
}

void HPBar::Render()
{
	UIObject::Render();
}
