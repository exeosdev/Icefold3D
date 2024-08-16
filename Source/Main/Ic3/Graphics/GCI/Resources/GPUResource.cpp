
#include "GPUResource.h"
#include "../GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	GPUResource::GPUResource(
			GPUDevice & pGPUDevice,
			EGPUResourceBaseType pResourceBaseType,
			const ResourceMemoryInfo & pResourceMemory )
	: GPUDeviceChildObject( pGPUDevice )
	, mResourceBaseType( pResourceBaseType )
	, mResourceMemory( pResourceMemory )
	{}

	GPUResource::~GPUResource() = default;

	bool GPUResource::isMapped() const
	{
		return ( _mappedMemory.pointer != nullptr ) && !_mappedMemory.mappedRegion.empty();
	}

	bool GPUResource::isMapped( const GPUMemoryRegion & pRegion ) const
	{
		return Ic3::rangeIsSubRangeOf( pRegion.asRange(), _mappedMemory.mappedRegion.asRange() );
	}

	ref_counter_value_t GPUResource::addActiveRef()
	{
		return _activeRefsCounter.increment();
	}

	ref_counter_value_t GPUResource::releaseActiveRef()
	{
		const auto activeRefNum = _activeRefsCounter.decrement();

		if( mGPUDevice.isResourceActiveRefsTrackingEnabled() && ( activeRefNum == 0 ) )
		{
			mGPUDevice.onGPUResourceActiveRefsZero( *this );
		}

		return activeRefNum;
	}

	void GPUResource::setMappedMemory( const ResourceMappedMemory & pMappedMemory )
	{
		_mappedMemory = pMappedMemory;
		_mappedMemory.sourceMemory = &mResourceMemory;
	}

	void GPUResource::resetMappedMemory()
	{
		_mappedMemory.pointer = nullptr;
		_mappedMemory.memoryMapFlags = 0;
		_mappedMemory.mappedRegion.setEmpty();
	}


	GPUResourceView::GPUResourceView(
			GPUDevice & pGPUDevice,
			EGPUResourceBaseType pAliasedResourceType,
			Bitmask<resource_flags_value_t> pResourceFlags )
	: GPUDeviceChildObject( pGPUDevice )
	, mAliasedResourceType( pAliasedResourceType )
	, mResourceFlags( pResourceFlags )
	{}

	GPUResourceView::~GPUResourceView() = default;

} // namespace Ic3::Graphics::GCI
