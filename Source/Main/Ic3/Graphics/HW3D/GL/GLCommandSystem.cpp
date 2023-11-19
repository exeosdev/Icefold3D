
#include "GLCommandSystem.h"
#include "GLCommandList.h"
#include "GLGPUDevice.h"
#include <Ic3/Graphics/GCI/CommandContext.h>
#include <mutex>

namespace Ic3::Graphics::GCI
{

	static void initializeOpenGLAPI()
	{
		static std::atomic_flag sInitFlag = { ATOMIC_FLAG_INIT };

		if( !sInitFlag.test_and_set( std::memory_order_acq_rel ) )
		{
			::glewInit();
		}
	}

	GLCommandSystem::GLCommandSystem( GLGPUDevice & pGPUDevice )
	: CommandSystem( pGPUDevice )
	, _targetSysGLSurface( nullptr )
	{}

	GLCommandSystem::~GLCommandSystem() = default;

	std::unique_ptr<CommandContext> GLCommandSystem::acquireCommandContext( ECommandContextType pContextType )
	{
		std::unique_ptr<CommandContext> commandContext;

		auto contextExecutionMode = CxDef::getCommandObjectExecutionMode( pContextType );
		if( auto * commandList = acquireCommandList( contextExecutionMode ) )
		{
			ic3DebugAssert( contextExecutionMode == ECommandExecutionMode::Direct );
			commandContext = std::make_unique<CommandContextDirectGraphics>( *commandList );
			commandList->mSysGLRenderContext->bindForCurrentThread( *_targetSysGLSurface );

			// A dirty workaround. GLEW is no longer used at the system level (now we use only
			// the minimal set of GLX/WGL functions), so it needs to happen here. This is best
			// to be replaced with a custom API loader/manager.
			initializeOpenGLAPI();
		}

		if( commandContext )
		{
			auto * openglGPUDevice = mGPUDevice.queryInterface<GLGPUDevice>();
			auto * openglDebugOutput = openglGPUDevice->getDebugOutputInterface();

			const auto enableDebugOutput = false;

			if( openglDebugOutput && enableDebugOutput )
			{
			    openglDebugOutput->enableDebugOutput( true );
			    openglDebugOutput->enableBreakOnEvent( true );
			    openglDebugOutput->enableSync( true );
			}
		}

		return commandContext;
	}

	CommandSync GLCommandSystem::submitContext( CommandContextDirect & pContext, const CommandContextSubmitInfo & pSubmitInfo )
	{
		CommandSync cmdSyncObject;

		if( pSubmitInfo.syncMode == ECommandSubmitSyncMode::Default )
		{
			auto * openglCommandSyncData = new GLCommandSyncData();
			cmdSyncObject.syncData = openglCommandSyncData;
			cmdSyncObject.syncDataReleaseFunc = releaseGLCommandSyncData;

			auto syncFence = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
			ic3OpenGLCheckLastResult();

			glFlush();
			ic3OpenGLCheckLastResult();

			openglCommandSyncData->openglSyncFence = syncFence;
		}

		return cmdSyncObject;
	}

	void GLCommandSystem::setTargetGLSurface( System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface )
	{
		// Surface is required to create a GL context. Since CommandSystem is bound to a device
		// and PresentationLayer is created when device is already available, there is no way
		// we can know the surface in advance. Hence, we set it manually (eh...) and use it to
		// create a GL context when a new CommandList is instantiated.
		_targetSysGLSurface = pSysGLDisplaySurface;
	}

	GLCommandList * GLCommandSystem::acquireCommandList( ECommandExecutionMode pCommandExecutionMode )
	{
		GLCommandList * commandList = nullptr;

		if( pCommandExecutionMode == ECommandExecutionMode::Direct )
		{
			if( !_mainCommandList )
			{
				initializeMainCommandList();
			}
			if( _mainCommandList && _mainCommandList->acquireList() )
			{
				commandList = _mainCommandList.get();
			}
		}

		return commandList;
	}

	bool GLCommandSystem::initializeMainCommandList()
	{
		ic3DebugAssert( !_mainCommandList );

		auto * openglGPUDevice = mGPUDevice.queryInterface<GLGPUDevice>();
		ic3DebugAssert( openglGPUDevice );

		auto sysGLRenderContext = GLCommandSystem::createSysGLRenderContext( *openglGPUDevice, _targetSysGLSurface );
		ic3DebugAssert( sysGLRenderContext );

		if( openglGPUDevice->isCompatibilityDevice() )
		{
			_mainCommandList = createGPUAPIObject<GLCommandListCompat>( *this, ECommandListType::DirectGraphics, sysGLRenderContext );
		}
		else
		{
			_mainCommandList = createGPUAPIObject<GLCommandListCore>( *this, ECommandListType::DirectGraphics, sysGLRenderContext );
		}

		return true;
	}

	System::OpenGLRenderContextHandle GLCommandSystem::createSysGLRenderContext( GLGPUDevice & pGPUDevice, System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface )
	{
		System::OpenGLRenderContextHandle sysGLRenderContext = nullptr;

		try
		{
			System::OpenGLRenderContextCreateInfo contextCreateInfo;
			contextCreateInfo.shareContext = nullptr;

		#if( ICFGX_GL_TARGET == ICFGX_GL_TARGET_GL43 )
			contextCreateInfo.requestedAPIVersion.major = 4;
			contextCreateInfo.requestedAPIVersion.minor = 3;
			contextCreateInfo.contextAPIProfile = System::EOpenGLAPIProfile::Core;
		#elif( ICFGX_GL_TARGET == ICFGX_GL_TARGET_ES31 )
			contextCreateInfo.runtimeVersionDesc.apiVersion.major = 3;
			contextCreateInfo.runtimeVersionDesc.apiVersion.minor = 1;
			contextCreateInfo.runtimeVersionDesc.apiProfile = System::EGLAPIProfile::OpenGLES;
		#endif

			if( pGPUDevice.isDebugDevice() )
			{
				contextCreateInfo.flags.set( System::E_OPENGL_RENDER_CONTEXT_CREATE_FLAG_ENABLE_DEBUG_BIT );
			}

			sysGLRenderContext = pSysGLDisplaySurface->mGLSystemDriver->createRenderContext( *pSysGLDisplaySurface, contextCreateInfo );
			sysGLRenderContext->bindForCurrentThread( *pSysGLDisplaySurface );

			auto sysVersionInfo = sysGLRenderContext->querySystemVersionInfo();
			auto sysVersionInfoStr = sysVersionInfo.toString();
			ic3DebugOutputFmt( "%s\n", sysVersionInfoStr.c_str() );
		}
		catch ( ... )
		{
			ic3DebugInterrupt();
		}

		return sysGLRenderContext;
	}


	void releaseGLCommandSyncData( void * pSyncData )
	{
		if( pSyncData )
		{
			auto * openglCommandSyncData = static_cast<GLCommandSyncData *>( pSyncData );
			openglCommandSyncData->openglSyncFence = nullptr;
			delete openglCommandSyncData;
		}
	}

} // namespace Ic3::Graphics::GCI
