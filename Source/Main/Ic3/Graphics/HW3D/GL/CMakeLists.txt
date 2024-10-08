
set( IC3_GRAPHICS_HWAPI_GL_SRC_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/Prerequisites/GLCommonDefs.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Prerequisites/GLCommonDefs.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Prerequisites/GLDebugOutput.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Prerequisites/GLDebugOutput.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Prerequisites/GLObject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Prerequisites/GLObject.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/GLPrerequisites.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLPrerequisites.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/GLCommandList.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLCommandList.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLCommandSystem.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLCommandSystem.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLApiTranslationLayer.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLApiTranslationLayer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLGPUDevice.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLGPUDevice.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLGPUDriver.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLGPUDriver.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLPresentationLayer.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/GLPresentationLayer.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLBufferObject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLBufferObject.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLFramebufferObject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLFramebufferObject.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLObjectAllocator.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLObjectAllocator.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLRenderbufferObject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLRenderbufferObject.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLSamplerObject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLSamplerObject.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLShaderCommon.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLShaderCommon.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLShaderObject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLShaderObject.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLShaderPipelineObject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLShaderPipelineObject.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLShaderProgramObject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLShaderProgramObject.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLTextureObject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLTextureObject.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLVertexArrayObject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Objects/GLVertexArrayObject.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/GLGPUBuffer.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/GLGPUBuffer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/GLRenderBuffer.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/GLRenderBuffer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/GLSampler.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/GLSampler.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/GLShader.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/GLShader.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/GLTexture.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/GLTexture.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLCommonGraphicsConfig.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLCommonGraphicsConfig.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLCommonGraphicsConfig.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLCommonGraphicsConfig.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLGlobalStateCache.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLGlobalStateCache.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLGraphicsShaderState.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLGraphicsShaderState.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLInputAssembler.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLInputAssembler.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLPipelineStateController.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLPipelineStateController.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLPipelineImmutableStateFactory.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLPipelineImmutableStateFactory.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLPipelineStateObject.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLPipelineStateObject.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLRenderTarget.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLRenderTarget.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLVertexArrayObjectCache.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/State/GLVertexArrayObjectCache.cpp"

        PARENT_SCOPE
)
