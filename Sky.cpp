#include "Sky.h"

Sky::Sky(std::shared_ptr<Mesh> mesh, Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState, Microsoft::WRL::ComPtr<ID3D11Device> device, std::shared_ptr<SimpleVertexShader> vertexShader, std::shared_ptr<SimplePixelShader> pixelShader, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	this->samplerState = samplerState;
	skyTexture = texture;
	this->mesh = mesh;
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;

	// Creates a rasterizer state
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_FRONT;
	device->CreateRasterizerState(&rastDesc, rasterizerState.GetAddressOf());

	// Creates a depth stencil
	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&depthDesc, depthStencil.GetAddressOf());

}

Sky::~Sky()
{
}

void Sky::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera)
{
	// Set rasterizer and depth stencil 
	context->RSSetState(rasterizerState.Get());
	context->OMSetDepthStencilState(depthStencil.Get(), 0);

	// Activate shaders
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Sets shader variables
	vertexShader->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	vertexShader->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());
	vertexShader->CopyAllBufferData();

	pixelShader->SetShaderResourceView("skybox", skyTexture);
	pixelShader->SetSamplerState("samplerState", samplerState);
	pixelShader->CopyAllBufferData();

	// Draw 
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Vertex and Index Buffer
	context->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	// Draws to screen
	context->DrawIndexed(
		mesh->GetIndexCount(),
		0,
		0);

	// Reset
	context->RSSetState(nullptr);
	context->OMSetDepthStencilState(nullptr, 0);
}
