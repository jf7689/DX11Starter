#pragma once

#include <DirectXMath.h>
#include <memory>
#include <unordered_map>
#include "SimpleShader.h"

class Material
{
public:
	Material(DirectX::XMFLOAT4 colorTint, std::shared_ptr<SimpleVertexShader> vertexShader, std::shared_ptr<SimplePixelShader> pixelShader, float roughness, float uvScale, DirectX::XMFLOAT2 uvOffset);
	~Material();

	// Getters
	DirectX::XMFLOAT4 GetColorTint();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	std::shared_ptr<SimplePixelShader> GetPixelShader();
	float GetRoughness();
	float GetUvScale();
	DirectX::XMFLOAT2 GetUvOffset();

	// Setters
	void SetColorTint(DirectX::XMFLOAT4 colorTint);
	void SetVertexShader(std::shared_ptr<SimpleVertexShader> vertexShader);
	void SetPixelShader(std::shared_ptr<SimplePixelShader> pixelShader);
	void SetRoughness(float roughness);
	void SetUvScale(float uvScale);
	void SetUvOffset(DirectX::XMFLOAT2 uvOffset);

	// Textures
	void AddTextureSRV(std::string name, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);
	void AddSampler(std::string name, Microsoft::WRL::ComPtr<ID3D11SamplerState> state);
	void SetMaps();

private:
	DirectX::XMFLOAT4 colorTint;
	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> pixelShader;
	float roughness;
	float uvScale;
	DirectX::XMFLOAT2 uvOffset;

	// Unordered maps - C# Dictionaries
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureSRVs;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11SamplerState>> samplers;

};

