#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include <memory>

class GameEntity
{
public:
	GameEntity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

	Transform* GetTransform();
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Material> GetMaterial();

	// Setters
	void SetTransform(Transform transform);
	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(std::shared_ptr<Material> material);

private:
	Transform transform;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

};

