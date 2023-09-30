#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <d3d11.h>
#include <vector>

struct MaterialSlot
{
	size_t start_index = 0;
	size_t num_index = 0;
	size_t material_id = 0;
};


class Mesh : public Resource
{
public:
	Mesh(const wchar_t* file_path);
	~Mesh();
	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();
	const MaterialSlot& getMaterialSlot(unsigned int slot);
	size_t getNumMaterialSlot();
private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
	std::vector<MaterialSlot> m_material_slot;
private:
	friend class DeviceContext;
};

