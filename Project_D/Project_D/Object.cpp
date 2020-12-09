#include "framework.h"
#include "Mesh.h"
#include "MapShader.h"
#include "PCamera.h"
#include "Object.h"

//Object::Object(int nMeshes)
//{
//	this->nMeshes = nMeshes;
//	if (nMeshes > 0)
//	{
//		meshes = new Mesh*[nMeshes];
//		for (int i = 0; i < nMeshes; ++i)
//			meshes[i] = nullptr;
//	}
//}
//
//Object::~Object()
//{
//	if (meshes)
//	{
//		for (int i = 0; i < nMeshes; ++i)
//		{
//			SAFE_RELEASE(meshes[i]);
//			meshes[i] = nullptr;
//		}
//		delete[] meshes;
//	}
//	SAFE_RELEASE(material);
//}
//
//void Object::SetMesh(const int& index, Mesh* mesh)
//{
//	
//	if (meshes)
//	{
//		SAFE_RELEASE(meshes[index]);
//		meshes[index] = mesh;
//	}
//}

//void Object::SetShader(Shader* shader)
//{
//	if (!material)
//	{
//		Material* material = new Material();
//		
//	}
//	if (material) material->SetShader(shader);
//}

Object::~Object()
{
	SAFE_RELEASE(mesh);
}

void Object::SetMesh(Mesh* mesh)
{
	SAFE_RELEASE(mesh);
	this->mesh = mesh;
	if (this->mesh) this->mesh->AddRefference();
}

void Object::SetShader(MapShader* shader)
{
	this->shader = shader;
}

void Object::Render(ID3D12GraphicsCommandList* commandList, PCamera* camera)
{
	if (shader)
	{
		shader->UpdateShaderVariable(commandList, &world);
		shader->Render(commandList);
	}

	if (mesh) mesh->Render(commandList);
}

void Object::ReleaseUploadBuffers()
{
	if (mesh) mesh->ReleaseUploadBuffers();
}

//void Object::SetMaterial(Material* material)
//{
//	SAFE_RELEASE(this->material);
//	this->material = material;
//	if (this->material) this->material->AddRef();
//}

//void Object::CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
//{
//	
//}
