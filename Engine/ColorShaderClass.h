#pragma once
#pragma comment(lib, "D3DCompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include <D3Dcompiler.h>
#include <fstream>

using namespace std;

class ColorShaderClass
{
private:
	struct MatrixBufferType
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, const DirectX::XMFLOAT4X4 &worldMatrix, const DirectX::XMFLOAT4X4 &viewMatrix, const DirectX::XMFLOAT4X4 &projectionMatrix);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, const DirectX::XMFLOAT4X4 &worldMatrix, const DirectX::XMFLOAT4X4 &viewMatrix, const DirectX::XMFLOAT4X4 &projectionMatrix);
	void RenderShader(ID3D11DeviceContext*, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};
