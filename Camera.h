#pragma once

#include "Transform.h"
#include <DirectXMath.h>

class Camera
{
public:
	Camera(float x, float y, float z, float aspectRatio, float fieldOfView, float nearPlane, float farPlane);
	~Camera();

	// Update methods
	void Update(float dt);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	// getter for the transform
	Transform* GetTransform();
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

private:
	// Camera matrices
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Overall transform info
	Transform transform;

	float aspectRatio;
	float fieldOfView;
	float nearPlane;
	float farPlane;

};

