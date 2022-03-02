#include "GameEntity.h"

GameEntity::GameEntity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) : transform()
{
	this->mesh = mesh;
	this->material = material;
}

Transform* GameEntity::GetTransform() { return &transform; }
std::shared_ptr<Mesh> GameEntity::GetMesh() { return mesh; }
std::shared_ptr<Material> GameEntity::GetMaterial() { return material; }

void GameEntity::SetTransform(Transform transform)
{
	this->transform = transform;
}

void GameEntity::SetMesh(std::shared_ptr<Mesh> mesh)
{
	this->mesh = mesh;
}

void GameEntity::SetMaterial(std::shared_ptr<Material> material)
{
	this->material = material;
}
