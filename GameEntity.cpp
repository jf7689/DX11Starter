#include "GameEntity.h"

GameEntity::GameEntity(std::shared_ptr<Mesh> _mesh)
{
	mesh = _mesh;
}

std::shared_ptr<Mesh> GameEntity::GetMesh()
{
	return mesh;
}

Transform* GameEntity::GetTransform()
{
	return &transform;
}
