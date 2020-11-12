#include "framework.h"
#include "Material.h"

void Material::AddReference()
{
}

void Material::Release()
{
}

void Material::SetShader(Shader * shader)
{
	this->shader = shader;
}

void Material::MaterialType(UINT Type)
{
	this->Type |= Type;
}

const UINT & Material::GetMaterialType() const
{
	return this->Type;
}

void Material::SetTexture(Texture* texture)
{
	this->texture = texture;
}

void Material::UpdateShaderVariables(ID3D12GraphicsCommandList * CommandList)
{
	CommandList->SetGraphicsRoot32BitConstants(1, 4, &this->phong.AmbientColor, 16);
	CommandList->SetGraphicsRoot32BitConstants(1, 4, &this->phong.AlbedoColor, 20);
	CommandList->SetGraphicsRoot32BitConstants(1, 4, &this->phong.SpecularColor, 24);
	CommandList->SetGraphicsRoot32BitConstants(1, 4, &this->phong.EmissiveColor, 28);

	CommandList->SetGraphicsRoot32BitConstants(1, 1, &this->Type, 32);

	if (this->texture) this->texture->UpdateShaderVariables(CommandList);
}

void Material::ReleaseShaderVariables()
{
	if (this->shader)
		this->shader->ReleaseShaderVariables();
	if (this->texture)
		this->texture->ReleaseShaderVariables();
}

void Material::ReleaseUploadBUffers()
{

}
