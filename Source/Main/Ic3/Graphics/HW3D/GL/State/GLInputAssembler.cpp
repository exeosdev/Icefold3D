
#include "GLInputAssembler.h"
#include <Ic3/Graphics/HW3D/GL/GLAPITranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLVertexArrayObject.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLGPUBuffer.h>

namespace Ic3::Graphics::GCI
{

	void GLIAInputLayoutDefinition::reset()
	{
		activeAttributesMask = 0;
		primitiveTopology = 0;
		memZero( attributeArray );
	}


	void GLIAVertexBuffersBindings::initializeInterleaved()
	{
		bindingType = EGLVertexBufferBindingType::Interleaved;
		interleavedBindings = {};
	}

	void GLIAVertexBuffersBindings::initializeSeparate()
	{
		bindingType = EGLVertexBufferBindingType::Separate;
		separateBindings = {};
	}

	void GLIAVertexBuffersBindings::reset()
	{
		if( bindingType == EGLVertexBufferBindingType::Interleaved )
		{
			memZero( interleavedBindings );
		}
		else if( bindingType == EGLVertexBufferBindingType::Separate )
		{
			memZero( separateBindings );
		}

		bindingType = EGLVertexBufferBindingType::Undefined;
	}

	GLIAVertexBufferBinding GLIAVertexBuffersBindings::getBinding( native_uint pStreamIndex ) const
	{
		if( bindingType == EGLVertexBufferBindingType::Interleaved )
		{
			return interleavedBindings[pStreamIndex];
		}
		else if( bindingType == EGLVertexBufferBindingType::Separate )
		{
			return {
				separateBindings.handleArray[pStreamIndex],
				separateBindings.offsetArray[pStreamIndex],
				separateBindings.strideArray[pStreamIndex]
			};
		}
		else
		{
			return { 0, 0, 0 };
		}
	}


	void GLIAVertexStreamDefinition::reset()
	{
		activeBindingsMask = 0;
		indexBufferBinding.reset();
		vertexBufferBindings.reset();
	}


	GLIAVertexStreamImmutableState::GLIAVertexStreamImmutableState(
			GLGPUDevice & pGPUDevice,
			const IAVertexStreamStateCommonProperties & pCommonProperties,
			const GLIAVertexStreamDefinition & pGLVertexStreamDefinition )
	: IAVertexStreamImmutableState( pGPUDevice, pCommonProperties )
	, mGLVertexStreamDefinition( pGLVertexStreamDefinition )
	{}

	GLIAVertexStreamImmutableState::~GLIAVertexStreamImmutableState() = default;

	GpaHandle<GLIAVertexStreamImmutableState> GLIAVertexStreamImmutableState::createInstance(
			GLGPUDevice & pGPUDevice,
			const IAVertexStreamDefinition & pVertexStreamDefinition )
	{
		const auto vertexStreamCommonProperties = smutil::getIAVertexStreamStateCommonProperties( pVertexStreamDefinition );
		const auto glcVertexStreamDefinition = smutil::translateIAVertexStreamDefinitionGL( pVertexStreamDefinition );

		auto immutableState = createGPUAPIObject<GLIAVertexStreamImmutableState>(
			pGPUDevice,
			vertexStreamCommonProperties,
			glcVertexStreamDefinition );

		return immutableState;
	}


	GLIAInputLayoutImmutableState::GLIAInputLayoutImmutableState(
			GLGPUDevice & pGPUDevice,
			const IAInputLayoutStateCommonProperties & pCommonProperties )
	: IAInputLayoutImmutableState( pGPUDevice, pCommonProperties )
	{}

	GLIAInputLayoutImmutableState::~GLIAInputLayoutImmutableState() = default;


	GLIAInputLayoutImmutableStateCore::GLIAInputLayoutImmutableStateCore(
			GLGPUDevice & pGPUDevice,
			const IAInputLayoutStateCommonProperties & pCommonProperties,
			GLVertexArrayObjectHandle pVertexArrayObject,
			GLenum pGLPrimitiveTopology )
	: GLIAInputLayoutImmutableState( pGPUDevice, pCommonProperties )
	, mVertexArrayObject( std::move( pVertexArrayObject ) )
	, mGLPrimitiveTopology( pGLPrimitiveTopology )
	{}

	GLIAInputLayoutImmutableStateCore::~GLIAInputLayoutImmutableStateCore() = default;

