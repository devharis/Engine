#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include "D3D.h"
#include "Camera.h"
#include "Model.h"
#include "LightShader.h"
#include "Light.h"
#include "Bitmap.h"
#include "TextureShader.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = .1f;

class Graphic{
public:
	Graphic();
	~Graphic();
	Graphic(const Graphic&);

	bool Init(int, int, HWND);
	void ShutDown();
	bool Frame();

private:
	bool Render(float rotation);
	D3D* m_D3D;

	Camera* m_Camera;
	Model* m_Model;
	LightShader* m_LightShader;
	Light* m_Light;
	TextureShader* m_TextureShader;

	Bitmap* m_Bitmap;
};