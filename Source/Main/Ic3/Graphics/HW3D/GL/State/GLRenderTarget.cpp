
#include "GLRenderTarget.h"
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLFramebufferObject.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLRenderbufferObject.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLRenderBuffer.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLTexture.h>

namespace Ic3::Graphics::GCI
{

	GLRenderTargetBindingImmutableState::GLRenderTargetBindingImmutableState(
			GLGPUDevice & pGPUDevice,
			const RenderTargetLayout & pRenderTargetLayout,
			GLRenderTargetBindingFBOData pGLFBOData )
	: RenderTargetBindingImmutableState( pGPUDevice, pRenderTargetLayout )
	, mGLFBOData( std::move( pGLFBOData ) )
	{}

	GLRenderTargetBindingImmutableState::~GLRenderTargetBindingImmutableState() = default;

	GLRenderTargetBindingInfo GLRenderTargetBindingImmutableState::getGLRenderTargetBindingInfo() const
	{
		GLRenderTargetBindingInfo glcRTBindingInfo{};
		glcRTBindingInfo.renderFBO = mGLFBOData.renderFBO.get();
		glcRTBindingInfo.resolveFBO = mGLFBOData.resolveFBO.get();
		glcRTBindingInfo.rtLayout = &mRenderTargetLayout;
		return glcRTBindingInfo;
	}

	GpaHandle<GLRenderTargetBindingImmutableState> GLRenderTargetBindingImmutableState::createInstance(
			GLGPUDevice & pGPUDevice,
			const RenderTargetBindingDefinition & pBindingDefinition )
	{
		if( !smutil::validateRenderTargetBindingDefinition( pBindingDefinition ) )
		{
			return nullptr;
		}

		auto glcRenderTargetBindingDefinition = smutil::translateRenderTargetBindingDefinition( pBindingDefinition );

		auto immutableState = createGPUAPIObject<GLRenderTargetBindingImmutableState>(
				pGPUDevice,
				glcRenderTargetBindingDefinition.rtLayout,
				std::move( glcRenderTargetBindingDefinition.fboData ) );

		return immutableState;
	}

	GpaHandle<GLRenderTargetBindingImmutableState> GLRenderTargetBindingImmutableState::createForScreen(
			GLGPUDevice & pGPUDevice,
			const RenderTargetLayout & pRenderTargetLayout )
	{
		GLRenderTargetBindingDefinition glcRenderTargetBindingDefinition;
		glcRenderTargetBindingDefinition.fboData.renderFBO = GLFramebufferObject::createForDefaultFramebuffer();
		glcRenderTargetBindingDefinition.fboData.resolveFBO = nullptr;
		glcRenderTargetBindingDefinition.rtLayout = pRenderTargetLayout;

		auto immutableState = createGPUAPIObject<GLRenderTargetBindingImmutableState>(
			pGPUDevice,
			glcRenderTargetBindingDefinition.rtLayout,
			std::move( glcRenderTargetBindingDefinition.fboData ) );

		return immutableState;
	}


	namespace smutil
	{

		GLRenderTargetBindingInfo getGLRenderTargetBindingInfo(
				const GLRenderTargetBindingDefinition & pBindingDefinition )
		{
			GLRenderTargetBindingInfo glcRTBindingInfo{};
			glcRTBindingInfo.renderFBO = pBindingDefinition.fboData.renderFBO.get();
			glcRTBindingInfo.resolveFBO = pBindingDefinition.fboData.resolveFBO.get();
			glcRTBindingInfo.rtLayout = &( pBindingDefinition.rtLayout );
			return glcRTBindingInfo;
		}

		GLRenderTargetBindingDefinition translateRenderTargetBindingDefinition(
				const RenderTargetBindingDefinition & pBindingDefinition )
		{
			GLRenderTargetBindingDefinition glcRenderTargetBindingDefinition{};

			const auto renderTargetLayout = smutil::getRenderTargetLayoutForBindingDefinition( pBindingDefinition );

			glcRenderTargetBindingDefinition.fboData.renderFBO =
					smutil::createFramebufferObject( pBindingDefinition, pBindingDefinition.activeAttachmentsMask );

			glcRenderTargetBindingDefinition.fboData.resolveFBO =
					smutil::createFramebufferObject( pBindingDefinition, pBindingDefinition.attachmentsActionResolveMask );

			glcRenderTargetBindingDefinition.rtLayout = renderTargetLayout;

			return glcRenderTargetBindingDefinition;
		}

