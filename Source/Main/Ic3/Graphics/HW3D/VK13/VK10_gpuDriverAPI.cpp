
#include "VK10UDriverAPI.h"
#include "ic3/GPUapiVK/VKUDriver.h"
#include "ic3/GPUapiVK/VKUDevice.h"
#include <Ic3/GPUapiVK/system/VK_presentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GPUDriverHandle VK10GPUDriverInterface::createDriver( const GPUDriverCreateInfo & pCreateInfo )
	{
		VKGPUDriverCreateInfo vk10CreateInfo;
		vk10CreateInfo.exfSystemContext = pCreateInfo.exfSystemContext;
		vk10CreateInfo.initFlags = pCreateInfo.initFlags;

		return VKGPUDriver::create( vk10CreateInfo );
	}

	PresentationLayerHandle VK10GPUDriverInterface::createScreenPresentationLayer( GPUDevice * pDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		VKPresentationLayerCreateInfo vk10CreateInfo;
		vk10CreateInfo.coreEngineState = pCreateInfo.coreEngineState;
		vk10CreateInfo.screenRect = pCreateInfo.screenRect;
		vk10CreateInfo.visualConfig = pCreateInfo.visualConfig;
		vk10CreateInfo.displayConfigFlags = pCreateInfo.displayConfigFlags;

		auto * deviceVK = pDevice->queryInterface<VKGPUDevice>();
		return VKScreenPresentationLayer::create( deviceVK, vk10CreateInfo );
	}

} // namespace Ic3::Graphics::GCI
