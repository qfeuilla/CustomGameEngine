#ifndef GRAPHICS_H
#define GRAPHICS_H

#pragma once
#include "CustomWindows.h"
#include "WndException.h"
#include <d3d11.h>
#include <wrl.h>

class Graphics
{
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
	void ClearBuffer(float red, float green, float blue) noexcept;

	void DrawTestTriangle(float angle);

private:
	
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
};

#endif