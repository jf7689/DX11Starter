#pragma once

#include <DirectXMath.h>
#include <memory>
#include "SimpleShader.h"

class Material
{
public:
	Material(DirectX::XMFLOAT4 colorTint, std::shared_ptr<SimpleVertexShader> vertexShader, std::shared_ptr<SimplePixelShader> pixelShader);
	~Material();

	// Getters
	DirectX::XMFLOAT4 GetColorTint();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	std::shared_ptr<SimplePixelShader> GetPixelShader();

	// Setters
	void SetColorTint(DirectX::XMFLOAT4 colorTint);
	void SetVertexShader(std::shared_ptr<SimpleVertexShader> vertexShader);
	void SetPixelShader(std::shared_ptr<SimplePixelShader> pixelShader);

private:
	DirectX::XMFLOAT4 colorTint;
	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> pixelShader;

};

