
#include "X11SysContext.h"
#include "X11AssetSystem.h"
#include "X11DisplaySystem.h"
#include "X11EventCore.h"
#include "X11FileManager.h"
#include "X11OpenGLDriver.h"
#include "X11WindowSystem.h"
#include <Ic3/System/SysContextNative.h>
#include <Ic3/System/AssetSystemNative.h>

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_X11 )
namespace Ic3::System
{

	namespace Platform
	{

		SysContextHandle createSysContext( const SysContextCreateInfo & pCreateInfo )
		{
			return createDynamicInterfaceObject<X11SysContext>();
		}

	}


	X11SysContext::X11SysContext()
	{
		_initializeX11ContextState();
	}

	X11SysContext::~X11SysContext() noexcept
	{
		_releaseX11ContextState();
	}

	AssetLoaderHandle X11SysContext::createAssetLoader( const AssetLoaderCreateInfo & pCreateInfo )
	{
		return Platform::createFileAssetLoader( getHandle<X11SysContext>(), *pCreateInfo.nativeParams );
	}

	DisplayManagerHandle X11SysContext::createDisplayManager()
	{
		return createSysObject<X11DisplayManager>( getHandle<X11SysContext>() );
	}

	EventControllerHandle X11SysContext::createEventController()
	{
		return createSysObject<X11EventController>( getHandle<X11SysContext>() );
	}

	FileManagerHandle X11SysContext::createFileManager()
	{
		return createSysObject<PosixFileManager>( getHandle<X11SysContext>() );
	}

	OpenGLSystemDriverHandle X11SysContext::createOpenGLSystemDriver( DisplayManagerHandle pDisplayManager )
	{
		if( !pDisplayManager )
		{
			pDisplayManager = createDisplayManager();
		}

		return createSysObject<X11OpenGLSystemDriver>( pDisplayManager->getHandle<X11DisplayManager>() );
	}

	WindowManagerHandle X11SysContext::createWindowManager( DisplayManagerHandle pDisplayManager )
	{
		if( !pDisplayManager )
		{
			pDisplayManager = createDisplayManager();
		}

		return createSysObject<X11WindowManager>( pDisplayManager->getHandle<X11DisplayManager>() );
	}

	void X11SysContext::_initializeX11ContextState()
	{
		// This has completely crashed the whole X11 subsystem when ran on a VM-ed ARM-based Ubuntu 21.
		// The environment: MacBook Pro 16.2 with M1 Max, Ubuntu 21.04 (ARM version), Parallels Pro.
		// Most X11 started giving infinite freezes diagnosed as deadlocks on a system-level mutexes.
		//
		// int thrInitStatus = ::XInitThreads();
		// if( thrInitStatus == False )
		// {
		// 	ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		// }

		auto * xDisplay = ::XOpenDisplay( nullptr );
		if( xDisplay == nullptr )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		mNativeData.xSessionData.display = xDisplay;
		mNativeData.xSessionData.screenIndex = XDefaultScreen( mNativeData.xSessionData.display );
		mNativeData.xSessionData.rootWindowXID = XRootWindow( mNativeData.xSessionData.display, mNativeData.xSessionData.screenIndex );
		mNativeData.xSessionData.atomCache.wmProtocol = XInternAtom( mNativeData.xSessionData.display, "WM_PROTOCOLS", True );
		mNativeData.xSessionData.atomCache.wmProtocolDelete = XInternAtom( mNativeData.xSessionData.display, "WM_DELETE_WINDOW", True );
		mNativeData.xSessionData.atomCache.wmProtocolDestroy = XInternAtom( mNativeData.xSessionData.display, "WM_DESTROY_WINDOW", True );
		mNativeData.xSessionData.atomCache.wmState = XInternAtom( mNativeData.xSessionData.display, "_NET_WM_STATE", True );
		mNativeData.xSessionData.atomCache.wmStateFullscreen = XInternAtom( mNativeData.xSessionData.display, "_NET_WM_STATE_FULLSCREEN", True );
		mNativeData.xSessionData.sessionInfo.connectionNumber = XConnectionNumber( xDisplay );
		mNativeData.xSessionData.sessionInfo.vendorName = XServerVendor( xDisplay );
		mNativeData.xSessionData.sessionInfo.displayString = XDisplayString( xDisplay );
	}

	void X11SysContext::_releaseX11ContextState()
	{
		mNativeData.xSessionData.display = nullptr;
		mNativeData.xSessionData.screenIndex = -1;
		mNativeData.xSessionData.rootWindowXID = Platform::E_X11_XID_NONE;
		mNativeData.xSessionData.atomCache.wmProtocolDelete = 0;
		mNativeData.xSessionData.atomCache.wmProtocolDestroy = 0;
		mNativeData.xSessionData.atomCache.wmState = 0;
		mNativeData.xSessionData.atomCache.wmStateFullscreen = 0;
		mNativeData.xSessionData.sessionInfo.connectionNumber = -1;
		mNativeData.xSessionData.sessionInfo.vendorName.clear();
		mNativeData.xSessionData.sessionInfo.displayString.clear();
	}

	std::string X11SysContext::queryCurrentProcessWorkingDirectory() const
	{
		char workingDirStrBuffer[2048];
		getcwd( workingDirStrBuffer, 2048 );

		return std::string( workingDirStrBuffer );
	}

	std::string X11SysContext::queryCurrentProcessExecutableFilePath() const
	{
		pid_t currentProcessID = getpid();

		std::string executableFilePath;
		std::string exeLink = std::string( "/proc/" ) + std::to_string( currentProcessID ) + std::string( "/exe" );

		if( char * bufferPtr = realpath( exeLink.c_str(), nullptr ) )
		{
			executableFilePath = bufferPtr;
			free( bufferPtr );
		}

		return executableFilePath;
	}

} // namespace Ic3::System
#endif // IC3_PCL_TARGET_SYSAPI_X11
