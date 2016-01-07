#include "Model.h"

using namespace DirectX;
Model::Model() :m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_model(nullptr) {
}

Model::Model(const Model& other){
}

Model::~Model(){
}

bool Model::Init(ID3D11Device* device, char* modelFilename, WCHAR* textureFileName){
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);

	if (!result)
	{
		return false;
	}

	result = InitBuffers(device);

	if (!result){
		OutputDebugString(L"Failed to init vertex and index buffer. \r\n");
		return false;
	}

	result = LoadTexture(device, textureFileName);

	if (!result){
		OutputDebugString(L"Failed to load texture from file. \r\n");

		return false;
	}

	return true;
}

void Model::Shutdown(){
	ReleaseTexture();
	ShutdownBuffers();
	// Release the model data.
	ReleaseModel();
}

void Model::Render(ID3D11DeviceContext* deviceContext){
	RenderBuffers(deviceContext);
	return;
}

int Model::GetIndexCount(){
	return m_indexCount;
}

ID3D11ShaderResourceView* Model::GetTexture(){
	return m_Texture->GetTexture();
}

bool Model::InitBuffers(ID3D11Device* device){
	VertexType* vertices;
	unsigned long* indices;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	HRESULT result;

	int i;

	//m_vertexCount = 3;
	//m_indexCount = 3;
	vertices = new VertexType[m_vertexCount];

	if (!vertices){
		OutputDebugString(L"failed to create vertex array");
		return false;
	}

	indices = new unsigned long[m_indexCount];

	if (!indices){
		OutputDebugString(L"failed to create indices\r\n");

		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	if (FAILED(result)){
		OutputDebugString(L"Failed to create vertex buffer.\r\n ");
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result)){
		OutputDebugString(L"Failed to create index buffer.\r\n");
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

bool Model::LoadTexture(ID3D11Device* device, WCHAR* filename){
	bool result;
	m_Texture = new Texture;

	if (!m_Texture){
		OutputDebugString(L"Failed to create texture object \r\n");
		return false;
	}
	result = m_Texture->Init(device, filename);

	if (!result){
		OutputDebugString(L"Failed to initialize texture object\r\n");
		return false;
	}
	return true;
}

void Model::ReleaseTexture(){
	if (m_Texture){
		m_Texture->Shutdown();
		m_Texture = nullptr;
	}
	return;
}

bool Model::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

//The ReleaseModel function handles deleting the model data array.
void Model::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

void Model::ShutdownBuffers(){
	if (m_indexBuffer){
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}
	if (m_vertexBuffer){
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
	return;
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext){
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;


}