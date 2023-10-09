
#include "MTLGPUDevice.h"
#include "MTLGPUDriver.h"

namespace Ic3::Graphics::GCI
{

	MetalGPUDevice::MetalGPUDevice( MetalGPUDriver & pGPUDriver, System::MetalDeviceHandle pSysMetalDevice )
	: GPUDevice( pGPUDriver )
	, mSysMetalDevice( std::move( pSysMetalDevice ) )
	, mMTLDevice( mSysMetalDevice->mDeviceData->mtlDevice )
	, _immutableStateFactoryGL( *this )
	, _immutableStateCache( _immutableStateFactoryGL )
	{}

	MetalGPUDevice::~MetalGPUDevice() = default;

	void MetalGPUDevice::waitForCommandSync( CommandSync & pCommandSync )
	{}

	void MetalGPUDevice::initializeCommandSystem()
	{}

	bool MetalGPUDevice::_drvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer )
	{}

	GPUBufferHandle MetalGPUDevice::_drvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo )
	{}

	SamplerHandle MetalGPUDevice::_drvCreateSampler( const SamplerCreateInfo & pCreateInfo )
	{}

	ShaderHandle MetalGPUDevice::_drvCreateShader( const ShaderCreateInfo & pCreateInfo )
	{}

	TextureHandle MetalGPUDevice::_drvCreateTexture( const TextureCreateInfo & pCreateInfo )
	{}

	RenderTargetTextureHandle MetalGPUDevice::_drvCreateRenderTargetTexture(
			const RenderTargetTextureCreateInfo & pCreateInfo )
	{}

	GraphicsPipelineStateObjectHandle MetalGPUDevice::_drvCreateGraphicsPipelineStateObject(
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{}
	
}
