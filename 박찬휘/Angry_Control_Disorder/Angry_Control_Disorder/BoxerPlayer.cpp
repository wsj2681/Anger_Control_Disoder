#include "framework.h"
#include "BoxerPlayer.h"
#include "ModelInfo.h"
#include "AnimationController.h"
#include "CallBack.h"

BoxerPlayer::BoxerPlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext)
{

	ModelInfo* BoxerModel = Object::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/ThaiBoxerS.bin", nullptr);
	SetChild(BoxerModel->m_pModelRootObject, true);

	this->head = FindFrame("Bip01_Head");
	this->rHand = FindFrame("Bip01_R_Hand");
	this->lHand = FindFrame("Bip01_L_Hand");
	this->rFoot = FindFrame("Bip01_L_Foot");
	this->lFoot = FindFrame("Bip01_R_Foot");
	this->spine = FindFrame("Bip01_Spine1");

	m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);

	m_pSkinnedAnimationController = new AnimationController(pd3dDevice, pd3dCommandList, 1, BoxerModel);

	m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);

	AnimationCallbackHandler* pAnimationCallbackHandler = new SoundCallbackHandler();
	m_pSkinnedAnimationController->SetAnimationCallbackHandler(1, pAnimationCallbackHandler);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	SetPlayerUpdatedContext(pContext);
	SetCameraUpdatedContext(pContext);

	SetPosition(XMFLOAT3(0.f, 10.f, -769.689f));


	if (BoxerModel) delete BoxerModel;
}

BoxerPlayer::~BoxerPlayer()
{

}

Camera* BoxerPlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return(m_pCamera);
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		SetFriction(250.0f);
		SetGravity(XMFLOAT3(0.0f, -400.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(head->GetPosition());
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 90.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case SPACESHIP_CAMERA:
		SetFriction(125.0f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(head->GetPosition());
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case THIRD_PERSON_CAMERA:
		SetFriction(250.0f);
		SetGravity(XMFLOAT3(0.0f, -250.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.25f);

		m_pCamera->SetOffset(XMFLOAT3(0.0f, 5.0f, -20.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	default:
		break;
	}
	m_pCamera->SetPosition(Vector3::Add(head->GetPosition(), m_pCamera->GetOffset()));
	Update(fTimeElapsed);

	return(m_pCamera);
}

void BoxerPlayer::Update(float fTimeElapsed)
{
	Player::Update(fTimeElapsed);
	// TODO : 애니메이션 셋 할때 이쪽으로 와서 한다. 
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	//SetTrackAnimationSet(0, ::IsZero(fLength) ? ANIMATION_IDLE : ANIMATION_ATTACK_LOOP);
}
