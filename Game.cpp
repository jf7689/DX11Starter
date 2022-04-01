#include "Game.h"
#include "Vertex.h"
#include "Input.h"
#include "WICTextureLoader.h"

// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true),			   // Show extra stats (fps) in title bar?
	vsync(false)
{
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

	// camera creation
	camera = std::make_shared<Camera>(0.0f, 0.0f, -20.0f, (float)width / height, XM_PIDIV4, 0.01f, 1000.0f);
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object created in Game

}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateBasicGeometry();
	
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Sets up the light
	ambientLight = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	// Directinal Light 1
	dirLight1 = {};
	dirLight1.Type = 0;
	dirLight1.Direction = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	dirLight1.Color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	dirLight1.Intensity = 0.1f;

	// Directional Light 2
	dirLight2 = {};
	dirLight2.Type = 0;
	dirLight2.Direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	dirLight2.Color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	dirLight2.Intensity = 0.2f;

	// Directional Light 3
	dirLight3 = {};
	dirLight3.Type = 0;
	dirLight3.Direction = DirectX::XMFLOAT3(-1.0f, 1.0f, -0.5f);
	dirLight3.Color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	dirLight3.Intensity = 0.15f;

	// Point Light 1
	pointLight1 = {};
	pointLight1.Type = 1;
	pointLight1.Range = 10.0f;
	pointLight1.Position = DirectX::XMFLOAT3(-3.0f, 0.0f, 0.0f);
	pointLight1.Intensity = 0.1f;
	pointLight1.Color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	// Point Light 2
	pointLight2 = {};
	pointLight2.Type = 1;
	pointLight2.Range = 10.0f;
	pointLight2.Position = DirectX::XMFLOAT3(4.0f, 1.0f, 0.0f);
	pointLight2.Intensity = 0.25f;
	pointLight2.Color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	// Load simple shaders
	vertexShader = std::make_shared<SimpleVertexShader>(device, context, GetFullPathTo_Wide(L"VertexShader.cso").c_str());
	pixelShader = std::make_shared<SimplePixelShader>(device, context, GetFullPathTo_Wide(L"PixelShader.cso").c_str());
	myShader = std::make_shared<SimplePixelShader>(device, context, GetFullPathTo_Wide(L"CustomPS.cso").c_str());
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
#pragma region Old Meshes
	/*
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	// - Note: Since we don't have a camera or really any concept of
	//    a "3d world" yet, we're simply describing positions within the
	//    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	// - This means (0,0) is at the very center of the screen.
	// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	//    Screen Coords", which are ways to describe a position without
	//    knowing the exact size (in pixels) of the image/window/etc.  
	// - Long story short: Resizing the window also resizes the triangle,
	//    since we're describing the triangle in terms of the window itself
	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), red },
		{ XMFLOAT3(+0.5f, -0.5f, +0.0f), blue },
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f), green },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	unsigned int indices[] = { 0, 1, 2 };

	std::shared_ptr<Mesh> triangle = std::make_shared<Mesh>(vertices, 3, indices, 3, device, context);
	meshes.push_back(triangle);

	// Vertex array for rectangle
	Vertex rectVertices[] =
	{
		{ XMFLOAT3(-0.8f, +0.8f, +0.0f), red },   // top left
		{ XMFLOAT3(-0.6f, +0.8f, +0.0f), blue },  // top right
		{ XMFLOAT3(-0.6f, +0.6f, +0.0f), green }, // bottom right
		{ XMFLOAT3(-0.8f, +0.6f, +0.0f), blue },  // bottom left
	};

	// Index array for pentagon
	unsigned int rectIndices[] = { 
		0, 1, 2, 
		0, 2, 3 
	};
	std::shared_ptr<Mesh> rectangle = std::make_shared<Mesh>(rectVertices, 4, rectIndices, 6, device, context);
	meshes.push_back(rectangle);

	Vertex pentagonVertices[] =
	{
		{ XMFLOAT3( 0.75f, +0.5f, +0.0f), red },  // center
		{ XMFLOAT3( 0.75f, +0.75f, +0.0f), red },  // top
		{ XMFLOAT3( 0.9f, +0.55f, +0.0f), green }, // right
		{ XMFLOAT3( 0.83f, +0.25f, +0.0f), green }, // bottom right
		{ XMFLOAT3( 0.67f, +0.25f, +0.0f), blue },  // bottom left
		{ XMFLOAT3( 0.6f, +0.55f, +0.0f), blue },  // left
	};

	// Index array for pentagon
	unsigned int pentagonIndices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 1
	};
	std::shared_ptr<Mesh> pentagon = std::make_shared<Mesh>(pentagonVertices, 6, pentagonIndices, 15, device, context);
	meshes.push_back(pentagon);
	*/
