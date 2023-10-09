
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_PIPELINE_STATE_CONTROLLER_H__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_PIPELINE_STATE_CONTROLLER_H__

#include "DX11inputAssembler.h"
#include "DX11RenderTarget.h"
#include <Ic3/Graphics/GCI/State/SeparablePipelineState.h>

namespace Ic3::Graphics::GCI
{

	class DX11GraphicsPipelineStateObject;

	/// @brief
	class DX11GraphicsPipelineStateController : public GraphicsPipelineStateControllerSeparableShader
	{
		friend class DX11CommandList;

	public:
		GPUDevice & mGPUDevice;
		DX11CommandList * const mDX11CommandList = nullptr;
		ID3D11DeviceContext1 * const mD3D11DeviceContext1 = nullptr;

	public:
		DX11GraphicsPipelineStateController( DX11CommandList & pDX11CommandList );
		~DX11GraphicsPipelineStateController();

		IC3_ATTR_NO_DISCARD const DX11IAVertexStreamDefinition & getCurrentIAVertexStreamDefinition() const noexcept;

		IC3_ATTR_NO_DISCARD DX11RenderTargetBindingData getCurrentRenderTargetBinding() const noexcept;

		virtual bool applyStateChanges() override;

		virtual bool setGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO ) override;
		virtual bool resetGraphicsPipelineStateObject() override;

		virtual bool setIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState ) override;
		virtual bool setIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState ) override;
		virtual bool resetIAVertexStreamState() override;

		virtual bool setRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState ) override;
		virtual bool setRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState ) override;
		virtual bool resetRenderTargetBindingState() override;

		virtual bool setViewport( const ViewportDesc & pViewportDesc ) override;
		virtual bool setShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData ) override;
		virtual bool setShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer ) override;
		virtual bool setShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture ) override;
		virtual bool setShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler ) override;

	private:
		Bitmask<uint32> applyCommonGraphicsConfigState( const DX11GraphicsPipelineStateObject & pGraphicsPSO );
		Bitmask<uint32> applyGraphicsShaderState( const SeparableShaderCache & pSeparableShaders );
		void applyIAVertexStreamState( const DX11IAVertexStreamDefinition & pVertexStreamDefinition );
		void applyRenderTargetBinding( const DX11RenderTargetBindingData & pRenderTargetBindingData );

	private:
		DX11IAVertexStreamDefinition _dynamicIAVertexStreamDefinitionDX11;
		DX11RenderTargetBindingData _dynamicRenderTargetBindingDataDX11;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_PIPELINE_STATE_CONTROLLER_H__
