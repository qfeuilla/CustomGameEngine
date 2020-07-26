#ifndef GRAPHICS_H
#define GRAPHICS_H

#pragma once
#include "CustomWindows.h"
#include "WndException.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>
#include "GraphicsThrowMacros.h"
#include "imgui/imgui_impl_dx11.h"
#include "ConditionalNoexcept.h"

#ifdef DEBUG
#define IS_DEBUG 1
#else
#define IS_DEBUG 0
#endif

namespace Bind
{
	class Bindable;
}

class Graphics
{
	friend class Bind::Bindable;
public:
	class Exception : public WndException {
		using WndException::WndException;
	};
	class HrException : public Exception {
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class DeviceRemovedException : public HrException {
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	};

	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void EndFrame();
	void BeginFrame(float red, float green, float blue) noexcept;

	void DrawIndexed(UINT count) noxnd;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;

	void SetCamera(DirectX::XMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;

	void EnableImgui() noexcept;
	void DisableImgui() noexcept;
	bool IsImguiEnabled() const noexcept;

private:
	bool imguiEnabled = true;

	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};

#endif