#pragma endregion
	// Create sampler state
	D3D11_SAMPLER_DESC ssd = {};
	ssd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	ssd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	ssd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	ssd.Filter = D3D11_FILTER_ANISOTROPIC;
	ssd.MaxAnisotropy = 16;
	ssd.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&ssd, samplerState.GetAddressOf());

	// Load in textures
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/tiledTexture.png").c_str(), nullptr, texture1.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/woodTexture.png").c_str(), nullptr, texture2.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/grassTexture.png").c_str(), nullptr, texture3.GetAddressOf());

	// Create materials
	materials.push_back(std::make_shared<Material>(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), vertexShader, pixelShader, 0.15f, 2.0f, XMFLOAT2(0.0f, 0.0f)));
	materials.push_back(std::make_shared<Material>(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), vertexShader, pixelShader, 0.15f, 15.0f, XMFLOAT2(0.5f, 0.5f)));
	materials.push_back(std::make_shared<Material>(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), vertexShader, pixelShader, 0.15f, 4.0f, XMFLOAT2(1.0f, 0.0f)));
	materials.push_back(std::make_shared<Material>(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), vertexShader, pixelShader, 0.15f, 0.2f, XMFLOAT2(1.0f, 1.0f)));
	materials.push_back(std::make_shared<Material>(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), vertexShader, pixelShader, 0.15f, 5.0f, XMFLOAT2(0.25f, 0.25f)));

	// Add textures
	materials[0]->AddTextureSRV("SurfaceTexture", texture1);
	materials[0]->AddSampler("BasicSampler", samplerState);

	materials[1]->AddTextureSRV("SurfaceTexture", texture1);
	materials[1]->AddSampler("BasicSampler", samplerState);

	materials[2]->AddTextureSRV("SurfaceTexture", texture2);
	materials[2]->AddSampler("BasicSampler", samplerState);

	materials[3]->AddTextureSRV("SurfaceTexture", texture3);
	materials[3]->AddSampler("BasicSampler", samplerState);

	materials[4]->AddTextureSRV("SurfaceTexture", texture3);
	materials[4]->AddSampler("BasicSampler", samplerState);

	// Creates meshes from 3D object
	std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device);
	meshes.push_back(cube);
	std::shared_ptr<Mesh> cylinder = std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/cylinder.obj").c_str(), device);
	meshes.push_back(cylinder);
	std::shared_ptr<Mesh> helix = std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/helix.obj").c_str(), device);
	meshes.push_back(helix);
	std::shared_ptr<Mesh> sphere = std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device);
	meshes.push_back(sphere);
	std::shared_ptr<Mesh> torus = std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/torus.obj").c_str(), device);
	meshes.push_back(torus);


#pragma region Create old game entities
	// gameEntities.push_back(std::make_shared<GameEntity>(meshes[0], materials[0]));
	// gameEntities.push_back(std::make_shared<GameEntity>(meshes[0], materials[0]));
	// gameEntities.push_back(std::make_shared<GameEntity>(meshes[1], materials[1]));
	// gameEntities.push_back(std::make_shared<GameEntity>(meshes[2], materials[2]));
	// gameEntities.push_back(std::make_shared<GameEntity>(meshes[2], materials[2]));
