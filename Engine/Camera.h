#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>

class Camera{
public:
	Camera();
	Camera(const Camera&);
	~Camera();
	
	void SetPos(float, float, float);
	void SetRot(float, float, float);
	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetRot();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

private:
	float m_posX, m_posY, m_posZ;
	float m_rotX, m_rotY, m_rotZ;
	DirectX::XMMATRIX m_viewMatrix;
};

#endif