	GpaHandle<GLIAInputLayoutImmutableStateCore> GLIAInputLayoutImmutableStateCore::createInstance(
			GLGPUDevice & pGPUDevice,
			const IAInputLayoutDefinition & pInputLayoutDefinition )
	{
		const auto inputLayoutCommonProperties = smutil::getIAInputLayoutStateCommonProperties( pInputLayoutDefinition );
		const auto glcInputLayoutDefinition = smutil::translateIAInputLayoutDefinitionGL( pInputLayoutDefinition );

		auto vertexArrayObject = smutil::createGLVertexArrayObjectLayoutOnlyGL( glcInputLayoutDefinition );
		if( !vertexArrayObject )
		{
			return nullptr;
		}

		auto immutableState = createGPUAPIObject<GLIAInputLayoutImmutableStateCore>(
			pGPUDevice,
			inputLayoutCommonProperties,
			std::move( vertexArrayObject ),
			glcInputLayoutDefinition.primitiveTopology);

		return immutableState;
	}


	GLIAInputLayoutImmutableStateCompat::GLIAInputLayoutImmutableStateCompat(
			GLGPUDevice & pGPUDevice,
			const IAInputLayoutStateCommonProperties & pCommonProperties,
			const GLIAInputLayoutDefinition & pGLInputLayoutDefinition )
	: GLIAInputLayoutImmutableState( pGPUDevice, pCommonProperties )
	, mGLInputLayoutDefinition( pGLInputLayoutDefinition )
	{}

	GLIAInputLayoutImmutableStateCompat::~GLIAInputLayoutImmutableStateCompat() = default;

	GpaHandle<GLIAInputLayoutImmutableStateCompat> GLIAInputLayoutImmutableStateCompat::createInstance(
			GLGPUDevice & pGPUDevice,
			const IAInputLayoutDefinition & pInputLayoutDefinition )
	{
		const auto inputLayoutCommonProperties = smutil::getIAInputLayoutStateCommonProperties( pInputLayoutDefinition );
		const auto glcInputLayoutDefinition = smutil::translateIAInputLayoutDefinitionGL( pInputLayoutDefinition );

		auto immutableState = createGPUAPIObject<GLIAInputLayoutImmutableStateCompat>(
			pGPUDevice,
			inputLayoutCommonProperties,
			glcInputLayoutDefinition );

		return immutableState;
	}


	namespace smutil
	{

		GLIAVertexAttributeInfo translateIAVertexAttributeInfoGL(
				const IAVertexAttributeInfo & pAttributeInfo )
		{
			GLIAVertexAttributeInfo glcAttributeInfo{};

			glcAttributeInfo.streamIndex = static_cast<GLuint>( pAttributeInfo.streamIndex );
			glcAttributeInfo.instanceRate = pAttributeInfo.instanceRate;
			glcAttributeInfo.relativeOffset = static_cast<uint32>( pAttributeInfo.relativeOffset );
			glcAttributeInfo.byteSize = CxDefs::getVertexAttribFormatByteSize( pAttributeInfo.format );

			const auto attributeBaseType = CxDefs::getVertexAttribFormatBaseDataType( pAttributeInfo.format );
			glcAttributeInfo.baseType = ATL::translateGLBaseDataType( attributeBaseType );

			const auto attributeComponentsNum = CxDefs::getVertexAttribFormatComponentsNum( pAttributeInfo.format );
			glcAttributeInfo.componentsNum = static_cast<uint32>( attributeComponentsNum );

			const auto attributeFormatFlags = CxDefs::getVertexAttribFormatFlags( pAttributeInfo.format );
			glcAttributeInfo.normalized = attributeFormatFlags.isSet( E_GPU_DATA_FORMAT_FLAG_NORMALIZED_BIT ) ? GL_TRUE : GL_FALSE;

			return glcAttributeInfo;
		}