#pragma endregion

	gameEntities.push_back(std::make_shared<GameEntity>(meshes[0], materials[0]));
	gameEntities.push_back(std::make_shared<GameEntity>(meshes[1], materials[4]));
	gameEntities.push_back(std::make_shared<GameEntity>(meshes[2], materials[3]));
	gameEntities.push_back(std::make_shared<GameEntity>(meshes[3], materials[1]));
	gameEntities.push_back(std::make_shared<GameEntity>(meshes[4], materials[2]));
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	camera->UpdateProjectionMatrix((float)width / height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Example input checking: Quit if the escape key is pressed
	if (Input::GetInstance().KeyDown(VK_ESCAPE))
		Quit();

#pragma region Transform old meshes
	/*
	float scale = cos(totalTime) * 0.5f + 0.5f;
	gameEntities[0]->GetTransform()->SetPosition(sin(totalTime), 0, 0);
	gameEntities[0]->GetTransform()->SetScale(scale, scale, scale);
	gameEntities[0]->GetTransform()->Rotate(0, 0, deltaTime * 0.2f);

	gameEntities[1]->GetTransform()->SetPosition(cos(totalTime), 0, 0);
	gameEntities[1]->GetTransform()->SetScale(scale, scale, scale);
	gameEntities[1]->GetTransform()->Rotate(0, 0, deltaTime * 0.5f);

	gameEntities[2]->GetTransform()->SetPosition(cos(totalTime) * 1.5f, 0, 0);
	gameEntities[2]->GetTransform()->Rotate(0, 0, deltaTime * 0.5f);

	gameEntities[3]->GetTransform()->Rotate(0, 0, deltaTime * 0.5f);
	gameEntities[4]->GetTransform()->SetScale(scale, scale, scale);
	*/
#pragma endregion

	gameEntities[0]->GetTransform()->SetPosition(-10.0f, 0.0f, 0.0f);
	gameEntities[1]->GetTransform()->SetPosition(-5.0f, 0.0f, 0.0f);
	gameEntities[2]->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	gameEntities[3]->GetTransform()->SetPosition(5.0f, 0.0f, 0.0f);
	gameEntities[4]->GetTransform()->SetPosition(10.0f, 0.0f, 0.0f);

	camera->Update(deltaTime);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	// context->VSSetShader(vertexShader.Get(), 0, 0);
	// context->PSSetShader(pixelShader.Get(), 0, 0);


	// Ensure the pipeline knows how to interpret the data (numbers)
	// from the vertex buffer.  
	// - If all of your 3D models use the exact same vertex layout,
	//    this could simply be done once in Init()
	// - However, this isn't always the case (but might be for this course)
	// context->IASetInputLayout(inputLayout.Get());

	// draw all meshes in gameEntities
	for (int i = 0; i < gameEntities.size(); i++)
	{
		// Set the current shaders
		gameEntities[i]->GetMaterial()->GetVertexShader()->SetShader();
		gameEntities[i]->GetMaterial()->GetPixelShader()->SetShader();

		// Define Vertex data
		std::shared_ptr<SimpleVertexShader> vs = gameEntities[i]->GetMaterial()->GetVertexShader();
		vs->SetMatrix4x4("world", gameEntities[i]->GetTransform()->GetWorldMatrix());
		vs->SetMatrix4x4("worldInvTranspose", gameEntities[i]->GetTransform()->GetWorldInverseTransposeMatrix());
		vs->SetMatrix4x4("view", camera->GetViewMatrix());
		vs->SetMatrix4x4("projection", camera->GetProjectionMatrix());
		vs->CopyAllBufferData();

		// Define Pixel Shader data
		std::shared_ptr<SimplePixelShader> ps = gameEntities[i]->GetMaterial()->GetPixelShader();
		ps->SetFloat4("colorTint", gameEntities[i]->GetMaterial()->GetColorTint());
		ps->SetFloat3("cameraPosition", camera->GetTransform()->GetPosition());
		ps->SetFloat("roughness", gameEntities[i]->GetMaterial()->GetRoughness());
		ps->SetFloat3("cameraPos", camera->GetTransform()->GetPosition());
		ps->SetFloat3("ambientLight", ambientLight);
		ps->SetFloat("uvScale", gameEntities[i]->GetMaterial()->GetUvScale());
		ps->SetFloat2("uvOffset", gameEntities[i]->GetMaterial()->GetUvOffset());
		ps->SetData("dirLight1", &dirLight1, sizeof(Light));
		ps->SetData("dirLight2", &dirLight2, sizeof(Light));
		ps->SetData("dirLight3", &dirLight3, sizeof(Light));
		ps->SetData("pointLight1", &pointLight1, sizeof(Light));
		ps->SetData("pointLight2", &pointLight2, sizeof(Light));
		gameEntities[i]->GetMaterial()->SetMaps();
		ps->CopyAllBufferData();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		context->IASetVertexBuffers(0, 1, gameEntities[i]->GetMesh()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(gameEntities[i]->GetMesh()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		// Draws meshes
		context->DrawIndexed(
			gameEntities[i]->GetMesh()->GetIndexCount(),
			0,
			0);
	}
	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(vsync ? 1 : 0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}