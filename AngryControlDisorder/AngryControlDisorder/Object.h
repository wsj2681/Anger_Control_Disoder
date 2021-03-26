#pragma once

class Mesh;
class Material;
class Texture;
class Shader;
class Camera;


class Object
{
public:

	Object();
	Object(int materials);
	Object(const Object&) = delete;
	Object& operator=(const Object&) = delete;
	virtual ~Object() = default;

private:

	int references = 0;

protected:

	char frameName[64];
	
	Mesh* mesh = nullptr;

	int materialCount = 0;
	Material** materials = nullptr;

	XMFLOAT4X4 toParent;
	XMFLOAT4X4 world;

	Object* parent = nullptr;
	Object* child = nullptr;
	Object* sibling = nullptr;

	bool isActive = true;

	XMFLOAT3 scale{ 1.f, 1.f, 1.f };

public:

	void AddRef();
	void Release();

	virtual void OnPrepareAnimate();
	virtual void Animate(float eTime);

	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* commandList, Camera* camera = nullptr);

	virtual void BuildMaterials(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	virtual void CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* commandList);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, XMFLOAT4X4* world);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, Material* material);
	virtual void ReleaseShadervariables();
	virtual void ReleaseUploadBuffers();

	void UpdateTransform(XMFLOAT4X4* parent = nullptr);

	void SetMesh(Mesh* mesh);
	void SetShader(Shader* shader);
	void SetShader(int materialCount, Shader* shader);
	void SetMaterial(int materialCount, Material* material);

	void SetChild(Object* child, bool referenceUpdate = false);

	void SetIsActive(bool active);
	const bool GetIsActive();

	const char* GetFrameName();

	const XMFLOAT3& GetPosition();
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& position);

	const XMFLOAT3& GetLook();
	void SetLook(float x, float y, float z);
	void SetLook(const XMFLOAT3& look);

	const XMFLOAT3& GetUp();
	void SetUp(float x, float y, float z);
	void SetUp(const XMFLOAT3& up);

	const XMFLOAT3& GetRight();
	void SetRight(float x, float y, float z);
	void SetRight(const XMFLOAT3& right);

	const XMFLOAT3& GetScale();
	void SetScale(float x, float y, float z);
	void SetScale(const XMFLOAT3& scale);

	void MoveForward(float distance = 1.f);
	void MoveStrafe(float distance = 1.f);
	void MoveUp(float distance = 1.f);

	void Rotate(float pitch = 10.f, float yaw = 10.f, float roll = 10.f);
	void Rotate(XMFLOAT3* axis, float angle);
	void Rotate(XMFLOAT4* quarernion);

	Object* GetParent();
	Object* FindFrame(char* frameName);

	Texture* FindReplicatedTexture(_TCHAR* textureName);
	UINT GetMeshType();
	/* Animation Function */
};

