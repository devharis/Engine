#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>

using namespace std;
using namespace DirectX;

class ColorShader
{
private:
	struct MatrixBufferType
	{
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 projectionMatrix;
	};

public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix);
	void RenderShader(ID3D11DeviceContext*, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};