#include "Transform.h"

using namespace DirectX;

Transform::Transform()
{
	// Set up our initial transformation values
	SetPosition(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScale(1, 1, 1);

	// Create our initial matrix
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());

	// matrix has yet to change
	matrixDirty = false;
}

void Transform::MoveAbsolute(float x, float y, float z)
{
	/* One way to do this
	position.x += x;
	position.y += y;
	position.z += z;
	*/

	// using math library instead
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR offset = XMVectorSet(x, y, z, 0);
	XMStoreFloat3(&position, pos + offset);

	// matrix has changed
	matrixDirty = true;
}

void Transform::Rotate(float p, float y, float r)
{
	XMVECTOR pyr = XMLoadFloat3(&pitchYawRoll);
	XMVECTOR rot = XMVectorSet(p, y, r, 0);
	XMStoreFloat3(&pitchYawRoll, pyr + rot);

	matrixDirty = true;
}

void Transform::Scale(float x, float y, float z)
{
	XMVECTOR scl = XMLoadFloat3(&scale);
	XMVECTOR resize = XMVectorSet(x, y, z, 0);
	XMStoreFloat3(&scale, XMVectorMultiply(scl, resize));

	matrixDirty = true;
}

void Transform::SetPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
	matrixDirty = true;
}

void Transform::SetRotation(float p, float y, float r)
{
	pitchYawRoll = XMFLOAT3(p, y, r);
	matrixDirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);
	matrixDirty = true;
}

DirectX::XMFLOAT3 Transform::GetPosition(){ return position; }
DirectX::XMFLOAT3 Transform::GetRotation(){ return pitchYawRoll; }
DirectX::XMFLOAT3 Transform::GetScale(){ return scale; }

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	// only update if there has been a change 
	if (matrixDirty)
	{
		// create the individual transfrom matrices 
		// for each type of transfrom
		XMMATRIX transMat = XMMatrixTranslation(position.x, position.y, position.z);
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z);
		XMMATRIX scaleMat = XMMatrixScaling(scale.x, scale.y, scale.z);

		// combine them and store the result
		XMMATRIX worldMat = scaleMat * rotMat * transMat;
		XMStoreFloat4x4(&worldMatrix, worldMat);

		matrixDirty = false;
	}

	return worldMatrix;
}
