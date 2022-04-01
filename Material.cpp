#include "Material.h"

Material::Material(DirectX::XMFLOAT4 colorTint, std::shared_ptr<SimpleVertexShader> vertexShader, std::shared_ptr<SimplePixelShader> pixelShader, float roughness, float uvScale, DirectX::XMFLOAT2 uvOffset)
{
    this->colorTint = colorTint;
    this->vertexShader = vertexShader;
    this->pixelShader = pixelShader;
    this->roughness = roughness;
    this->uvScale = uvScale;
    this->uvOffset = uvOffset;
}

Material::~Material()
{
}

DirectX::XMFLOAT4 Material::GetColorTint() { return colorTint; }
std::shared_ptr<SimpleVertexShader> Material::GetVertexShader() { return vertexShader; }
std::shared_ptr<SimplePixelShader> Material::GetPixelShader() { return pixelShader; }
float Material::GetRoughness() { return roughness; }
float Material::GetUvScale() { return uvScale; }
DirectX::XMFLOAT2 Material::GetUvOffset() { return uvOffset; }

void Material::SetColorTint(DirectX::XMFLOAT4 colorTint)
{
    this->colorTint = colorTint;
}

void Material::SetVertexShader(std::shared_ptr<SimpleVertexShader> vertexShader)
{
    this->vertexShader = vertexShader;
}

void Material::SetPixelShader(std::shared_ptr<SimplePixelShader> pixelShader)
{
    this->pixelShader = pixelShader;
}

void Material::SetRoughness(float roughness)
{
    this->roughness = roughness;
}

void Material::SetUvScale(float uvScale)
{
    this->uvScale = uvScale;
}

void Material::SetUvOffset(DirectX::XMFLOAT2 uvOffset)
{
    this->uvOffset = uvOffset;
}

void Material::AddTextureSRV(std::string name, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
    textureSRVs.insert({ name, texture });
}

void Material::AddSampler(std::string name, Microsoft::WRL::ComPtr<ID3D11SamplerState> state)
{
    samplers.insert({ name, state });
}

void Material::SetMaps()
{
    for (auto& t : textureSRVs) { pixelShader->SetShaderResourceView(t.first.c_str(), t.second); }
    for (auto& s : samplers) { pixelShader->SetSamplerState(s.first.c_str(), s.second); }
    pixelShader->CopyAllBufferData();
}
