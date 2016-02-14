#include "Bitmap.h"



Bitmap::Bitmap()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_Texture = nullptr;
}

Bitmap::Bitmap(const Bitmap &)
{
}

Bitmap::~Bitmap()
{
}

bool Bitmap::Init(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR * textureFilename, int bitmapWidth, int bitmapHeight)
{
	bool result;

	//Screen size
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Size of bitmap in pixels
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	m_previousPosX = -1;
	m_previousPosY = -1;

	// Init the vertex & index buffers
	result = InitBuffers(device);
	if (!result)
		return false;

	// Load the texture model
	result = LoadTexture(device, textureFilename);
	if (!result)
		return false;

	return true;
}

void Bitmap::Shutdown()
{
	// Release the model texture
	ReleaseTexture();

	// Shutdown the vertex & index buffers
	ShutdownBuffers();

	return;
}

bool Bitmap::Render(ID3D11DeviceContext* deviceContext, int positonX, int positionY)
{
	bool result;

	// Rebuild the dynamic vertex buffer for rendering to a possibly diff location
	result = UpdateBuffers(deviceContext, positonX, positionY);
	if (!result)
		return false;

	// Put the vertex & index buffers on the graphics pipeline to prep for drawing
	RenderBuffers(deviceContext);

	return true;
}

int Bitmap::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView * Bitmap::GetTexture()
{
	return m_Texture->GetTexture();
}

bool Bitmap::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Set the number of vertices in vertex array
	m_vertexCount = 6;
	
	// Set the number of indices in index array
	m_indexCount = m_vertexCount;

	// Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	// Create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	// Init vertex array to zero at first
	memset(vertices, 0, (sizeof(VertexType)* m_vertexCount));

	// Load the index array with data
	for (i = 0; i < m_indexCount; i++)
		indices[i] = i;

	// Set up the desc for static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give subresource structure pointer to vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// Set the desc of static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give subresource structure pointer to index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (!result)
		return false;

	// Release arrays after vertex and index buffers have been created
	delete []vertices;
	vertices = nullptr;

	delete []indices;
	indices = nullptr;

	return true;
}

void Bitmap::ShutdownBuffers()
{
	// Release the index buffer
	if (m_indexBuffer){
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	// Release the vertex buffer
	if(m_vertexBuffer){
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	return;
}

bool Bitmap::UpdateBuffers(ID3D11DeviceContext *deviceContext, int positionX, int positionY)
{
	float left, right, top, bottom;
	VertexType* verticesPtr;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;

	// If the position hasn't changed, don't update
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
		return true;

	// If changed, then update the new position
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	// Calc the screen coord of the left side of the bitmap
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;
	// Calc the screen coord of the right side of the bitmap
	right = left + (float)m_bitmapWidth;
	// Calc the screen coord of the top of the bitmap
	top = (float)(m_screenHeight / 2) - (float)positionY;
	// Calc the screen coord of the bottom of the bitmap
	bottom = top - (float)m_bitmapHeight;

	// Create vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	// Load the vertex array with data
	// First triangle
	vertices[0].position = XMFLOAT3(left, top, 0.0f); // Top left
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f); // Bottom right
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f); // Top left
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// Second triangle
	vertices[3].position = XMFLOAT3(left, top, 0.0f); // Top left
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f); // Top right
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f); // Bottom right
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	// Lock the vertex buffer to enable writing
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	// Get a pointer to the data in the vertex buffer
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) *m_vertexCount));

	// Unlock the vertex buffer
	deviceContext->Unmap(m_vertexBuffer, 0);

	// Release the vertex array as it's not needed anymore
	delete []vertices;
	vertices = nullptr;

	return true;
}

void Bitmap::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer (in this case triangles)
	deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	return;
}

bool Bitmap::LoadTexture(ID3D11Device *device, WCHAR *filename)
{
	bool result;

	// Create the texture object
	m_Texture = new Texture();
	if (!m_Texture)
		return false;
	
	// Init the texture object
	result = m_Texture->Init(device, filename);
	if (!result)
		return false;

	return true;
}

void Bitmap::ReleaseTexture()
{
	// Release the texture object
	if(m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}

	return;
}
