#include <Windows.h>

#include "AppWindow.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
	Vector4D m_light_position = Vector4D(0, 1, 0, 0);
	float m_light_radius = 4.0f;
	float m_time = 0.0f;
};

AppWindow::AppWindow()
{}

void AppWindow::update()
{
	updateCamera();
	updateLight();
	updateSkybox();
}

void AppWindow::render()
{
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

	//updateModel(Vector3D(0, 0, 0), m_material);
	//drawMesh(m_sky_mesh, m_material);

	//updateModel(Vector3D(4, 0, 0), m_earth_material);
	//drawMesh(m_sky_mesh, m_earth_material);

	//updateModel(Vector3D(-4, 0, 0), m_bricks_material);
	//drawMesh(m_sky_mesh, m_bricks_material);

	m_list_materials.clear();
	m_list_materials.push_back(m_barrel_material);
	m_list_materials.push_back(m_bricks_material);
	m_list_materials.push_back(m_windows_material);
	m_list_materials.push_back(m_wood_material);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			updateModel(Vector3D(-14.0f + 14.0f * i, 0, -14.0f + 14.0f * j), m_list_materials);
			drawMesh(m_house_mesh, m_list_materials);
		}
	}

	m_list_materials.clear();
	m_list_materials.push_back(m_terrain_mat);
	updateModel(Vector3D(0, 0, 0), m_list_materials);
	drawMesh(m_terrain_mesh, m_list_materials);

	m_list_materials.clear();
	m_list_materials.push_back(m_sky_mat);

	drawMesh(m_sky_mesh, m_list_materials);

	m_swap_chain->present(true);


	m_old_delta = m_new_delta;
	m_new_delta = (float)::GetTickCount64();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
	m_time += m_delta_time;
}

void AppWindow::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.1f);
	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.1f);
	world_cam.setTranslation(new_pos);
	m_world_cam = world_cam;

	world_cam.inverse();
	m_view_cam = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);
	m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
}

void AppWindow::updateModel(Vector3D position, const std::vector<MaterialPtr>& list_materials)
{
	constant cc;

	Matrix4x4 temp;
	Matrix4x4 m_light_rotation_matrix;
	m_light_rotation_matrix.setIdentity();
	m_light_rotation_matrix.setRotationY(m_light_rot_y);

	cc.m_world.setIdentity();
	cc.m_world.setTranslation(position);
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();

	cc.m_light_position = m_light_position;
	cc.m_light_radius = m_light_radius;
	cc.m_light_direction = m_light_rotation_matrix.getZDirection();
	cc.m_time = m_time;
	for (auto m : list_materials)
	{
		m->setData(&cc, sizeof(constant));
	}
}

void AppWindow::updateLight()
{
	m_light_rot_y += 1.57f * m_delta_time;

	float dist_from_origin = 3.0f;
	//m_light_position = Vector4D(cos(m_light_rot_y) * dist_from_origin, 1.0f, sin(m_light_rot_y) * dist_from_origin, 1.0f);
	m_light_position = Vector4D(180, 140, 70, 1.0f);
}

void AppWindow::updateSkybox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(100.0f, 100.0f, 100.0f));
	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	m_sky_mat->setData(&cc, sizeof(constant));
}

void AppWindow::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	for (size_t m = 0; m < mesh->getNumMaterialSlot(); m++)
	{
		if (m >= list_materials.size()) break;

		MaterialSlot mat = mesh->getMaterialSlot(m);
		GraphicsEngine::get()->setMaterial(list_materials[m]);
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mat.num_index, 0, mat.start_index);
	}
}


AppWindow::~AppWindow()
{}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	m_play_state = true;
	InputSystem::get()->showCursor(false);

	m_earth_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wall.jpg");
	//m_bricks_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	//m_random_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_wood.jpg");
		//m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\scene.obj");
	m_sand_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sand.jpg");
	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.jpg");
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	m_terrain_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\terrain.obj");
	m_house_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\house.obj");

	m_barrel_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\barrel.jpg");
	m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_wood.jpg");
	m_brick_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_brick.jpg");
	m_windows_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_windows.jpg");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -1));

	m_material = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"PointLightPixelShader.hlsl");
	m_material->addTexture(m_earth_tex);
	m_material->setCullMode(CULL_MODE_BACK);

	//m_bricks_material = GraphicsEngine::get()->createMaterial(m_material);
	//m_bricks_material->addTexture(m_bricks_tex);
	//m_bricks_material->setCullMode(CULL_MODE_BACK);

	//m_earth_material = GraphicsEngine::get()->createMaterial(m_material);
	//m_earth_material->addTexture(m_random_tex);
	//m_earth_material->setCullMode(CULL_MODE_BACK);

	m_terrain_mat = GraphicsEngine::get()->createMaterial(m_material);
	m_terrain_mat->addTexture(m_sand_tex);
	m_terrain_mat->setCullMode(CULL_MODE_BACK);

	m_barrel_material = GraphicsEngine::get()->createMaterial(m_material);
	m_barrel_material->addTexture(m_barrel_tex);
	m_barrel_material->setCullMode(CULL_MODE_BACK);

	m_wood_material = GraphicsEngine::get()->createMaterial(m_material);
	m_wood_material->addTexture(m_wood_tex);
	m_wood_material->setCullMode(CULL_MODE_BACK);

	m_bricks_material = GraphicsEngine::get()->createMaterial(m_material);
	m_bricks_material->addTexture(m_brick_tex);
	m_bricks_material->setCullMode(CULL_MODE_BACK);

	m_windows_material = GraphicsEngine::get()->createMaterial(m_material);
	m_windows_material->addTexture(m_windows_tex);
	m_windows_material->setCullMode(CULL_MODE_BACK);

	m_sky_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SkyBoxShader.hlsl");
	m_sky_mat->addTexture(m_sky_tex);
	m_sky_mat->setCullMode(CULL_MODE_FRONT);

	m_list_materials.reserve(32);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();
	this->render();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right, rc.bottom);
	this->render();
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		m_rightward = 1.0f;
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == 'G')
	{
		m_play_state = (m_play_state) ? false : true;
		InputSystem::get()->showCursor(!m_play_state);
	}
	else if (key == 'F')
	{
		m_fullscreen_state = (m_fullscreen_state) ? false : true;
		RECT size_screen = this->getSizeScreen();
		if (m_fullscreen_state)
		{
			m_swap_chain->setFullScreen(m_fullscreen_state, size_screen.right, size_screen.bottom);
		}
		else
		{
			m_swap_chain->setFullScreen(false, 1024, 768);
		}
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	if (!m_play_state) return;
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);
	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;
	InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}