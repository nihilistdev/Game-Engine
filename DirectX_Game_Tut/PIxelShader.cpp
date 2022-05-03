#include "PixelShader.h"
#include "GraphicsEngine.h"

PixelShader::PixelShader()
{}

PixelShader::~PixelShader()
{}

bool PixelShader::release()
{
	m_ps->Release();
	delete this;
	return false;
}

bool PixelShader::init(void* shader_byte_code, size_t byte_code_size)
{
	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_ps)))
		return false;
	return true;
}