#pragma once
#include "DXCore.h"
#include "Mesh.h"
#include "SimpleShader.h"
#include "Camera.h"
#include <memory>
#include <wrl/client.h>

class Sky
{
public:
	Sky(std::shared_ptr<Mesh> mesh,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState,
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		std::shared_ptr<SimpleVertexShader> vertexShader,
		std::shared_ptr<SimplePixelShader> pixelShader,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);	
	~Sky();	

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera); 

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencil;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> pixelShader;

};