		GLIAInputLayoutDefinition translateIAInputLayoutDefinitionGL(
				const IAInputLayoutDefinition & pDefinition )
		{
			GLIAInputLayoutDefinition glcInputLayoutDefinition{};

			const auto definedVertexAttributesNum = popCount( pDefinition.activeAttributesMask );

			uint32 currentVertexAttributesNum = 0;
			uint64 currentAttributePackedRelativeOffset = 0;

			for( uint32 attributeIndex = 0; attributeIndex < gpm::IA_MAX_VERTEX_ATTRIBUTES_NUM; ++attributeIndex )
			{
				const auto attributeBit = CxDefs::makeIAVertexAttributeFlag( attributeIndex );
				if( pDefinition.activeAttributesMask.isSet( attributeBit ) )
				{
					const auto & inputAttributeInfo = pDefinition.attributeArray[attributeIndex];
					auto & glcAttributeInfo = glcInputLayoutDefinition.attributeArray[attributeIndex];

					// Translate the attribute data. This includes the relative offset.
					glcAttributeInfo = translateIAVertexAttributeInfoGL( inputAttributeInfo );

					if( inputAttributeInfo.relativeOffset == CxDefs::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND )
					{
						// If the offset is APPEND, update it with the current packed offset calculated.
						glcAttributeInfo.relativeOffset = numeric_cast<uint32>( currentAttributePackedRelativeOffset );
					}
					else if( inputAttributeInfo.relativeOffset == CxDefs::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 )
					{
						// If the offset is APPEND, update it with the current packed offset calculated.
						glcAttributeInfo.relativeOffset = numeric_cast<uint32>( memGetAlignedValue( currentAttributePackedRelativeOffset, 16 ) );
					}

					// Update the current packed offset.
					currentAttributePackedRelativeOffset = glcAttributeInfo.relativeOffset + glcAttributeInfo.byteSize;

					++currentVertexAttributesNum;

					if( currentVertexAttributesNum == definedVertexAttributesNum )
					{
						break;
					}
				}
			}

			glcInputLayoutDefinition.activeAttributesMask = pDefinition.activeAttributesMask;
			glcInputLayoutDefinition.primitiveTopology = ATL::translateGLPrimitiveTopology( pDefinition.primitiveTopology );

			return glcInputLayoutDefinition;
		}

		GLIAVertexStreamDefinition translateIAVertexStreamDefinitionGL(
				const IAVertexStreamDefinition & pDefinition )
		{
			GLIAVertexStreamDefinition glcVertexStreamDefinition{};

			if( pDefinition.activeBindingsMask.isSetAnyOf( E_IA_VERTEX_STREAM_BINDING_MASK_VERTEX_BUFFER_ALL_BITS ) )
			{
				translateVertexBufferReferencesGL(
					pDefinition.vertexBufferReferences,
					pDefinition.activeBindingsMask,
					glcVertexStreamDefinition.vertexBufferBindings );
			}

			if( pDefinition.activeBindingsMask.isSet( E_IA_VERTEX_STREAM_BINDING_FLAG_INDEX_BUFFER_BIT ) )
			{
				translateIndexBufferReferenceGL(
					pDefinition.indexBufferReference,
					glcVertexStreamDefinition.indexBufferBinding );
			}

			glcVertexStreamDefinition.activeBindingsMask = ( pDefinition.activeBindingsMask & E_IA_VERTEX_STREAM_BINDING_MASK_ALL );
			glcVertexStreamDefinition.vertexBufferBindings.activeRanges = smutil::generateActiveVertexBufferRanges( pDefinition.vertexBufferReferences );

			return glcVertexStreamDefinition;
		}

		uint32 translateVertexBufferReferencesGL(
				const IAVertexBufferReferenceArray & pVBReferences,
				Bitmask<EIAVertexStreamBindingFlags> pBindingMask,
				GLIAVertexBuffersBindings & pOutGLBindings)
		{
			uint32 activeBindingsNum = 0;

		#if( ICFGX_GL_PLATFORM_TYPE == ICFGX_GL_PLATFORM_TYPE_ES )
			pOutGLBindings.initializeInterleaved();
		#else
			pOutGLBindings.initializeSeparate();
		#endif

			for( native_uint streamIndex = 0; streamIndex < gpm::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM; ++streamIndex )
			{
				const auto & inputVertexBufferRef = pVBReferences[streamIndex];
				const auto vbBindingFlag = CxDefs::makeIAVertexBufferFlag( streamIndex );

				if( inputVertexBufferRef && pBindingMask.isSet( vbBindingFlag ) )
				{
					const auto * glcVertexBuffer = inputVertexBufferRef.sourceBuffer->queryInterface<GLGPUBuffer>();

				#if( ICFGX_GL_PLATFORM_TYPE == ICFGX_GL_PLATFORM_TYPE_ES )
					pOutGLBindings.interleavedBindings[streamIndex].handle = glcVertexBuffer->mGLBufferObject->mGLHandle;
					pOutGLBindings.interleavedBindings[streamIndex].offset = numeric_cast<GLintptr>( inputVertexBufferRef.dataOffset() );
					pOutGLBindings.interleavedBindings[streamIndex].stride = numeric_cast<GLsizei>( inputVertexBufferRef.vertexStride );
				#else
					pOutGLBindings.separateBindings.handleArray[streamIndex] = glcVertexBuffer->mGLBufferObject->mGLHandle;
					pOutGLBindings.separateBindings.offsetArray[streamIndex] = numeric_cast<GLintptr>( inputVertexBufferRef.dataOffset() );
					pOutGLBindings.separateBindings.strideArray[streamIndex] = numeric_cast<GLsizei>( inputVertexBufferRef.vertexStride );
				#endif

					++activeBindingsNum;
				}
				else
				{
				#if( ICFGX_GL_PLATFORM_TYPE == ICFGX_GL_PLATFORM_TYPE_ES )
					pOutGLBindings.interleavedBindings[streamIndex].handle = 0u;
					pOutGLBindings.interleavedBindings[streamIndex].offset = 0u;
					pOutGLBindings.interleavedBindings[streamIndex].stride = 0u;
				#else
					pOutGLBindings.separateBindings.handleArray[streamIndex] = 0u;
					pOutGLBindings.separateBindings.offsetArray[streamIndex] = 0u;
					pOutGLBindings.separateBindings.strideArray[streamIndex] = 0u;
				#endif
				}
			}

			return activeBindingsNum;
		}

