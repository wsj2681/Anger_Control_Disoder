//#include "framework.h"
//#include "Texture.h"
//#include "Shader.h"
//#include "Material.h"
//
//Material::~Material()
//{
//	SAFE_RELEASE(texture);
//	SAFE_RELEASE(shader);
//}
//
//void Material::SetReflection(const unsigned int nReflection)
//{
//	this->nReflection = nReflection;
//}
//
//void Material::setReflection(XMFLOAT4X4* reflection)
//{
//	this->reflection = reflection;
//}
//
//void Material::SetTexture(Texture* texture)
//{
//	SAFE_RELEASE(this->texture);
//	this->texture = texture;
//	if (this->texture) this->texture->AddRef();
//}
//
//void Material::SetShader(Shader* shader)
//{
//	SAFE_RELEASE(this->shader);
//	this->shader = shader;
//	if (this->shader) this->shader->AddRef();
//}
//
//void Material::AddRef()
//{
//	++nRefers;
//}
//
//void Material::Release()
//{
//	if (--nRefers <= 0) delete this;
//}
