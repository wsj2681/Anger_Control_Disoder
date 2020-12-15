#include "GameObject.h"
#include "Shader.h"

CGameObject::CGameObject(){
	//XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
}

CGameObject::~CGameObject(){
	if (m_pMesh)
		m_pMesh->Release();
}

void CGameObject::SetShader(CShader* pShader){
	m_pShader = pShader;
}

void CGameObject::SetMesh(CMesh* pMesh){
	
	if (m_pMesh)
		m_pMesh->Release();

	m_pMesh = pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

void CGameObject::ReleaseUploadBuffers(){
	//정점 버퍼를 위한 업로드 버퍼를 소멸시킨다.
	if (m_pMesh)
		m_pMesh->ReleaseUploadBuffers();
}

void CGameObject::Rotate(XMFLOAT3* pxmf3Axis, float fAngle){
	ASSERT(pxmf3Axis != nullptr, "The pxmf3Axis must not be null");

	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Animate(float fTimeElapsed)
{
}

void CGameObject::OnPrepareRender()
{
}

void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera){
	OnPrepareRender();
	if (m_pShader){
		//게임 객체의 월드 변환 행렬을 셰이더의 상수 버퍼로 전달(복사)한다.
		m_pShader->UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
		m_pShader->Render(pd3dCommandList, pCamera);
	}
	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList);
}

CRotatingObject::CRotatingObject() {

}
CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Animate(float fTimeElapsed){
	CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fTimeElapsed);
}