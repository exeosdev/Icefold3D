
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_SAMPLER_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_SAMPLER_H__

#include "../MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/Sampler.h>

#include <Metal/MTLSampler.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( MetalSampler );

	class  MetalSampler : public Sampler
	{
	public:
		id<MTLSamplerState> const mMTLSamplerState;

	public:
		MetalSampler( MetalGPUDevice & pGPUDevice, id<MTLSamplerState> pMTLSamplerState );
		virtual ~MetalSampler();

		static MetalSamplerHandle createSampler( MetalGPUDevice & pGPUDevice, const SamplerCreateInfo & pCreateInfo );

	private:
		static MTLSamplerDescriptor * translateSamplerConfig( const SamplerConfig & pSamplerConfig );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_SAMPLER_H__
