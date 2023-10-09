
#include "GLTexture.h"
#include "GLRenderBuffer.h"
#include <Ic3/Graphics/HW3D/GL/GLAPITranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>

namespace Ic3::Graphics::GCI
{

	GLTexture::GLTexture(
			GLGPUDevice & pGPUDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const TextureProperties & pTextureProperties,
			const TextureLayout & pTextureLayout,
			GLTextureObjectHandle pGLTextureObject )
	: Texture( pGPUDevice, pResourceMemory, pTextureProperties, pTextureLayout )
	, mGLTextureObject( std::move( pGLTextureObject ) )
	{ }

	GLTexture::~GLTexture() = default;

	GLTextureHandle GLTexture::createDefault( GLGPUDevice & pGPUDevice, const TextureCreateInfo & pCreateInfo )
	{
		GLTextureCreateInfo openglCreateInfo;
		openglCreateInfo.bindTarget = ATL::translateGLTextureBindTarget( pCreateInfo.texClass );
		openglCreateInfo.dimensions = rcutil::getValidTextureDimensions( pCreateInfo.texClass, pCreateInfo.dimensions );
		openglCreateInfo.msaaLevel = pCreateInfo.msaaLevel;
		openglCreateInfo.internalFormat = ATL::translateGLTextureInternalFormat( pCreateInfo.internalFormat );
		openglCreateInfo.openglInitDataDesc.subTextureInitDataPtr = pCreateInfo.initDataDesc.subTextureInitDataBasePtr;
		openglCreateInfo.openglInitDataDesc.textureInitFlags = pCreateInfo.initDataDesc.textureInitFlags;
		auto textureInitDataBaseType = CxDefs::getTextureFormatBaseDataType( pCreateInfo.internalFormat );
		openglCreateInfo.openglInitDataDesc.openglPixelDataLayout = ATL::translateGLTexturePixelDataLayout( pCreateInfo.internalFormat );
		openglCreateInfo.openglInitDataDesc.openglPixelDataType = ATL::translateGLBaseDataType( textureInitDataBaseType );

		GLTextureObjectHandle openglTextureObject = nullptr;
		if( pGPUDevice.isCompatibilityDevice() )
		{
			openglTextureObject = GLTextureObject::createCompat( openglCreateInfo );
			ic3DebugAssert( openglTextureObject );
		}
		else
		{
			openglTextureObject = GLTextureObject::createCore( openglCreateInfo );
			ic3DebugAssert( openglTextureObject );
		}

		const auto glcTextureInternalFormat = openglTextureObject->queryInternalFormat();
		const auto glcTextureByteSize = openglTextureObject->querySize();

		ResourceMemoryInfo textureMemoryInfo;
		textureMemoryInfo.sourceHeapRegion.offset = 0;
		textureMemoryInfo.sourceHeapRegion.size = glcTextureByteSize;
		textureMemoryInfo.baseAlignment = pCreateInfo.memoryBaseAlignment;
		textureMemoryInfo.memoryFlags = pCreateInfo.memoryFlags;

		TextureProperties textureProperties;
		textureProperties.resourceFlags = pCreateInfo.resourceFlags;

		TextureLayout textureLayout;
		textureLayout.texClass = pCreateInfo.texClass;
		textureLayout.dimensions = pCreateInfo.dimensions;
		textureLayout.internalFormat = pCreateInfo.internalFormat;
		textureLayout.msaaLevel = pCreateInfo.msaaLevel;
		textureLayout.storageSize = numeric_cast<uint32>( glcTextureByteSize );
		textureLayout.bitsPerPixel = ATL::queryGLTextureInternalFormatBPP( glcTextureInternalFormat );

		auto openglTexture = createDynamicInterfaceObject<GLTexture>(
			pGPUDevice,
			textureMemoryInfo,
			textureProperties,
			textureLayout,
			std::move( openglTextureObject ) );

		return openglTexture;
	}

	GLTextureHandle GLTexture::createForRenderTarget( GLGPUDevice & pGPUDevice, const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		TextureCreateInfo textureCreateInfo;
		textureCreateInfo.texClass = ETextureClass::T2D;
		textureCreateInfo.dimensions.width = pCreateInfo.rtTextureLayout.imageRect.width;
		textureCreateInfo.dimensions.height = pCreateInfo.rtTextureLayout.imageRect.height;
		textureCreateInfo.memoryFlags = E_GPU_MEMORY_ACCESS_FLAG_GPU_READ_BIT;
		textureCreateInfo.resourceFlags = ( pCreateInfo.bindFlags & E_GPU_RESOURCE_USAGE_MASK_ALL );
		textureCreateInfo.internalFormat = pCreateInfo.rtTextureLayout.internalFormat;
		textureCreateInfo.dimensions.arraySize = 1;
		textureCreateInfo.dimensions.depth = 1;
		textureCreateInfo.dimensions.mipLevelsNum = 1;

		auto glcTexture = GLTexture::createDefault( pGPUDevice, textureCreateInfo );

		return glcTexture;
	}

	RenderTargetTextureHandle GLTexture::createRenderTargetTextureView( GLGPUDevice & pGPUDevice, const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		if( pCreateInfo.targetTexture )
		{
			return createDefaultRenderTargetTextureView( pGPUDevice, pCreateInfo );
		}

		const auto renderBufferIncompatibleBindFlags =
				E_GPU_RESOURCE_USAGE_FLAG_SHADER_INPUT_BIT |
				E_GPU_RESOURCE_USAGE_FLAG_RENDER_TARGET_COLOR_BIT |
				E_GPU_RESOURCE_USAGE_FLAG_TRANSFER_TARGET_BIT;

		const auto rtTextureType = rcutil::queryRenderTargetTextureType( pCreateInfo.targetTexture->mTextureLayout.internalFormat );

		if( pCreateInfo.bindFlags.isSetAnyOf( renderBufferIncompatibleBindFlags ) )
		{
			auto glcTexture = createForRenderTarget( pGPUDevice, pCreateInfo );
			if( !glcTexture )
			{
				return nullptr;
			}

			auto textureRTView = createGPUAPIObject<RenderTargetTexture>(
					pGPUDevice,
					rtTextureType,
					pCreateInfo.rtTextureLayout,
					TextureReference{ glcTexture } );

			return textureRTView;
		}
		else
		{
			auto glcRenderBuffer = GLInternalRenderBuffer::createInstance( pGPUDevice, pCreateInfo );
			if( !glcRenderBuffer )
			{
				return nullptr;
			}

			auto renderBufferRTView = createGPUAPIObject<RenderTargetTexture>(
					pGPUDevice,
					rtTextureType,
					pCreateInfo.rtTextureLayout,
					glcRenderBuffer,
					pCreateInfo.bindFlags );

			return renderBufferRTView;
		}
	}

} // namespace Ic3::Graphics::GCI
