
#include "DX12GPUDriverAPI.h"
#include "DX12GPUDriver.h"
#include "DX12GPUDevice.h"
#include <Ic3/Graphics/HW3D/DX12/system/DX12_presentationLayer.h>

namespace Ic3::Graphics::GCI
{

	GPUDriverHandle DX12GPUDriverInterface::createDriver( const GPUDriverCreateInfo & pCreateInfo )
	{
		DX12GPUDriverCreateInfo dx12CreateInfo;
		dx12CreateInfo.exfSystemContext = pCreateInfo.exfSystemContext;
		dx12CreateInfo.configFlags = pCreateInfo.configFlags;

		return DX12GPUDriver::create( dx12CreateInfo );
	}

	PresentationLayerHandle DX12GPUDriverInterface::createScreenPresentationLayer( GPUDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo )
	{
		DX12PresentationLayerCreateInfo dx12CreateInfo;
		dx12CreateInfo.coreEngineState = pCreateInfo.coreEngineState;
		dx12CreateInfo.screenRect = pCreateInfo.screenRect;
		dx12CreateInfo.visualConfig = pCreateInfo.visualConfig;
		dx12CreateInfo.displayConfigFlags = pCreateInfo.displayConfigFlags;

		if( pDevice.isDebugDevice() )
		{
			dx12CreateInfo.dxgiFlags.set( DXGI_CREATE_FACTORY_DEBUG );
		}

		auto * deviceDX12 = pDevice.queryInterface<DX12GPUDevice>();
		return DX12ScreenPresentationLayer::create( *deviceDX12, dx12CreateInfo );
	}

} // namespace Ic3::Graphics::GCI
