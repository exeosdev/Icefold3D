
#include "Texture.h"
#include "RenderTargetTexture.h"

namespace Ic3::Graphics::GCI
{

	Texture::Texture(
			GPUDevice & pGPUDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const TextureProperties & pTextureProperties,
			const TextureLayout & pTextureLayout )
	: GPUResource( pGPUDevice, EGPUResourceBaseType::Texture, pResourceMemory )
	, mTextureProperties( pTextureProperties )
	, mTextureLayout( pTextureLayout )
	{}

	Texture::~Texture() = default;

	const GPUResourceProperties & Texture::getProperties() const
	{
		return mTextureProperties;
	}

	TextureSubResource Texture::getAllSubResourcesRef() const
	{
		switch( mTextureLayout.texClass )
		{
			case ETextureClass::T2D:
			{
				TextureSubResource2D subResource2D;
				subResource2D.mipLevel = CxDef::TEX_SUBRESOURCE_MIP_LEVEL_ALL_MIPS;
				return TextureSubResource{ subResource2D, ETextureClass::T2D };
			}
			case ETextureClass::T2DArray:
			{
				TextureSubResource2DArray subResource2DArray;
				subResource2DArray.arrayIndex = CxDef::TEX_SUBRESOURCE_ARRAY_INDEX_ALL_TEXTURES;
				subResource2DArray.mipLevel = CxDef::TEX_SUBRESOURCE_MIP_LEVEL_ALL_MIPS;
				return TextureSubResource{ subResource2DArray, ETextureClass::T2DArray };
			}
			case ETextureClass::T2DMS:
			{
				TextureSubResource2D subResource2DMS;
				subResource2DMS.mipLevel = 0; // MSAA textures only have a single sub-resource (mip level 0).
				return TextureSubResource{ subResource2DMS, ETextureClass::T2DMS };
			}
			case ETextureClass::T2DMSArray:
			{
				TextureSubResource2DArray subResource2DMSArray;
				subResource2DMSArray.arrayIndex = CxDef::TEX_SUBRESOURCE_ARRAY_INDEX_ALL_TEXTURES;
				subResource2DMSArray.mipLevel = 0; // MSAA textures only have a single sub-resource (mip level 0).
				return TextureSubResource{ subResource2DMSArray, ETextureClass::T2DMSArray };
			}
			case ETextureClass::T3D:
			{
				TextureSubResource3D subResource3D;
				subResource3D.mipLevel = CxDef::TEX_SUBRESOURCE_MIP_LEVEL_ALL_MIPS;
				subResource3D.depthLayerIndex = CxDef::TEX_SUBRESOURCE_DEPTH_ALL_LAYERS;
				return TextureSubResource{ subResource3D };
			}
			case ETextureClass::TCubeMap:
			{
				TextureSubResourceCubeMap subResourceCubeMap;
				subResourceCubeMap.faceIndex = CxDef::TEX_SUBRESOURCE_CUBE_MAP_FACE_ALL_FACES;
				subResourceCubeMap.mipLevel = CxDef::TEX_SUBRESOURCE_MIP_LEVEL_ALL_MIPS;
				return TextureSubResource{ subResourceCubeMap };
			}
			default:
			{
				return TextureSubResource{};
			}
		}
	}

	TextureSubResource Texture::getDefaultSubResourceRef() const
	{
		switch( mTextureLayout.texClass )
		{
			case ETextureClass::T2D:
			{
				TextureSubResource2D subResource2D;
				subResource2D.mipLevel = 0;
				return TextureSubResource{ subResource2D, ETextureClass::T2D };
			}
			case ETextureClass::T2DArray:
			{
				TextureSubResource2DArray subResource2DArray;
				subResource2DArray.arrayIndex = 0;
				subResource2DArray.mipLevel = 0;
				return TextureSubResource{ subResource2DArray, ETextureClass::T2DArray };
			}
			case ETextureClass::T2DMS:
			{
				TextureSubResource2D subResource2DMS;
				subResource2DMS.mipLevel = 0;
				return TextureSubResource{ subResource2DMS, ETextureClass::T2DMS };
			}
			case ETextureClass::T2DMSArray:
			{
				TextureSubResource2DArray subResource2DMSArray;
				subResource2DMSArray.arrayIndex = 0;
				subResource2DMSArray.mipLevel = 0;
				return TextureSubResource{ subResource2DMSArray, ETextureClass::T2DMSArray };
			}
			case ETextureClass::T3D:
			{
				TextureSubResource3D subResource3D;
				subResource3D.mipLevel = 0;
				subResource3D.depthLayerIndex = CxDef::TEX_SUBRESOURCE_DEPTH_ALL_LAYERS;
				return TextureSubResource{ subResource3D };
			}
			case ETextureClass::TCubeMap:
			{
				TextureSubResourceCubeMap subResourceCubeMap;
				subResourceCubeMap.faceIndex = E_TEXTURE_CUBE_MAP_FACE_FIRST;
				subResourceCubeMap.mipLevel = 0;
				return TextureSubResource{ subResourceCubeMap };
			}
			default:
			{
				return TextureSubResource{};
			}
		}
	}

	RenderTargetTextureHandle Texture::createDefaultRenderTargetTextureView(
			GPUDevice & pGPUDevice,
			const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		if( pCreateInfo.targetTexture )
		{
			const auto rttType = rcutil::queryRenderTargetTextureType( pCreateInfo.targetTexture->mTextureLayout.internalFormat );
			const auto rttLayout = rcutil::queryRenderTargetTextureLayout( pCreateInfo.targetTexture->mTextureLayout );

			auto existingTextureRTT = createGPUAPIObject<RenderTargetTexture>( pGPUDevice, rttType, rttLayout, pCreateInfo.targetTexture );

			return existingTextureRTT;
		}

		return nullptr;
	}


	namespace rcutil
	{

		TextureDimensions getValidTextureDimensions( ETextureClass pTexClass, const TextureDimensions & pDimensions )
		{
			TextureDimensions validTextureDimensions = pDimensions;

			if( pTexClass == ETextureClass::T2D )
			{
				validTextureDimensions.arraySize = 1;
				validTextureDimensions.depth = 1;
			}
			else if( pTexClass == ETextureClass::T2DArray )
			{
				validTextureDimensions.depth = 1;
			}
			else if( pTexClass == ETextureClass::T2DMS )
			{
				validTextureDimensions.arraySize = 1;
				validTextureDimensions.depth = 1;
				validTextureDimensions.mipLevelsNum = 1;
			}
			else if( pTexClass == ETextureClass::T2DMSArray )
			{
				validTextureDimensions.depth = 1;
				validTextureDimensions.mipLevelsNum = 1;
			}
			else if( pTexClass == ETextureClass::T3D )
			{
				validTextureDimensions.arraySize = 1;
			}
			else if( pTexClass == ETextureClass::TCubeMap )
			{
				validTextureDimensions.depth = 1;
				validTextureDimensions.arraySize = 1;
			}

			return validTextureDimensions;
		}

	}

} // namespace Ic3::Graphics::GCI
