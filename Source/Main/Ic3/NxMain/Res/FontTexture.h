
#pragma once

#ifndef __IC3_NXMAIN_RES_FONT_TEXTURE_H__
#define __IC3_NXMAIN_RES_FONT_TEXTURE_H__

#include "fontCommon.h"
#include <Ic3/Graphics/GCI/Resources/textureCommon.h>
#include <Ic3/Cppx/ByteArray.h>

namespace Ic3
{

	/// @brief Data of a texture object used by a font to store glyph images.
	struct FontTextureInfo
	{
		GCI::TextureHandle texture;

		// Texture dimension. Usually - a single 2D texture or an array of those.
		GCI::ETextureClass texClass;

		// Internal format of the texture.
		GCI::ETextureFormat format;

		// Number of sub-textures (layers) in the texture. 1 for a single texture,
		// 1 or more for texture arrays.
		uint32 textureArraySize;

		FontTextureInfo() = default;

		FontTextureInfo( std::nullptr_t )
		: texture( nullptr )
		, textureArraySize( 0 )
		{}

		explicit operator bool() const
		{
			return texture && ( textureArraySize > 0 );
		}
	};

	struct FontTextureCreateInfo
	{
		struct TextureLayerInitData
		{
			uint32 layerIndex;
			DynamicByteArray initDataBuffer;
		};

		std::vector<TextureLayerInitData> textureLayerInitDataArray;

		GCI::TextureCreateInfo gpuTextureCreateInfo;
	};

//	struct FontTextureSubDataDesc
//	{
//		const byte * data = nullptr;
//		size_t dataSize;
//		size_t dataRowPitch;
//		uint32 subTextureIndex;
//		Math::Vec2u32 targetOffset;
//		Math::Vec2u32 targetRect;
//
//		explicit operator bool() const
//		{
//			return data != nullptr;
//		}
//	};
//
//	class FontTexturePreloadCache
//	{
//	public:
//		using PreloadedSubDataArray = std::vector<FontTextureSubDataDesc>;
//
//		FontTexturePreloadCache() = default;
//
//		FontTexturePreloadCache( FontTexturePreloadCache && ) = default;
//		FontTexturePreloadCache & operator=( FontTexturePreloadCache && ) = default;
//
//		byte * allocateCacheMemory( size_t pMemorySize );
//
//		bool resize( size_t pCacheCapacity );
//
//		void reset();
//
//		const PreloadedSubDataArray & getPreloadedSubData() const;
//
//		bool empty() const;
//
//		bool isFull() const;
//
//	private:
//		size_t _dataBufferCapacity;
//		size_t _dataBufferAllocOffset;
//		DynamicMemoryBuffer _dataBuffer;
//		PreloadedSubDataArray _preloadedSubData;
//	};

} // namespace Ic3

#endif // __IC3_NXMAIN_RES_FONT_TEXTURE_H__
