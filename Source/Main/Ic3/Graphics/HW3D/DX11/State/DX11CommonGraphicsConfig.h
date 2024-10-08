
#ifndef __IC3_GRAPHICS_HW3D_DX11_COMMON_GRAPHICS_CONFIG_H__
#define __IC3_GRAPHICS_HW3D_DX11_COMMON_GRAPHICS_CONFIG_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/State/CommonGraphicsConfigImmutableStates.h>

namespace Ic3::Graphics::GCI
{

	struct DX11BlendConfig : public D3D11_BLEND_DESC
	{
		Math::RGBAColorR32Norm ConstantFactor;
	};

	struct DX11DepthStencilConfig : public D3D11_DEPTH_STENCIL_DESC
	{};

	struct DX11RasterizerConfig : public D3D11_RASTERIZER_DESC
	{};


	///
	class DX11BlendImmutableState : public BlendImmutableState
	{
	public:
		ComPtr<ID3D11BlendState> const mD3D11BlendState;

	public:
		DX11BlendImmutableState(
				DX11GPUDevice & pGPUDevice,
				Bitmask<EBlendConfigFlags> pBlendFlags,
				ComPtr<ID3D11BlendState> pD3D11BlendState );

		virtual ~DX11BlendImmutableState();

		static GpaHandle<DX11BlendImmutableState> createInstance( DX11GPUDevice & pGPUDevice, const BlendConfig & pBlendConfig );
	};

	///
	class DX11DepthStencilImmutableState : public DepthStencilImmutableState
	{
	public:
		ComPtr<ID3D11DepthStencilState> const mD3D11DepthStencilState;

	public:
		DX11DepthStencilImmutableState(
				DX11GPUDevice & pGPUDevice,
				Bitmask<EDepthStencilConfigFlags> pDepthStencilFlags,
				ComPtr<ID3D11DepthStencilState> pD3D11DepthStencilState );

		virtual ~DX11DepthStencilImmutableState();

		static GpaHandle<DX11DepthStencilImmutableState> createInstance( DX11GPUDevice & pGPUDevice, const DepthStencilConfig & pDepthStencilConfig );
	};

	///
	class DX11RasterizerImmutableState : public RasterizerImmutableState
	{
	public:
		ComPtr<ID3D11RasterizerState> const mD3D11RasterizerState;

	public:
		DX11RasterizerImmutableState(
				DX11GPUDevice & pGPUDevice,
				Bitmask<ERasterizerConfigFlags> pRasterizerFlags,
				ComPtr<ID3D11RasterizerState> pD3D11RasterizerState );

		virtual ~DX11RasterizerImmutableState();

		static GpaHandle<DX11RasterizerImmutableState> createInstance( DX11GPUDevice & pGPUDevice, const RasterizerConfig & pRasterizerConfig );
	};


	namespace smutil
	{

		IC3_ATTR_NO_DISCARD DX11BlendConfig translateDX11BlendConfig( const BlendConfig & pConfig );

		IC3_ATTR_NO_DISCARD DX11DepthStencilConfig translateDX11DepthStencilConfig( const DepthStencilConfig & pConfig );

		IC3_ATTR_NO_DISCARD DX11RasterizerConfig translateDX11RasterizerConfig( const RasterizerConfig & pConfig );

		IC3_ATTR_NO_DISCARD D3D11_RENDER_TARGET_BLEND_DESC translateDX11RTColorAttachmentBlendSettings(
			const RTColorAttachmentBlendSettings & pSettings );

	}

}

#endif // __IC3_GRAPHICS_HW3D_DX11_COMMON_GRAPHICS_CONFIG_H__
