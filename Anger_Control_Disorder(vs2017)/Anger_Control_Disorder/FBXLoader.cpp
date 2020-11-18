#include "FBXLoader.h"

void FBXLoader::Init(const char* fileName)
{

	Manager = FbxManager::Create();

	IOsettings = FbxIOSettings::Create(Manager, IOSROOT);
	Manager->SetIOSettings(IOsettings);

	FbxImporter* Importer = FbxImporter::Create(Manager, "");
	if (Importer->Initialize(fileName, -1, Manager->GetIOSettings()))
	{
		cout << "Importer : Initialize OK" << endl;
	}

	Scene = FbxScene::Create(Manager, "");
	if (Importer->Import(Scene))
	{
		cout << "Importer : Scene Import OK" << endl;
	}

	Importer->Destroy();

	FbxAxisSystem sceneAxisSystem = Scene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem::MayaZUp.ConvertScene(Scene);

	FbxGeometryConverter geometryConverter(Manager);
	geometryConverter.Triangulate(Scene, true);

	FbxNode* pFbxRootNode = Scene->GetRootNode();
	LoadNode(pFbxRootNode);

	cout << "OK" << endl;
}

void FBXLoader::Destroy()
{

	this->Scene->Destroy();
	this->IOsettings->Destroy();
	this->Manager->Destroy();
}

bool FBXLoader::LoadTexturefromFBX(const char* fileName)
{
	return false;
}

bool FBXLoader::LoadMaterialfromFBX(FbxGeometry* geometry)
{
	unsigned int MaterialsCount = this->Scene->GetMaterialCount();

	for (unsigned int i = 0; i < MaterialsCount; ++i)
	{

		FbxSurfaceMaterial* material = this->Scene->GetMaterial(i);
		FbxProperty prop;
		if (material->GetClassId.Is(FbxSurfacePhong::ClassId))
		{
			FbxSurfacePhong* phong = dynamic_cast<FbxSurfacePhong*>(material);

		}
	}
	return false;
}

void FBXLoader::LoadAnimationfromFBX(const char* fileName)
{
}

bool FBXLoader::LoadControlPoints(FbxMesh* mesh)
{
	unsigned int count = mesh->GetControlPointsCount();

	CtrlPoints.reserve(count);

	for (unsigned int i = 0; i < count; ++i)
	{
		XMFLOAT3 position;

		position.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
		position.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
		position.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);

		this->CtrlPoints.push_back(position);
	}

	return true;
}

void FBXLoader::LoadNode(FbxNode* node)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute)
	{
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* mesh = node->GetMesh();
			FbxGeometry* geometry = node->GetGeometry();

			LoadMaterialfromFBX(geometry);
			LoadControlPoints(mesh);

			unsigned int triCount = mesh->GetPolygonCount();
			unsigned int vertexCount = 0;

			for (unsigned int i = 0; i < triCount; ++i)
			{
				for (unsigned int j = 0; j < 3; ++j)
				{
					int controlPointIndex = mesh->GetPolygonVertex(i, j);

					TextureVertex vertex;
					vertex.Position = CtrlPoints[controlPointIndex];
					vertex.Normal = ReadNormal(mesh, controlPointIndex, vertexCount);
					vertex.Binormal = ReadBinormal(mesh, controlPointIndex, vertexCount);
					vertex.Tangent = ReadTangent(mesh, controlPointIndex, vertexCount);
					vertex.UV = ReadUV(mesh, mesh->GetTextureUVIndex(i, j));

					InsertVertex(vertex);

					vertexCount++;
				}
			}
		}

	}
	const unsigned int childCount = node->GetChildCount();

	for (unsigned int i = 0; i < childCount; ++i)
	{
		LoadNode(node->GetChild(i));
	}
}

XMFLOAT3 FBXLoader::ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh == nullptr)
	{
		cout << "mesh is nullptr" << endl;
		return XMFLOAT3();
	}

	if (mesh->GetElementNormalCount() < 1)
	{
		cout << "Normals is empty" << endl;
		return XMFLOAT3();
	}

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);

	XMFLOAT3 result{ 0.f, 0.f, 0.f };

	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:

		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
				break;
			}
		case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
				break;
			}
		default:
			cout << "Read Normal : controlPointIndex = " << controlPointIndex << ", vertexCounter = " << vertexCounter << endl;
			break;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:

		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
				break;
			}
		case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
				break;
			}
		default:
			cout << "Read Normal : controlPointIndex = " << controlPointIndex << ", vertexCounter = " << vertexCounter << endl;
			break;
		}
		break;

	default:
		cout << "vertexNormal->GetMappingMode() is Default" << endl;
		break;
	}

	return result;
}

