
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_EGL_OPENGL_DRIVER_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_EGL_OPENGL_DRIVER_H__

#include <Ic3/System/OpenGLCommon.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "eglCoreUtils.h"

namespace Ic3::System
{

	namespace Platform
	{

		inline constexpr size_t CX_EGL_MAX_EGL_CONFIGS_NUM = 256u;

		inline constexpr size_t CX_EGL_MAX_EGL_CONFIG_ATTRIBUTES_NUM = 64u;

		struct EGLDisplaySurfaceNativeData
		{
			EGLDisplay eDisplay = EGL_NO_DISPLAY;
			EGLNativeWindowType eNativeWindow = nullptr;
			EGLConfig eFBConfig = nullptr;
			EGLSurface eSurfaceHandle = nullptr;
		};

		struct EGLRenderContextNativeData
		{
			EGLDisplay eDisplay = EGL_NO_DISPLAY;
			EGLContext eContextHandle = nullptr;
		};

		struct EGLDriverNativeData
		{
			EGLDisplay eDisplay = EGL_NO_DISPLAY;
			EGLNativeWindowType eNativeWindow = nullptr;
			Version eglVersion;
		};

		void eglInitializeGLDriver( EGLDriverNativeData & pEGLDriverNativeData );

		void eglReleaseGLDriver( EGLDriverNativeData & pEGLDriverNativeData );

		EGLConfig eglChooseCoreFBConfig( EGLDisplay pDisplay,
		                                 const VisualConfig & pVisualConfig,
		                                 const Version & pTargetAPIVersion );

		EGLint eglQueryFBConfigAttribute( EGLDisplay pEGLDisplay, EGLConfig pEGLConfig, EGLenum pAttribute );

		void eglCreateSurface( EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
		                       EGLDisplay pEGLDisplay,
		                       EGLNativeWindowType pWindow,
		                       EGLConfig pEGLConfig,
		                       const VisualConfig & pVisualConfig );

		void eglCreateSurfaceForCurrentThread( EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData );

		void eglDestroySurface( EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData );

		void eglCreateCoreContext( EGLRenderContextNativeData & pEGLContextNativeData,
		                           const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
		                           const OpenGLRenderContextCreateInfo & pCreateInfo );

		void eglCreateCoreContextForCurrentThread( EGLRenderContextNativeData & pEGLContextNativeData );

		void eglDestroyRenderContext( EGLRenderContextNativeData & pEGLContextNativeData );

		void eglBindContextForCurrentThread( const EGLRenderContextNativeData & pEGLContextNativeData,
		                                     const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData );

	}

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_EGL_OPENGL_DRIVER_H__
