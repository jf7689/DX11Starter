#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include "Vertex.h"

class Mesh
{
public:
	Mesh(Vertex* _vertices, int _nVertices, unsigned int* _indices, int _nIndicies, 
		Microsoft::WRL::ComPtr<ID3D11Device> _device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context);
	Mesh(const char* objFile, Microsoft::WRL::ComPtr<ID3D11Device> _device);
	~Mesh();

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vb;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ib;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	// number of indicies
	int nIndicies;

};
