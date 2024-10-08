
#include "GLFramebufferObject.h"
#include "GLRenderbufferObject.h"
#include "GLTextureObject.h"
#include "../State/GLCommonStateDefs.h"

namespace Ic3::Graphics::GCI
{

	GLFramebufferObject::GLFramebufferObject( GLuint pHandle )
	: GLObject( GLObjectBaseType::Framebuffer, pHandle )
	{}

	GLFramebufferObject::~GLFramebufferObject() = default;

	GLFramebufferObjectHandle GLFramebufferObject::create()
	{
		GLuint framebufferHandle = 0;

		glGenFramebuffers( 1, &framebufferHandle );
		ic3OpenGLHandleLastError();

		GLFramebufferObjectHandle openglFramebufferObject{ new GLFramebufferObject( framebufferHandle ) };

		return openglFramebufferObject;
	}

	GLFramebufferObjectHandle GLFramebufferObject::createForDefaultFramebuffer()
	{
		return GLFramebufferObjectHandle{ new GLFramebufferObject( 0u ) };
	}

	bool GLFramebufferObject::release()
	{
		if( mGLHandle != 0 )
		{
			glDeleteFramebuffers( 1, &mGLHandle );
			ic3OpenGLHandleLastError();
		}

		return true;
	}

	bool GLFramebufferObject::validateHandle() const
	{
		auto isBuffer = glIsFramebuffer( mGLHandle );
		ic3OpenGLHandleLastError();

		return isBuffer != GL_FALSE;
	}

	void GLFramebufferObject::bindColorRenderbuffer(
			uint32 pColorAttachmentIndex,
			GLRenderbufferObject & pGLRenderbuffer,
			GLenum pActiveBindTarget )
	{
		const auto framebufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		glFramebufferRenderbuffer(
				framebufferBindTarget,
				GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
				GL_RENDERBUFFER,
				pGLRenderbuffer.mGLHandle );
		ic3OpenGLHandleLastError();
	}

	void GLFramebufferObject::bindDepthStencilRenderbuffer(
			GLRenderbufferObject & pGLRenderbuffer,
			Bitmask<uint32> pBufferMask,
			GLenum pActiveBindTarget )
	{
		const auto framebufferBindTarget = checkActiveBindTarget( pActiveBindTarget );
		const auto fboAttachmentID = smutil::getFramebufferDepthStencilAttachmentIDForRTBufferMask( pBufferMask );

		glFramebufferRenderbuffer(
				framebufferBindTarget,
				fboAttachmentID,
				GL_RENDERBUFFER,
				pGLRenderbuffer.mGLHandle );
		ic3OpenGLHandleLastError();
	}

