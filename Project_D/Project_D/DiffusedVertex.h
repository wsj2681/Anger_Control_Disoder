#pragma once

class DiffusedVertex : public Vertex
{
public:

    DiffusedVertex() = default;
    DiffusedVertex(const XMFLOAT3& position, const XMFLOAT4& diffuse);
    DiffusedVertex(float x, float y, float z, const XMFLOAT4& diffuse);
    DiffusedVertex(float x, float y, float z, float r, float g, float b, float a);

private:

    XMFLOAT4 diffuseColor{ 0.f, 0.f, 0.f, 0.f };

};

