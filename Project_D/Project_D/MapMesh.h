#pragma once

class DiffusedVertex;

class MapMesh : public Mesh
{
public:
	MapMesh() = default;
	MapMesh(const MapMesh&) = delete;
	MapMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, void* meshData);
	MapMesh& operator=(const MapMesh&) = delete;
	virtual ~MapMesh() = default;

private:
	
};