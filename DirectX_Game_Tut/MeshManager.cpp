#include "MeshManager.h"
#include "Mesh.h"

MeshManager::MeshManager()
{}

MeshManager::~MeshManager()
{}

MeshPtr MeshManager::createMeshFromFile(const wchar_t* file_path)
{
	return std::static_pointer_cast<Mesh>(createResourceFromFile(file_path));
}

Resource* MeshManager::createResourceFromFileConcrete(const wchar_t* file_Path)
{
	Mesh* mesh = nullptr;

	try
	{
		mesh = new Mesh(file_Path);
	}
	catch (...) {}

	return mesh;
}