		bool translateIndexBufferReferenceGL(
				const IAIndexBufferReference & pIBReference,
				GLIAIndexBufferBinding & pOutGLBinding )
		{
			if( pIBReference )
			{
				const auto * glcIndexBuffer = pIBReference.sourceBuffer->queryInterface<GLGPUBuffer>();

				pOutGLBinding.handle = glcIndexBuffer->mGLBufferObject->mGLHandle;
				pOutGLBinding.offset = pIBReference.dataOffset();
				pOutGLBinding.format = ATL::translateGLIndexDataFormat( pIBReference.indexFormat );
				pOutGLBinding.elementByteSize = CxDefs::getIndexDataFormatByteSize( pIBReference.indexFormat );

				return true;
			}
			else
			{
				pOutGLBinding.handle = 0;
				pOutGLBinding.offset = 0;
				pOutGLBinding.format = 0;
				pOutGLBinding.elementByteSize = 0;

				return false;
			}
		}

		GLIAVertexBuffersBindings translateVertexBufferReferencesGL(
				const IAVertexBufferReferenceArray & pVBReferences,
				Bitmask<EIAVertexStreamBindingFlags> pBindingMask )
		{
			GLIAVertexBuffersBindings glcVertexBufferBindings{};
			translateVertexBufferReferencesGL( pVBReferences, pBindingMask, glcVertexBufferBindings );
			return glcVertexBufferBindings;
		}

		GLIAIndexBufferBinding translateIndexBufferReferenceGL(
				const IAIndexBufferReference & pIBReference )
		{
			GLIAIndexBufferBinding glcIndexBufferBinding{};
			translateIndexBufferReferenceGL( pIBReference, glcIndexBufferBinding );
			return glcIndexBufferBinding;
		}

		GLVertexArrayObjectHandle createGLVertexArrayObjectLayoutOnlyGL(
				const GLIAInputLayoutDefinition & pInputLayoutDefinition ) noexcept
		{
			if( !pInputLayoutDefinition.activeAttributesMask.isSetAnyOf( E_IA_VERTEX_ATTRIBUTE_MASK_ALL ) )
			{
				return nullptr;
			}

			auto vertexArrayObject = GLVertexArrayObject::create();
			updateGLVertexArrayObjectLayoutOnlyGL( *vertexArrayObject, pInputLayoutDefinition );

			return vertexArrayObject;
		}

