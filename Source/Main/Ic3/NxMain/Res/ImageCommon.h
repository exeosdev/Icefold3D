
#ifndef __IC3_NXMAIN_IMAGE_COMMON_H__
#define __IC3_NXMAIN_IMAGE_COMMON_H__

#include "resourceCommon.h"
#include <Ic3/CoreLib/graphicsTypes.h>
#include <Ic3/Cppx/MemoryBuffer.h>

namespace Ic3
{

	using ImageDataBuffer = DynamicMemoryBuffer;

	struct ImageFormatInfo
	{
		Math::Vec2u32 dimensions;
		EPixelDataLayout pixelLayout;
		uint16 bitDepth = 0;
		uint16 pixelByteSize = 0;
	};

	struct ImageData
	{
	public:
		ImageFormatInfo formatInfo;
		ImageDataBuffer pixelBuffer;
		size_t sizeInBytes = 0;

	public:
		ImageData( const ImageData & ) = delete;
		ImageData & operator=( const ImageData & ) = delete;

		ImageData() = default;
		ImageData( ImageData && ) = default;
		ImageData & operator=( ImageData && ) = default;

		ImageData( std::nullptr_t )
		: ImageData()
		{}

		explicit operator bool() const
		{
			return !empty() && ( formatInfo.pixelLayout != EPixelDataLayout::Undefined );
		}

		void reset();

		void release();

		ImageData clone() const;

		bool empty() const;

		void swap( ImageData & pOther );
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_IMAGE_COMMON_H__