XMFLOAT3 FBXLoader::ReadBinormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh == nullptr)
	{
		cout << "mesh is nullptr" << endl;
		return XMFLOAT3();
	}

	if (mesh->GetElementBinormalCount() < 1)
	{
		//cout << "Binormals is empty" << endl;
		return XMFLOAT3();
	}

	const FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);

	XMFLOAT3 result{ 0.f, 0.f, 0.f };

	switch (vertexBinormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:

		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
				result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
				result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
				break;
			}
		case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexBinormal->GetIndexArray().GetAt(controlPointIndex);
				result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
				break;
			}
		default:
			cout << "Read Binormal : controlPointIndex = " << controlPointIndex << ", vertexCounter = " << vertexCounter << endl;
			break;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:

		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
				result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
				result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
				break;
			}
		case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexBinormal->GetIndexArray().GetAt(vertexCounter);
				result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
				break;
			}
		default:
			cout << "Read Binormal : controlPointIndex = " << controlPointIndex << ", vertexCounter = " << vertexCounter << endl;
			break;
		}
		break;

	default:
		cout << "vertexBinormal->GetMappingMode() is Default" << endl;
		break;
	}

	return result;
}

XMFLOAT3 FBXLoader::ReadTangent(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh == nullptr)
	{
		cout << "mesh is nullptr" << endl;
		return XMFLOAT3();
	}

	if (mesh->GetElementTangentCount() < 1)
	{
		//cout << "Tangents is empty" << endl;
		return XMFLOAT3();
	}

	const FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);

	XMFLOAT3 result{ 0.f, 0.f, 0.f };

	switch (vertexTangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:

		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[1]);
				result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[2]);
				break;
			}
		case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexTangent->GetIndexArray().GetAt(controlPointIndex);
				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
				break;
			}
		default:
			cout << "Read Tangent : controlPointIndex = " << controlPointIndex << ", vertexCounter = " << vertexCounter << endl;
			break;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:

		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[1]);
				result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[2]);
				break;
			}
		case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexTangent->GetIndexArray().GetAt(vertexCounter);
				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
				break;
			}
		default:
			cout << "Read Tangent : controlPointIndex = " << controlPointIndex << ", vertexCounter = " << vertexCounter << endl;
			break;
		}
		break;

	default:
		cout << "vertexTangent->GetMappingMode() is Default" << endl;
		break;
	}

	return result;
}

XMFLOAT2 FBXLoader::ReadUV(const FbxMesh* mesh, int TextureUVIndex)
{
	if (mesh == nullptr)
	{
		cout << "mesh is nullptr" << endl;
		return XMFLOAT2();
	}

	if (mesh->GetElementUVCount() < 1)
	{
		cout << "UV is empty" << endl;
		return XMFLOAT2();
	}

	const FbxGeometryElementUV* vertexTangent = mesh->GetElementUV(0);

	XMFLOAT2 result{ 0.f, 0.f };

	switch (vertexTangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:

		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(TextureUVIndex).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(TextureUVIndex).mData[1]);
				break;
			}
		case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexTangent->GetIndexArray().GetAt(TextureUVIndex);
				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
				break;
			}
		default:
			cout << "UV Tangent : TexureUVIndex = " << TextureUVIndex << endl;
			break;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:

		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(TextureUVIndex).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(TextureUVIndex).mData[1]);
				break;
			}
		case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexTangent->GetIndexArray().GetAt(TextureUVIndex);
				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
				break;
			}
		default:
			cout << "UV Tangent : TexureUVIndex = " << TextureUVIndex << endl;
			break;
		}
		break;

	default:
		cout << "vertexUV->GetMappingMode() is Default" << endl;
		break;
	}

	return result;
}

void FBXLoader::InsertVertex(TextureVertex& vertex)
{
	size_t index = TexVertex.size();

	auto lookup = indexMapping.find(index);

	if (lookup != indexMapping.end())
	{
		indices.push_back(lookup->first);
	}
	else
	{
		indexMapping[index] = vertex;
		indices.push_back(index);
		TexVertex.push_back(vertex);
	}
}
