#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "IndexBuffer.h"
#include "InputListener.h"
#include "Matrix4x4.h"

class AppWindow : public Window, public InputListener
{
public:
	AppWindow();
	~AppWindow();
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	virtual void onSize() override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
public:
	void update();
	void render();
	void updateCamera();
	void updateModel(Vector3D position, const MaterialPtr& material);
	void updateLight();
	void updateSkybox();
	void drawMesh(const MeshPtr& mesh, const MaterialPtr& material);
private:
	SwapChainPtr m_swap_chain;
	VertexBufferPtr m_vb;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	PixelShaderPtr m_sky_ps;
	ConstantBufferPtr m_cb;
	ConstantBufferPtr m_sky_cb;
	IndexBufferPtr m_ib;
	TexturePtr m_earth_tex;
	TexturePtr m_bricks_tex;
	TexturePtr m_earth_spec_tex;
	TexturePtr m_random_tex;
	TexturePtr m_sky_tex;
	TexturePtr m_earth_night_tex;
	MeshPtr m_mesh;
	MeshPtr m_sky_mesh;
	MaterialPtr m_material;
	MaterialPtr m_bricks_material;
	MaterialPtr m_earth_material;
	MaterialPtr m_sky_mat;
private:
	float m_old_delta;
	float m_new_delta;
	float m_delta_time;
	float m_delta_pos;
	float m_delta_scale;
	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;
	float m_scale_cube = 1.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	float m_light_rot_y = 0.0f;
	bool m_play_state = false;
	bool m_fullscreen_state = false;
	float m_time = 0.0f;
	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;
	Vector4D m_light_position;
};