#include "AppWindow.h"

struct vec_3
{
	float x, y, z;
};

struct vertex
{
	vec_3 position;
};

AppWindow::AppWindow()
{}

AppWindow::~AppWindow()
{}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] =
	{
		{-0.5f, -0.5f, 0.0f},
		{-0.5f, 0.5f, 0.0f},
		{0.5f , -0.5f, 0.0f},
		{0.5f, 0.5f, 0.0f}
	};

	m_vertex_buffer = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->createShaders();
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vertex_buffer->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vertex_buffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vertex_buffer->getSizeVertexList(), 0);
	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vertex_buffer->release();
	m_swap_chain->release();
	GraphicsEngine::get()->release();
}