		GLFramebufferObjectHandle createFramebufferObject(
				const RenderTargetBindingDefinition & pBindingDefinition,
				Bitmask<ERTAttachmentFlags> pAttachmentMask )
		{
			if( pAttachmentMask.empty() )
			{
				return nullptr;
			}

			auto framebufferObject = GLFramebufferObject::create();
			if( !framebufferObject )
			{
				return nullptr;
			}

			for( uint32 caIndex = 0; CxDefs::isRTColorAttachmentIndexValid( caIndex ); ++caIndex )
			{
				const auto attachmentBit = CxDefs::makeRTAttachmentFlag( caIndex );
				if( pAttachmentMask.isSet( attachmentBit ) )
				{
					auto & colorAttachmentBinding = pBindingDefinition.colorAttachments[caIndex];
					auto & textureReference = colorAttachmentBinding.attachmentTexture->mTargetTexture;
					auto * openglTexture = textureReference.getRefTexture()->queryInterface<GLTexture>();

					framebufferObject->bindColorTexture(
							caIndex,
							*( openglTexture->mGLTextureObject ),
							textureReference.getRefSubResource() );
				}
			}

			if( pAttachmentMask.isSet( E_RT_ATTACHMENT_FLAG_DEPTH_STENCIL_BIT ) )
			{
				auto & depthStencilAttachmentBinding = pBindingDefinition.depthStencilAttachment;

				if( depthStencilAttachmentBinding.attachmentTexture->isDepthStencilRenderBuffer() )
				{
					auto * renderBuffer = depthStencilAttachmentBinding.attachmentTexture->getInternalRenderBuffer<GLInternalRenderBuffer>();

					framebufferObject->bindDepthStencilRenderbuffer(
							*renderBuffer->mGLRenderbufferObject,
							depthStencilAttachmentBinding.attachmentTexture->mRTBufferMask );
				}
				else
				{
					auto & textureReference = depthStencilAttachmentBinding.attachmentTexture->mTargetTexture;
					auto * openglTexture = textureReference.getRefTexture()->queryInterface<GLTexture>();

					framebufferObject->bindDepthStencilTexture(
							*openglTexture->mGLTextureObject,
							textureReference.getRefSubResource(),
							depthStencilAttachmentBinding.attachmentTexture->mRTBufferMask );
				}
			}

			if( !pAttachmentMask.isSetAnyOf( E_RT_ATTACHMENT_MASK_COLOR_ALL ) )
			{
				glDrawBuffer( GL_NONE );
				ic3OpenGLHandleLastError();
			}

			if( !framebufferObject->checkStatus() )
			{
				ic3DebugInterrupt();
				return nullptr;
			}

			return framebufferObject;
		}

		RenderTargetLayout translateSystemVisualConfigToRenderTargetLayout( const System::VisualConfig & pSysVisualConfig )
		{
			RenderTargetLayout renderTargetLayout;
			renderTargetLayout.activeAttachmentsMask = E_RT_ATTACHMENT_MASK_DEFAULT_C0_DS;
			renderTargetLayout.colorAttachments[0].format = ETextureFormat::BGRA8UN;
			renderTargetLayout.depthStencilAttachment.format = ETextureFormat::D24UNS8U;
			return renderTargetLayout;
		}