		bool updateGLVertexArrayObjectLayoutOnlyGL(
				GLVertexArrayObject & pVertexArrayObject,
				const GLIAInputLayoutDefinition & pInputLayoutDefinition ) noexcept
		{
			if( !pInputLayoutDefinition.activeAttributesMask.isSetAnyOf( E_IA_VERTEX_ATTRIBUTE_MASK_ALL ) )
			{
				return false;
			}

			glBindVertexArray( pVertexArrayObject.mGLHandle );
			ic3OpenGLHandleLastError();

			for( uint32 attributeIndex = 0; attributeIndex < gpm::IA_MAX_VERTEX_ATTRIBUTES_NUM; ++attributeIndex )
			{
				const auto attributeBit = CxDefs::makeIAVertexAttributeFlag( attributeIndex );
				if( pInputLayoutDefinition.activeAttributesMask.isSet( attributeBit ) )
				{
					const auto & glcAttribute = pInputLayoutDefinition.attributeArray[attributeIndex];

					glEnableVertexAttribArray( attributeIndex );
					ic3OpenGLHandleLastError();

					glVertexAttribFormat(
						attributeIndex,
						glcAttribute.componentsNum,
						glcAttribute.baseType,
						glcAttribute.normalized,
						glcAttribute.relativeOffset );
					ic3OpenGLHandleLastError();

					// NOTE: glVertexAttribDivisor modifies the binding between attribute index and its vertex stream slot.
					// Internally, it does the equivalent of:
					// 1. glVertexBindingDivisor( attributeIndex, instanceRate );
					// 2. glVertexAttribBinding( attributeIndex, attributeIndex );
					// glVertexAttribDivisor( attributeIndex, glcVertexAttribute.instanceRate );
					// For this reason, we use glVertexBindingDivisor() instead.

					glVertexBindingDivisor( attributeIndex, glcAttribute.instanceRate );
					ic3OpenGLHandleLastError();

					// This call has to be executed after any call that implicitly modifies vertex attribute binding.
					// One of the example is glVertexAttribDivisor() above (the actual reason of the old reported crash).
					glVertexAttribBinding( attributeIndex, glcAttribute.streamIndex );
					ic3OpenGLHandleLastError();
				}
			}

			glBindVertexArray( 0 );
			ic3OpenGLHandleLastError();

			return true;
		}

		GLVertexArrayObjectHandle createGLVertexArrayObjectLayoutStreamCombinedGL(
				const GLIAInputLayoutDefinition & pInputLayoutDefinition,
				const GLIAVertexStreamDefinition & pVertexStreamDefinition ) noexcept
		{
			if( !pInputLayoutDefinition.activeAttributesMask.isSetAnyOf( E_IA_VERTEX_ATTRIBUTE_MASK_ALL ) )
			{
				return nullptr;
			}

			auto vertexArrayObject = GLVertexArrayObject::create();
			updateGLVertexArrayObjectLayoutStreamCombinedGL( *vertexArrayObject, pInputLayoutDefinition, pVertexStreamDefinition );

			return vertexArrayObject;
		}

		bool updateGLVertexArrayObjectLayoutStreamCombinedGL(
				GLVertexArrayObject & pVertexArrayObject,
				const GLIAInputLayoutDefinition & pInputLayoutDefinition,
				const GLIAVertexStreamDefinition & pVertexStreamDefinition ) noexcept
		{
			if( !pInputLayoutDefinition.activeAttributesMask.isSetAnyOf( E_IA_VERTEX_ATTRIBUTE_MASK_ALL ) )
			{
				return false;
			}

			glBindVertexArray( pVertexArrayObject.mGLHandle );
			ic3OpenGLHandleLastError();

			GLuint currentVBBinding = 0;

			for( uint32 attributeIndex = 0; attributeIndex < gpm::IA_MAX_VERTEX_ATTRIBUTES_NUM; ++attributeIndex )
			{
				const auto attributeBit = CxDefs::makeIAVertexAttributeFlag( attributeIndex );
				if( pInputLayoutDefinition.activeAttributesMask.isSet( attributeBit ) )
				{
					const auto & glcAttribute = pInputLayoutDefinition.attributeArray[attributeIndex];
					const auto & glcVertexBufferBinding = pVertexStreamDefinition.vertexBufferBindings.getBinding( glcAttribute.streamIndex );

					ic3DebugAssert( glcVertexBufferBinding );

					if( currentVBBinding != glcVertexBufferBinding.handle )
					{
						glBindBuffer( GL_ARRAY_BUFFER, glcVertexBufferBinding.handle );
						ic3OpenGLHandleLastError();

						currentVBBinding = glcVertexBufferBinding.handle;
					}

					glEnableVertexAttribArray( attributeIndex );
					ic3OpenGLHandleLastError();

					glVertexAttribDivisor( attributeIndex, glcAttribute.instanceRate );
					ic3OpenGLHandleLastError();

					glVertexAttribPointer(
						attributeIndex,
						glcAttribute.componentsNum,
						glcAttribute.baseType,
						glcAttribute.normalized,
						glcVertexBufferBinding.stride,
						reinterpret_cast<const void *>( glcVertexBufferBinding.offset + glcAttribute.relativeOffset ) );
					ic3OpenGLHandleLastError();
				}
			}

			glBindBuffer( GL_ARRAY_BUFFER, 0u );
			ic3OpenGLHandleLastError();

			glBindVertexArray( 0 );
			ic3OpenGLHandleLastError();

			return true;
		}

	}

}
