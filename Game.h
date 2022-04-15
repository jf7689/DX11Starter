#pragma once

#include "DXCore.h"
#include "Mesh.h"
#include "Transform.h"
#include "GameEntity.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "Material.h"
#include "Light.h"
#include "Sky.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <memory>
#include <vector>

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Should we use vsync to limit the frame rate?
	bool vsync;

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateBasicGeometry();

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Simple Shaders
	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimplePixelShader> myShader;
	std::shared_ptr<SimpleVertexShader> skyVertexShader;
	std::shared_ptr<SimplePixelShader> skyPixelShader;

	// Shared ptr
	std::vector <std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<GameEntity>> gameEntities;
	std::vector < std::shared_ptr<Material> > materials;
	std::shared_ptr<Camera> camera;

	// Lights
	DirectX::XMFLOAT3 ambientLight;
	Light dirLight1;
	Light dirLight2;
	Light dirLight3;
	Light pointLight1;
	Light pointLight2;

	// Textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture3;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normal1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normal2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normal3;

	// PBR Textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> albedo1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normals1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness1;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> albedo2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normals2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness2;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> albedo3;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normals3;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness3;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness3;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> albedo4;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normals4;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness4;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness4;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> albedo5;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normals5;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness5;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness5;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> albedo6;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normals6;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness6;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness6;

	// Skybox
	std::shared_ptr<Sky> skybox;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyboxTexture;

	// Sampler State
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

};

