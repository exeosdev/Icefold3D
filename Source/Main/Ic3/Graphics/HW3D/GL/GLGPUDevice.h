
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_DEVICE_H__

#include "GLAPITranslationLayer.h"
#include "State/GLPipelineImmutableStateFactory.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <Ic3/Graphics/GCI/State/PipelineImmutableStateCache.h>

namespace Ic3::Graphics::GCI
{

	class GLCommandContext;
	class GLGraphicsPipelineStateController;
	class GLGraphicsPipelineStateObject;
	class GLVertexStreamStateObject;

	/// @brief
	class GLGPUDevice : public GPUDevice
	{
		friend class GLCommandContext;
		friend class GLCommandList;
		friend class GLGraphicsPipelineStateObject;

	public:
		System::OpenGLSystemDriverHandle const mSysGLDriver;

		System::OpenGLVersionSupportInfo const mSysGLSupportInfo;

		Bitmask<EGLRuntimeSupportFlags> const mGLRuntimeSupportFlags;

	public:
		explicit GLGPUDevice( GLGPUDriver & pGPUDriver, GLPipelineImmutableStateFactory & pImmutableStateFactory );
		virtual ~GLGPUDevice();

		IC3_ATTR_NO_DISCARD virtual bool isCompatibilityDevice() const noexcept = 0;

		GLDebugOutput * getDebugOutputInterface() const;

		virtual void waitForCommandSync( CommandSync & pCommandSync ) override;

	protected:
		bool initializeGLDebugOutput();

		virtual void initializeCommandSystem() override;

	private:
	    virtual bool _drvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer ) override;

		virtual GPUBufferHandle _drvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo ) override final;
	    virtual SamplerHandle _drvCreateSampler( const SamplerCreateInfo & pCreateInfo ) override final;
		virtual ShaderHandle _drvCreateShader( const ShaderCreateInfo & pCreateInfo ) override final;
		virtual TextureHandle _drvCreateTexture( const TextureCreateInfo & pCreateInfo ) override final;

		virtual RenderTargetTextureHandle _drvCreateRenderTargetTexture(
				const RenderTargetTextureCreateInfo & pCreateInfo ) override final;

		virtual GraphicsPipelineStateObjectHandle _drvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo ) override final;

	private:
		GLPipelineImmutableStateFactory * _immutableStateFactoryGL;
		PipelineImmutableStateCache _immutableStateCache;
		std::unique_ptr<GLDebugOutput> _glDebugOutput;
	};

	class GLGPUDeviceCore : public GLGPUDevice
	{
	public:
		explicit GLGPUDeviceCore( GLGPUDriver & pGPUDriver );
		virtual ~GLGPUDeviceCore();

		virtual bool isCompatibilityDevice() const noexcept override final;

	private:
		GLPipelineImmutableStateFactoryCore _immutableStateFactoryCore;
	};

	class GLGPUDeviceCompat : public GLGPUDevice
	{
	public:
		explicit GLGPUDeviceCompat( GLGPUDriver & pGPUDriver );
		virtual ~GLGPUDeviceCompat();

		virtual bool isCompatibilityDevice() const noexcept override final;

	private:
		GLPipelineImmutableStateFactoryCompat _immutableStateFactoryCompat;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_DEVICE_H__
