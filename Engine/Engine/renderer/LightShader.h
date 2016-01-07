#pragma once

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>

using namespace std;
using namespace DirectX;

class LightShader{
private:
	struct MatrixBufferType{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projMatrix;
	};
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};
	struct LightBufferType{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};

public:
	LightShader();
	~LightShader();
	LightShader(const LightShader&);

	bool Init(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, 
		XMMATRIX& worldMatrix,
		XMMATRIX& viewMatrix,
		XMMATRIX& projMatrix,
		ID3D11ShaderResourceView* texture,
		XMFLOAT3 lightDirection,
		XMFLOAT4 ambientColor,
		XMFLOAT4 diffuseColor,
		XMFLOAT3 cameraPosition,
		XMFLOAT4 specularColor,
		float specularPower);

private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, 
		XMMATRIX& worldMatrix,
		XMMATRIX& viewMatrix,
		XMMATRIX& projMatrix,
		ID3D11ShaderResourceView*,
		XMFLOAT3 lightDirection,
		XMFLOAT4 ambientColor,
		XMFLOAT4 diffuseColor,
		XMFLOAT3 cameraPosition,
		XMFLOAT4 specularColor,
		float specularPower
		);
	void RenderShader(ID3D11DeviceContext*, int);

	void ProcessShaderError(LPCWSTR, HWND, ID3D10Blob*, WCHAR*);
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	ID3D11SamplerState* m_sampleState;

	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;
};