		void clearRenderPassFramebuffer(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState )
		{
			const bool clearConfigOverride = pDynamicState.activeStateMask.isSet( E_GRAPHICS_PIPELINE_DYNAMIC_STATE_FLAG_COMMON_CLEAR_CONFIG_BIT );

			if( pRenderPassConfiguration.attachmentsActionClearMask != 0 )
			{
				auto colorAttachmentsClearMask = pRenderPassConfiguration.attachmentsActionClearMask & E_RT_ATTACHMENT_MASK_COLOR_ALL;
				for( uint32 caIndex = 0; CxDefs::isRTColorAttachmentIndexValid( caIndex ); ++caIndex )
				{
					const auto attachmentBit = CxDefs::makeRTAttachmentFlag( caIndex );
					if( colorAttachmentsClearMask.isSet( attachmentBit ) )
					{
						const auto & attachmentConfig = pRenderPassConfiguration.colorAttachments[caIndex];
						if( attachmentConfig.clearMask.isSet( E_RENDER_TARGET_BUFFER_FLAG_COLOR_BIT ) )
						{
							const auto * clearConfig = clearConfigOverride ? &pDynamicState.commonClearConfig : &attachmentConfig.clearConfig;

							GLfloat clearArray[4] = {
								clearConfig->colorValue.fpRed,
								clearConfig->colorValue.fpGreen,
								clearConfig->colorValue.fpBlue,
								clearConfig->colorValue.fpAlpha
							};

							glClearBufferfv( GL_COLOR, caIndex, clearArray );
							ic3OpenGLHandleLastError();
						}
						colorAttachmentsClearMask.unset( attachmentBit );
					}

					if( colorAttachmentsClearMask.empty() )
					{
						break;
					}
				}

				if( pRenderPassConfiguration.attachmentsActionClearMask.isSet( E_RT_ATTACHMENT_FLAG_DEPTH_STENCIL_BIT ) )
				{
					const auto & attachmentConfig = pRenderPassConfiguration.depthStencilAttachment;
					const auto * clearConfig = clearConfigOverride ? &pDynamicState.commonClearConfig : &attachmentConfig.clearConfig;

					if( attachmentConfig.clearMask.isSet( E_RENDER_TARGET_BUFFER_FLAG_DEPTH_BIT ) )
					{
						const GLfloat depthValue = clearConfig->depthValue;
						glClearBufferfv( GL_DEPTH, 0, &depthValue );
						ic3OpenGLHandleLastError();
					}

					if( attachmentConfig.clearMask.isSet( E_RENDER_TARGET_BUFFER_FLAG_STENCIL_BIT ) )
					{
						const GLint stencilValue = clearConfig->stencilValue;
						glClearBufferiv( GL_STENCIL, 0, &stencilValue );
						ic3OpenGLHandleLastError();
					}
				}
			}
		}

		void resolveRenderPassFramebuffer(
				const GLRenderTargetBindingInfo & pRTBindingInfo,
				const RenderPassConfiguration & pRenderPassConfiguration )
		{
			if( pRenderPassConfiguration.attachmentsActionResolveMask != 0 )
			{
				ic3DebugAssert( pRTBindingInfo.resolveFBO );

				GLint drawFramebufferHandle = -1;
				glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebufferHandle );
				ic3OpenGLHandleLastError();

				GLint readFramebufferHandle = -1;
				glGetIntegerv( GL_READ_FRAMEBUFFER_BINDING, &readFramebufferHandle );
				ic3OpenGLHandleLastError();

				const auto & fboImageSize = pRTBindingInfo.rtLayout->sharedImageRect;

				glBindFramebuffer( GL_READ_FRAMEBUFFER, drawFramebufferHandle );
				ic3OpenGLHandleLastError();

				glBindFramebuffer( GL_DRAW_FRAMEBUFFER, pRTBindingInfo.renderFBO->mGLHandle );
				ic3OpenGLHandleLastError();

				auto colorAttachmentsResolveMask = pRenderPassConfiguration.attachmentsActionResolveMask;
				for( uint32 caIndex = 0; CxDefs::isRTColorAttachmentIndexValid( caIndex ); ++caIndex )
				{
					const auto attachmentBit = CxDefs::makeRTAttachmentFlag( caIndex );
					if( colorAttachmentsResolveMask.isSet( attachmentBit ) )
					{
						glReadBuffer( GL_COLOR_ATTACHMENT0 + caIndex );
						ic3OpenGLHandleLastError();

						glDrawBuffer( GL_COLOR_ATTACHMENT0 + caIndex );
						ic3OpenGLHandleLastError();

						glBlitFramebuffer(
								0,
								0,
								fboImageSize.width,
								fboImageSize.height,
								0,
								0,
								fboImageSize.width,
								fboImageSize.height,
								GL_COLOR_BUFFER_BIT,
								GL_LINEAR );
						ic3OpenGLHandleLastError();

						colorAttachmentsResolveMask.unset( attachmentBit );

						if( colorAttachmentsResolveMask.empty() )
						{
							break;
						}
					}

					glBindFramebuffer( GL_READ_FRAMEBUFFER, readFramebufferHandle );
					ic3OpenGLHandleLastError();

					glBindFramebuffer( GL_DRAW_FRAMEBUFFER, drawFramebufferHandle );
					ic3OpenGLHandleLastError();
				}
			}
		}

	}

} // namespace Ic3::Graphics::GCI
