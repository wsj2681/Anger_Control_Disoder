#pragma once

#include "SoundManager.h"
#include "Camera.h"
#include "Shader.h"

class Scene
{
public:
	Scene() = default;
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	~Scene() = default;

private:
	Camera* testCamera = nullptr;
	Shader* shader = nullptr;

public:
	SoundManager* soundmanager = nullptr;

	void BulidObjects(ID3D12Device* Device, ID3D12GraphicsCommandList* CommandList);
	void ReleaseObjects();

	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* Device);
	ID3D12RootSignature* GetGraphicsRootSignature();
	void SetGraphicRootSignature(ID3D12GraphicsCommandList* CommandList);

	void AnimateObjects(float fDeltaTime);

	void PreRender(ID3D12GraphicsCommandList* CommandList, Camera* camera);
	void Render(ID3D12GraphicsCommandList* CommandList);

	void ReleaseUploadBuffers();

	//void CreateShaderVariables(ID3D12Device* Device, ID3D12GraphicsCommandList* CommandList);
	//void UpdateShaderVariables(ID3D12GraphicsCommandList* CommandList);
	//void ReleaseShaderVariables();

	void OnProcessingMouseMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProecssingWindowMessage(HWND hWnd, UINT MessageID, WPARAM wParam, LPARAM lParam);

protected:
	ID3D12RootSignature* GraphicRootSignature = nullptr;
};