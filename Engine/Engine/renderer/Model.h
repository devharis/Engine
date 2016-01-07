#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

using namespace std;

#include "Texture.h"

class Model{
private:
	struct VertexType{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Model();
	~Model();
	Model(const Model&);
	
	bool Init(ID3D11Device*, char*, WCHAR* textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;

	ModelType* m_model;
	
};