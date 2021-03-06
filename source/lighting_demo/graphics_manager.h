/* The Halfling Project - A Graphics Engine and Projects
 *
 * The Halfling Project is the legal property of Adrian Astley
 * Copyright Adrian Astley 2013
 */

#pragma once

#include "common/graphics_manager_base.h"
#include "common/structured_buffer.h"
#include "common/lights.h"
#include "common/materials.h"

#include <d3d11.h>
#include "DirectXMath.h"
#include <AntTweakBar.h>

namespace LightingDemo {

class GameStateManager;

struct VertexShaderFrameConstants {
	DirectX::XMMATRIX viewProj;
	DirectX::XMMATRIX proj;
};

struct VertexShaderObjectConstants {
	DirectX::XMMATRIX worldViewProj;
	DirectX::XMMATRIX world;
};

struct PixelShaderFrameConstants {
	Common::DirectionalLight directionalLight;
	DirectX::XMFLOAT3 eyePosition;
	float pad;
};

struct PixelShaderObjectConstants {
	Common::Material material;
};

class GraphicsManager : public Common::GraphicsManagerBase {
public:
	GraphicsManager(GameStateManager *gameStateManager);

private:
	GameStateManager *m_gameStateManager;

	bool m_vsync;
	bool m_wireframe;

	ID3D11RenderTargetView *m_renderTargetView;
	ID3D11InputLayout *m_inputLayout;

	TwBar *m_frameStatsBar;
	TwBar *m_settingsBar;

	// Shaders
	ID3D11VertexShader *m_vertexShader;
	ID3D11PixelShader *m_pixelShader;

	ID3D11Buffer *m_vertexShaderFrameConstantsBuffer;
	ID3D11Buffer *m_vertexShaderObjectConstantsBuffer;
	ID3D11Buffer *m_pixelShaderFrameConstantsBuffer;
	ID3D11Buffer *m_pixelShaderObjectConstantsBuffer;

	// We assume there is only one directional light. Therefore, it is stored in a cbuffer
	Common::StructuredBuffer<Common::PointLight> *m_pointLightBuffer;
	Common::StructuredBuffer<Common::SpotLight> *m_spotLightBuffer;

	ID3D11RasterizerState *m_wireframeRS;
	ID3D11RasterizerState *m_solidRS;

public:
	bool Initialize(int clientWidth, int clientHeight, HWND hwnd, bool fullscreen);
	void Shutdown();
	void DrawFrame(float deltaTime);
	void SetFrameConstants(DirectX::XMMATRIX &projMatrix, DirectX::XMMATRIX &viewProjMatrix);
	void SetObjectConstants(DirectX::XMMATRIX &worldMatrix, DirectX::XMMATRIX &worldViewProjMatrix, const Common::Material &material);
	void SetLightBuffers(DirectX::XMMATRIX &viewMatrix);
	void OnResize(int newClientWidth, int newClientHeight);

	static void TW_CALL SetWireframeRSCallback(const void *value, void *clientData);
	static void TW_CALL GetWireframeTSCallback(void *value, void *clientData);

private:
	void InitTweakBar();
	void LoadShaders();
};

} // End of namespace CrateDemo
