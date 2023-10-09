
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DRIVER_API_H__
#define __IC3_GRAPHICS_GCI_GPU_DRIVER_API_H__

#include "Prerequisites.h"
#include <functional>

namespace Ic3::Graphics::GCI
{

	struct GPUDriverCreateInfo;
	struct PresentationLayerCreateInfo;

	struct GPUDriverAPI
	{
		using CreateDriverFuncType = std::function<GPUDriverHandle( const GPUDriverCreateInfo & )>;
		using CreateScreenPresentationLayerFuncType = std::function<PresentationLayerHandle( GPUDevice &, const PresentationLayerCreateInfo & )>;

		CreateDriverFuncType createDriver;
		CreateScreenPresentationLayerFuncType createScreenPresentationLayer;
	};

	class GPUDriverInterface
	{
	public:
		GPUDriverInterface() = default;
		virtual ~GPUDriverInterface() = default;

		virtual GPUDriverHandle createDriver( const GPUDriverCreateInfo & pCreateInfo ) = 0;
		virtual PresentationLayerHandle createScreenPresentationLayer( GPUDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo ) = 0;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DRIVER_API_H__
