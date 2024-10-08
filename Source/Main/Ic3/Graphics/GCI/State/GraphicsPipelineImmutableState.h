
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_H__

#include "CommonGraphicsConfig.h"
#include "InputAssemblerCommon.h"

namespace Ic3::Graphics::GCI
{

	class Shader;

	enum class EGraphicsPipelineImmutableStateType : uint16
	{
		Blend,
		DepthStencil,
		ShaderLinkage,
		IAInputLayout,
		IAVertexStream,
		Rasterizer,
		RenderTargetBinding,
		RenderPassConfiguration,
		Unknown
	};

	class GraphicsPipelineImmutableState : public GPUDeviceChildObject
	{
	public:
		GraphicsPipelineImmutableState( const GraphicsPipelineImmutableState & ) = delete;
		GraphicsPipelineImmutableState & operator=( const GraphicsPipelineImmutableState & ) = delete;

		GraphicsPipelineImmutableState( GPUDevice & pGPUDevice );
		virtual ~GraphicsPipelineImmutableState();

		IC3_ATTR_NO_DISCARD virtual EGraphicsPipelineImmutableStateType queryStateType() const noexcept;

		IC3_ATTR_NO_DISCARD virtual bool isDynamicOverrideState() const noexcept;
	};

	/// @brief Dummy trait type for the descriptor cache. List all the things required for the traits to provide.
	class PipelineImmutableStateFactory
	{
	public:
		PipelineImmutableStateFactory() = default;
		virtual ~PipelineImmutableStateFactory() = default;

		virtual BlendImmutableStateHandle createBlendState( const BlendConfig & pConfig ) = 0;
		virtual DepthStencilImmutableStateHandle createDepthStencilState( const DepthStencilConfig & pConfig ) = 0;
		virtual GraphicsShaderLinkageImmutableStateHandle createGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet ) = 0;
		virtual IAInputLayoutImmutableStateHandle createIAInputLayoutState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary ) = 0;
		virtual IAVertexStreamImmutableStateHandle createIAVertexStreamState( const IAVertexStreamDefinition & pDefinition ) = 0;
		virtual RasterizerImmutableStateHandle createRasterizerState( const RasterizerConfig & pConfig ) = 0;
		virtual RenderTargetBindingImmutableStateHandle createRenderTargetBindingState( const RenderTargetBindingDefinition & pDefinition ) = 0;
		virtual RenderPassConfigurationImmutableStateHandle createRenderPassState( const RenderPassConfiguration & pConfiguration ) = 0;

	protected:
		virtual RenderPassConfigurationImmutableStateHandle createRenderPassStateDefault(
				GPUDevice & pGPUDevice,
				const RenderPassConfiguration & pConfiguration );
	};

	/// @brief
	class PipelineImmutableStateFactoryNull : public PipelineImmutableStateFactory
	{
	public:
		PipelineImmutableStateFactoryNull() = default;
		virtual ~PipelineImmutableStateFactoryNull() = default;

		virtual BlendImmutableStateHandle createBlendState( const BlendConfig & ) override final;
		virtual DepthStencilImmutableStateHandle createDepthStencilState( const DepthStencilConfig & ) override final;
		virtual GraphicsShaderLinkageImmutableStateHandle createGraphicsShaderLinkageState( const GraphicsShaderSet & ) override final;
		virtual IAInputLayoutImmutableStateHandle createIAInputLayoutState( const IAInputLayoutDefinition &, Shader & ) override final;
		virtual IAVertexStreamImmutableStateHandle createIAVertexStreamState( const IAVertexStreamDefinition & ) override final;
		virtual RasterizerImmutableStateHandle createRasterizerState( const RasterizerConfig & ) override final;
		virtual RenderTargetBindingImmutableStateHandle createRenderTargetBindingState( const RenderTargetBindingDefinition & ) override final;
		virtual RenderPassConfigurationImmutableStateHandle createRenderPassState( const RenderPassConfiguration & ) override final;
	};

	/// @brief
	class PipelineImmutableStateFactorySeparableShader : public PipelineImmutableStateFactory
	{
	public:
		GPUDevice & mGPUDevice;

	public:
		PipelineImmutableStateFactorySeparableShader( GPUDevice & pGPUDevice );
		virtual ~PipelineImmutableStateFactorySeparableShader();

		virtual GraphicsShaderLinkageImmutableStateHandle createGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet ) override;
	};

	class PipelineImmutableStateFactoryAdapter
	{
	public:
		PipelineImmutableStateFactoryAdapter( PipelineImmutableStateFactory & pStateFactory );
		~PipelineImmutableStateFactoryAdapter();

		BlendImmutableStateHandle createState( const BlendConfig & pConfig );
		DepthStencilImmutableStateHandle createState( const DepthStencilConfig & pConfig );
		GraphicsShaderLinkageImmutableStateHandle createState( const GraphicsShaderSet & pShaderSet );
		IAInputLayoutImmutableStateHandle createState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary );
		IAVertexStreamImmutableStateHandle createState( const IAVertexStreamDefinition & pDefinition );
		RasterizerImmutableStateHandle createState( const RasterizerConfig & pConfig );
		RenderTargetBindingImmutableStateHandle createState( const RenderTargetBindingDefinition & pDefinition );
		RenderPassConfigurationImmutableStateHandle createState( const RenderPassConfiguration & pConfiguration );

	private:
		PipelineImmutableStateFactory * _stateFactory;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_H__
