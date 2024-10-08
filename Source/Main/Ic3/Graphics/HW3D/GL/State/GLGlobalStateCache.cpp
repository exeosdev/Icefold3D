
#include "GLGlobalStateCache.h"

namespace Ic3::Graphics::GCI
{

	const GLGlobalState GLGlobalStateCache::sDefaultState = GLGlobalStateCache::getDefaultGlobalState();

	GLGlobalStateCache::GLGlobalStateCache()
	{
		reset();
	}

	void GLGlobalStateCache::reset()
	{
		_cachedState = sDefaultState;
	}

	void GLGlobalStateCache::applyShaderPipelineBinding( GLuint pShaderPipelineHandle )
	{
		if( pShaderPipelineHandle != _cachedState.shaderPipelineBinding )
		{
			glBindProgramPipeline( pShaderPipelineHandle );
			ic3OpenGLHandleLastError();
			_cachedState.shaderPipelineBinding = pShaderPipelineHandle;
		}
	}

	void GLGlobalStateCache::applyShaderProgramBinding( GLuint pShaderProgramHandle )
	{
		if( pShaderProgramHandle != _cachedState.shaderProgramBinding )
		{
			glUseProgram( pShaderProgramHandle );
			ic3OpenGLHandleLastError();
			_cachedState.shaderProgramBinding = pShaderProgramHandle;
		}
	}

