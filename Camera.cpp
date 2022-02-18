#include "Camera.h"
#include "Input.h"

using namespace DirectX;

Camera::Camera(float x, float y, float z, float aspectRatio, float fieldOfView, float nearPlane, float farPlane)
{
    // Set up the initial transform and matrices
    transform.SetPosition(x, y, z);
    this->aspectRatio = aspectRatio;
    this->fieldOfView = fieldOfView;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    UpdateViewMatrix();
    UpdateProjectionMatrix(aspectRatio);
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
    // Camera's Speed
    float speed = dt * 5.0f;

    // Take in user input
    Input& input = Input::GetInstance();    // Gets Singleton Instance

    // Adjust speed based on speed modifier keys
    if (input.KeyDown(VK_SHIFT)) { speed *= 5.0f; }
    if (input.KeyDown(VK_CONTROL)) { speed *= 0.5f; }

    // Move based on keyboard input
    if (input.KeyDown('W')) { transform.MoveRelative(0, 0, speed); }
    if (input.KeyDown('S')) { transform.MoveRelative(0, 0, -speed); }
    if (input.KeyDown('A')) { transform.MoveRelative(-speed, 0, 0); }
    if (input.KeyDown('D')) { transform.MoveRelative(speed, 0, 0); }
    if (input.KeyDown('X')) { transform.MoveAbsolute(0, -speed, 0); }
    if (input.KeyDown(' ')) { transform.MoveAbsolute(0, speed, 0); }

    // Handle mouse movement when left click is down
    if (input.MouseLeftDown())
    {
        // Calculate the cursor change
        float lookSpeed = dt * 5.0f;
        float xDiff = input.GetMouseXDelta() * lookSpeed;
        float yDiff = input.GetMouseYDelta() * lookSpeed;

        // Rotate the transform based on the mouse movement
        transform.Rotate(yDiff, xDiff, 0);
    }

    UpdateViewMatrix();

    // Update field of view
    if (input.KeyDown(VK_OEM_PLUS)) { fieldOfView += dt; }
    if (input.KeyDown(VK_OEM_MINUS)) { fieldOfView -= dt; }
    UpdateProjectionMatrix(aspectRatio);

}

void Camera::UpdateViewMatrix()
{
    XMFLOAT3 pos = transform.GetPosition();
    XMFLOAT3 up = transform.GetUp();
    XMFLOAT3 forward = transform.GetForward();

    // Update and store the view matrix
    XMMATRIX view = XMMatrixLookToLH(XMLoadFloat3(&pos), XMLoadFloat3(&forward), XMLoadFloat3(&up));
    XMStoreFloat4x4(&viewMatrix, view);
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
    // update aspect atio
    this->aspectRatio = aspectRatio;

    // Creates a projection matrix 
    XMMATRIX proj = XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);
    XMStoreFloat4x4(&projectionMatrix, proj);
}

Transform* Camera::GetTransform(){ return &transform;}
DirectX::XMFLOAT4X4 Camera::GetViewMatrix(){ return viewMatrix;}
DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix(){ return projectionMatrix; }