	void GLFramebufferObject::bindColorTexture(
			uint32 pColorAttachmentIndex,
			GLTextureObject & pGLTexture,
			const TextureSubResource & pSubResource,
			GLenum pActiveBindTarget )
	{
		const auto framebufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		switch( pGLTexture.mGLTextureBindTarget )
		{
			case GL_TEXTURE_2D:
			{
				glFramebufferTexture2D(
						framebufferBindTarget,
						GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
						GL_TEXTURE_2D,
						pGLTexture.mGLHandle,
						numeric_cast<GLint>( pSubResource.uSubRes2D.mipLevel ) );
				ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_2D_ARRAY:
			{
				glFramebufferTextureLayer(
						framebufferBindTarget,
						GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
						pGLTexture.mGLHandle,
						numeric_cast<GLint>( pSubResource.uSubRes2D.mipLevel ),
						numeric_cast<GLint>( pSubResource.uSubRes2DArray.arrayIndex ) );
				ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_2D_MULTISAMPLE:
			{
				glFramebufferTexture2D(
						framebufferBindTarget,
						GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
						GL_TEXTURE_2D_MULTISAMPLE,
						pGLTexture.mGLHandle,
						numeric_cast<GLint>( pSubResource.uSubRes2D.mipLevel ) );
				ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_3D:
			{
				glFramebufferTextureLayer(
						framebufferBindTarget,
						GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
						pGLTexture.mGLHandle,
						numeric_cast<GLint>( pSubResource.uSubRes3D.mipLevel ),
						numeric_cast<GLint>( pSubResource.uSubRes3D.depthLayerIndex ) );
				ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_CUBE_MAP:
			{
				glFramebufferTexture2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + pSubResource.uSubResCubeMap.faceIndex,
						GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
						GL_TEXTURE_2D,
						pGLTexture.mGLHandle,
						numeric_cast<GLint>( pSubResource.uSubResCubeMap.mipLevel ) );
				ic3OpenGLHandleLastError();
				break;
			}
		}
	}

	void GLFramebufferObject::bindDepthStencilTexture(
			GLTextureObject & pGLTexture,
			const TextureSubResource & pSubResource,
			Bitmask<uint32> pBufferMask,
			GLenum pActiveBindTarget )
	{
		const auto framebufferBindTarget = checkActiveBindTarget( pActiveBindTarget );
		const auto fboAttachmentID = smutil::getFramebufferDepthStencilAttachmentIDForRTBufferMask( pBufferMask );

		switch( pGLTexture.mGLTextureBindTarget )
		{
			case GL_TEXTURE_2D:
			{
				glFramebufferTexture2D(
						framebufferBindTarget,
						fboAttachmentID,
						GL_TEXTURE_2D,
						pGLTexture.mGLHandle,
						numeric_cast<GLint>( pSubResource.uSubRes2D.mipLevel ) );
				ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_2D_ARRAY:
			{
				glFramebufferTextureLayer(
						framebufferBindTarget,
						fboAttachmentID,
						pGLTexture.mGLHandle,
						numeric_cast<GLint>( pSubResource.uSubRes2DArray.mipLevel ),
						numeric_cast<GLint>( pSubResource.uSubRes2DArray.arrayIndex ) );
				ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_2D_MULTISAMPLE:
			{
				glFramebufferTexture2D(
						framebufferBindTarget,
						fboAttachmentID,
						GL_TEXTURE_2D_MULTISAMPLE,
						pGLTexture.mGLHandle,
						numeric_cast<GLint>( pSubResource.uSubRes2D.mipLevel ) );
				ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_3D:
			{
				glFramebufferTextureLayer(
						framebufferBindTarget,
						fboAttachmentID,
						pGLTexture.mGLHandle,
						numeric_cast<GLint>( pSubResource.uSubRes3D.mipLevel ),
						numeric_cast<GLint>( pSubResource.uSubRes3D.depthLayerIndex ) );
				ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_CUBE_MAP:
			{
				glFramebufferTexture2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + pSubResource.uSubResCubeMap.faceIndex,
						fboAttachmentID,
						GL_TEXTURE_2D,
						pGLTexture.mGLHandle,
						numeric_cast<GLint>( pSubResource.uSubResCubeMap.mipLevel ) );
				ic3OpenGLHandleLastError();
				break;
			}
		}
	}

	bool GLFramebufferObject::checkStatus( GLenum pActiveBindTarget ) const
	{
		auto framebufferBindTarget = checkActiveBindTarget( pActiveBindTarget );

		GLenum framebufferStatus = glCheckFramebufferStatus( framebufferBindTarget );
		ic3OpenGLHandleLastError();

		if( framebufferStatus != GL_FRAMEBUFFER_COMPLETE )
		{
			switch( framebufferStatus )
			{
				case GL_FRAMEBUFFER_UNDEFINED:
					ic3DebugInterrupt();
					break;

				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					ic3DebugInterrupt();
					break;

				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					ic3DebugInterrupt();
					break;

				case GL_FRAMEBUFFER_UNSUPPORTED:
					ic3DebugInterrupt();
					break;

				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
					ic3DebugInterrupt();
					break;
			}

			return false;
		}

		return true;
	}

	GLenum GLFramebufferObject::checkActiveBindTarget( GLenum pBindTarget ) const
	{
		if( pBindTarget == 0 )
		{
			pBindTarget = GL_FRAMEBUFFER;

			glBindFramebuffer( GL_FRAMEBUFFER, mGLHandle );
			ic3OpenGLHandleLastError();
		}

		return pBindTarget;
	}

} // namespace Ic3::Graphics::GCI
