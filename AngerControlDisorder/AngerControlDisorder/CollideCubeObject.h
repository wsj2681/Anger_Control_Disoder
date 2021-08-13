#pragma once
#include "Object.h"


class CollideCubeMesh;
class DiffusedShader;

class CollideCubeObject : public Object
{
public:
    CollideCubeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
    virtual ~CollideCubeObject() = default;
};