	void GLGlobalStateCache::applyIndexBufferBinding( GLuint pIndexBufferObjectHandle )
	{
		if( pIndexBufferObjectHandle != _cachedState.indexBufferBinding )
		{
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIndexBufferObjectHandle );
			ic3OpenGLHandleLastError();
			_cachedState.indexBufferBinding = pIndexBufferObjectHandle;
		}
	}

	void GLGlobalStateCache::applyVertexArrayObjectBinding( GLuint pVertexArrayObjectHandle )
	{
		if( pVertexArrayObjectHandle != _cachedState.vertexArrayObjectBinding )
		{
			glBindVertexArray( pVertexArrayObjectHandle );
			ic3OpenGLHandleLastError();
			_cachedState.vertexArrayObjectBinding = pVertexArrayObjectHandle;
			_cachedState.indexBufferBinding = 0;
		}
	}

	void GLGlobalStateCache::applyBlendState( const GLBlendConfig & pBlendConfig )
	{
		auto & cachedBlendConfig = _cachedState.blendConfig;

		if( !pBlendConfig.attachmentsMask.isSetAnyOf( E_RT_ATTACHMENT_MASK_COLOR_ALL ) )
		{
			if( cachedBlendConfig.blendActiveGlobal )
			{
				glDisable( GL_BLEND );
				ic3OpenGLHandleLastError();

				cachedBlendConfig.blendActiveGlobal = false;
			}
		}
		else
		{
			const bool isSeparateBlendingEnabled = cachedBlendConfig.flags.isSet( E_BLEND_CONFIG_FLAG_ENABLE_MRT_INDEPENDENT_BLENDING_BIT );
			const bool isSeparateBlendingRequested = pBlendConfig.flags.isSet( E_BLEND_CONFIG_FLAG_ENABLE_MRT_INDEPENDENT_BLENDING_BIT );

			if( isSeparateBlendingRequested )
			{
				bool compareCurrentState = isSeparateBlendingEnabled;

				for( uint32 caIndex = 0; CxDef::isRTColorAttachmentIndexValid( caIndex ); ++caIndex )
				{
					auto & cachedBlendProps = cachedBlendConfig.attachments[caIndex];
					const auto & blendProps = pBlendConfig.attachments[caIndex];

					if( !blendProps.blendActive )
					{
						if( !compareCurrentState || cachedBlendProps.blendActive )
						{
							glDisablei( GL_BLEND, caIndex );
							ic3OpenGLHandleLastError();
							cachedBlendProps.blendActive = 0;
						}
					}
					else // Enable blending for this attachment
					{
						if( !compareCurrentState || !cachedBlendProps.blendActive )
						{
							glEnablei( GL_BLEND, caIndex );
							ic3OpenGLHandleLastError();
							cachedBlendProps.blendActive = 1;
						}

						if( !compareCurrentState || memCompareNotEqual( blendProps.equation, cachedBlendProps.equation ) )
						{
							glBlendEquationSeparatei( caIndex, blendProps.equation.rgb, blendProps.equation.alpha );
							ic3OpenGLHandleLastError();
							cachedBlendProps.equation = blendProps.equation;
						}

						if( !compareCurrentState || memCompareNotEqual( blendProps.factor, cachedBlendProps.factor ) )
						{
							glBlendFuncSeparatei( caIndex, blendProps.factor.rgbSrc, blendProps.factor.rgbDst, blendProps.factor.alphaSrc, blendProps.factor.alphaDst );
							ic3OpenGLHandleLastError();
							cachedBlendProps.factor = blendProps.factor;
						}
					}
				}
			}
			else
			{
				bool compareCurrentState = !isSeparateBlendingEnabled;

				if( !compareCurrentState || !cachedBlendConfig.blendActiveGlobal  )
				{
					glEnable( GL_BLEND );
					ic3OpenGLHandleLastError();
					cachedBlendConfig.blendActiveGlobal = true;
				}

				auto & cachedBlendProps = cachedBlendConfig.attachments[0];
				const auto & blendProps = pBlendConfig.attachments[0];

				if( !compareCurrentState || memCompareNotEqual( blendProps.equation, cachedBlendProps.equation ) )
				{
					glBlendEquationSeparate( blendProps.equation.rgb, blendProps.equation.alpha );
					ic3OpenGLHandleLastError();
					cachedBlendProps.equation = blendProps.equation;
				}

				if( !compareCurrentState || memCompareNotEqual( blendProps.factor, cachedBlendProps.factor ) )
				{
					glBlendFuncSeparate( blendProps.factor.rgbSrc, blendProps.factor.rgbDst, blendProps.factor.alphaSrc, blendProps.factor.alphaDst );
					ic3OpenGLHandleLastError();
					cachedBlendProps.factor = blendProps.factor;
				}
			}

			if( pBlendConfig.flags.isSetAnyOf( E_BLEND_CONFIG_FLAG_SET_FIXED_BLEND_CONSTANTS_BIT ) )
			{
				const auto & blendConstantColor = pBlendConfig.constantColor;

				if( memCompareNotEqual( blendConstantColor, cachedBlendConfig.constantColor ) )
				{
					glBlendColor( blendConstantColor.fpRed, blendConstantColor.fpGreen, blendConstantColor.fpBlue, blendConstantColor.fpAlpha );
					ic3OpenGLHandleLastError();
					cachedBlendConfig.constantColor = pBlendConfig.constantColor;
				}
			}
		}
	}

	void GLGlobalStateCache::applyDepthStencilState( const GLDepthStencilConfig & pDepthStencilConfig, uint8 pStencilRefValue )
	{
		auto & cachedDepthStencilConfig = _cachedState.depthStencilConfig;
		auto & cachedDepthSettings = cachedDepthStencilConfig.depthSettings;
		auto & cachedStencilSettings = cachedDepthStencilConfig.stencilSettings;

		if( !pDepthStencilConfig.depthTestActive )
		{
			if( cachedDepthStencilConfig.depthTestActive )
			{
				glDisable( GL_DEPTH_TEST );
				ic3OpenGLHandleLastError();
				cachedDepthStencilConfig.depthTestActive = false;
			}
		}
		else
		{
			if( !cachedDepthStencilConfig.depthTestActive )
			{
				glEnable( GL_DEPTH_TEST );
				ic3OpenGLHandleLastError();
				cachedDepthStencilConfig.depthTestActive = true;
			}

			if( cachedDepthSettings.depthCompFunc != pDepthStencilConfig.depthSettings.depthCompFunc )
			{
				glDepthFunc( pDepthStencilConfig.depthSettings.depthCompFunc );
				ic3OpenGLHandleLastError();
				cachedDepthSettings.depthCompFunc = pDepthStencilConfig.depthSettings.depthCompFunc;
			}

			if( cachedDepthSettings.writeMask != pDepthStencilConfig.depthSettings.writeMask )
			{
				glDepthMask( pDepthStencilConfig.depthSettings.writeMask ? GL_TRUE : GL_FALSE );
				ic3OpenGLHandleLastError();
				cachedDepthSettings.writeMask = pDepthStencilConfig.depthSettings.writeMask;
			}
		}

		if( !pDepthStencilConfig.stencilTestActive )
		{
			if( cachedDepthStencilConfig.stencilTestActive )
			{
				glDisable( GL_STENCIL_TEST );
				ic3OpenGLHandleLastError();
				cachedDepthStencilConfig.stencilTestActive = false;
			}
		}
		else
		{
			if( !cachedDepthStencilConfig.stencilTestActive )
			{
				glEnable( GL_STENCIL_TEST );
				ic3OpenGLHandleLastError();
				cachedDepthStencilConfig.stencilTestActive = true;
			}

			const auto & frontFace = pDepthStencilConfig.stencilSettings.frontFace;
			if( !memCompareEqual( frontFace, cachedStencilSettings.frontFace ) )
			{
				glStencilFuncSeparate( GL_FRONT, frontFace.compFunc, pStencilRefValue, frontFace.readMask );
				ic3OpenGLHandleLastError();

				glStencilOpSeparate( GL_FRONT, frontFace.opFail, frontFace.opPassDepthFail, frontFace.opPassDepthPass );
				ic3OpenGLHandleLastError();

				glStencilMaskSeparate( GL_FRONT, frontFace.writeMask );
				ic3OpenGLHandleLastError();

				memCopy( cachedStencilSettings.frontFace, frontFace );
			}

			const auto & backFace = pDepthStencilConfig.stencilSettings.backFace;
			if( !memCompareEqual( backFace, cachedStencilSettings.backFace ) )
			{
				glStencilFuncSeparate( GL_FRONT, backFace.compFunc, pStencilRefValue, backFace.readMask );
				ic3OpenGLHandleLastError();

				glStencilOpSeparate( GL_FRONT, backFace.opFail, backFace.opPassDepthFail, backFace.opPassDepthPass );
				ic3OpenGLHandleLastError();

				glStencilMaskSeparate( GL_FRONT, backFace.writeMask );
				ic3OpenGLHandleLastError();

				memCopy( cachedStencilSettings.backFace, backFace );
			}
		}
	}

	void GLGlobalStateCache::applyRasterizerState( const GLRasterizerConfig & pRasterizerConfig )
	{
		auto & cachedRasterizerConfig = _cachedState.rasterizerConfig;

		if( pRasterizerConfig.scissorTestActive != cachedRasterizerConfig.scissorTestActive )
		{
			if( pRasterizerConfig.scissorTestActive )
			{
				glEnable( GL_SCISSOR_TEST );
				ic3OpenGLHandleLastError();
				cachedRasterizerConfig.scissorTestActive = true;
			}
			else
			{
				glDisable( GL_SCISSOR_TEST );
				ic3OpenGLHandleLastError();
				cachedRasterizerConfig.scissorTestActive = false;
			}
		}

		if( pRasterizerConfig.cullMode != cachedRasterizerConfig.cullMode )
		{
			if( pRasterizerConfig.cullMode != GL_NONE )
			{
				glEnable( GL_CULL_FACE );
				ic3OpenGLHandleLastError();

				glCullFace( pRasterizerConfig.cullMode );
				ic3OpenGLHandleLastError();

				cachedRasterizerConfig.cullMode = pRasterizerConfig.cullMode;
			}
			else
			{
				glDisable( GL_CULL_FACE );
				ic3OpenGLHandleLastError();

				cachedRasterizerConfig.cullMode = GL_NONE;
			}
		}

		if( pRasterizerConfig.frontFaceVerticesOrder != cachedRasterizerConfig.frontFaceVerticesOrder )
		{
			glFrontFace( pRasterizerConfig.frontFaceVerticesOrder );
			ic3OpenGLHandleLastError();

			cachedRasterizerConfig.frontFaceVerticesOrder = pRasterizerConfig.frontFaceVerticesOrder;
		}

	#if( ICFGX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
		if( pRasterizerConfig.primitiveFillMode != cachedRasterizerConfig.primitiveFillMode )
		{
			glPolygonMode( GL_FRONT_AND_BACK, pRasterizerConfig.primitiveFillMode );
			ic3OpenGLHandleLastError();

			cachedRasterizerConfig.frontFaceVerticesOrder = pRasterizerConfig.frontFaceVerticesOrder;
		}
	#endif
	}

	GLGlobalState GLGlobalStateCache::getDefaultGlobalState()
	{
		GLGlobalState defaultGlobalState;

		defaultGlobalState.shaderPipelineBinding = 0;
		defaultGlobalState.shaderProgramBinding = 0;
		defaultGlobalState.vertexArrayObjectBinding = 0;

		// BlendConfig
		{
			defaultGlobalState.blendConfig.blendActiveGlobal = false;
			defaultGlobalState.blendConfig.flags = 0;
			defaultGlobalState.blendConfig.attachmentsMask = 0;
			defaultGlobalState.blendConfig.constantColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			defaultGlobalState.blendConfig.attachments[0].blendActive = false;
			defaultGlobalState.blendConfig.attachments[0].equation.rgb = GL_FUNC_ADD;
			defaultGlobalState.blendConfig.attachments[0].equation.alpha = GL_FUNC_ADD;
			defaultGlobalState.blendConfig.attachments[0].factor.rgbSrc = GL_ONE;
			defaultGlobalState.blendConfig.attachments[0].factor.alphaSrc = GL_ONE;
			defaultGlobalState.blendConfig.attachments[0].factor.rgbDst = GL_ZERO;
			defaultGlobalState.blendConfig.attachments[0].factor.alphaDst = GL_ZERO;
			defaultGlobalState.blendConfig.attachments[1] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[2] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[3] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[4] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[5] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[6] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[7] = defaultGlobalState.blendConfig.attachments[0];
		}

		// DepthStencilConfig
		{
			defaultGlobalState.depthStencilConfig.depthTestActive = false;
			defaultGlobalState.depthStencilConfig.depthSettings.depthCompFunc = GL_LESS;
			defaultGlobalState.depthStencilConfig.depthSettings.writeMask = GL_TRUE;

			defaultGlobalState.depthStencilConfig.stencilTestActive = false;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace.compFunc = GL_ALWAYS;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace.writeMask = 0xFFFFFFFF;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace.opFail = GL_KEEP;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace.opPassDepthFail = GL_KEEP;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace.opPassDepthPass = GL_KEEP;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace = defaultGlobalState.depthStencilConfig.stencilSettings.backFace;
		}

		// RasterizerConfig
		{
			defaultGlobalState.rasterizerConfig.scissorTestActive = false;
			defaultGlobalState.rasterizerConfig.cullMode = GL_NONE; // This is initially GL_BACK, but culling itself is disabled!
			defaultGlobalState.rasterizerConfig.frontFaceVerticesOrder = GL_CCW;
		#if( ICFGX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
			defaultGlobalState.rasterizerConfig.primitiveFillMode = GL_FILL;
		#endif
		}

		memZero( defaultGlobalState.samplerBindings );
		memZero( defaultGlobalState.textureUnitBindings );

		return defaultGlobalState;
	}

}
