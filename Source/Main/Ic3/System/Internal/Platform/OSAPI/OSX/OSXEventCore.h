
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_EVENT_CORE_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_EVENT_CORE_H__

#include "OSXCommon.h"
#include <Ic3/System/EventCore.h>
#include <Ic3/System/EventObject.h>
#include <Ic3/Cppx/BitUtils.h>

#import <AppKit/NSEvent.h>

namespace Ic3::System
{

	class OSXEventController;

	namespace Platform
	{

		enum OSXEventID : NSUInteger
		{
			OSXEventIDNull                      = 0,
			OSXEventIDGenericLeftMouseDown      = NSEventTypeLeftMouseDown      ,
			OSXEventIDGenericLeftMouseUp        = NSEventTypeLeftMouseUp        ,
			OSXEventIDGenericRightMouseDown     = NSEventTypeRightMouseDown     ,
			OSXEventIDGenericRightMouseUp       = NSEventTypeRightMouseUp       ,
			OSXEventIDGenericMouseMoved         = NSEventTypeMouseMoved         ,
			OSXEventIDGenericLeftMouseDragged   = NSEventTypeLeftMouseDragged   ,
			OSXEventIDGenericRightMouseDragged  = NSEventTypeRightMouseDragged  ,
			OSXEventIDGenericMouseEntered       = NSEventTypeMouseEntered       ,
			OSXEventIDGenericMouseExited        = NSEventTypeMouseExited        ,
			OSXEventIDGenericKeyDown            = NSEventTypeKeyDown            ,
			OSXEventIDGenericKeyUp              = NSEventTypeKeyUp              ,
			OSXEventIDGenericFlagsChanged       = NSEventTypeFlagsChanged       ,
			OSXEventIDGenericAppKitDefined      = NSEventTypeAppKitDefined      ,
			OSXEventIDGenericSystemDefined      = NSEventTypeSystemDefined      ,
			OSXEventIDGenericApplicationDefined = NSEventTypeApplicationDefined ,
			OSXEventIDGenericPeriodic           = NSEventTypePeriodic           ,
			OSXEventIDGenericCursorUpdate       = NSEventTypeCursorUpdate       ,
			OSXEventIDGenericRotate             = NSEventTypeRotate             ,
			OSXEventIDGenericBeginGesture       = NSEventTypeBeginGesture       ,
			OSXEventIDGenericEndGesture         = NSEventTypeEndGesture         ,
			OSXEventIDX21                       ,
			OSXEventIDGenericScrollWheel        = NSEventTypeScrollWheel        ,
			OSXEventIDGenericTabletPoint        = NSEventTypeTabletPoint        ,
			OSXEventIDGenericTabletProximity    = NSEventTypeTabletProximity    ,
			OSXEventIDGenericOtherMouseDown     = NSEventTypeOtherMouseDown     ,
			OSXEventIDGenericOtherMouseUp       = NSEventTypeOtherMouseUp       ,
			OSXEventIDGenericOtherMouseDragged  = NSEventTypeOtherMouseDragged  ,
			OSXEventIDX28                       ,
			OSXEventIDGenericGesture            = NSEventTypeGesture            ,
			OSXEventIDGenericMagnify            = NSEventTypeMagnify            ,
			OSXEventIDGenericSwipe              = NSEventTypeSwipe              ,
			OSXEventIDGenericSmartMagnify       = NSEventTypeSmartMagnify       ,
			OSXEventIDGenericQuickLook          = NSEventTypeQuickLook          ,
			OSXEventIDGenericPressure           = NSEventTypePressure           ,
			OSXEventIDX35                       ,
			OSXEventIDX36                       ,
			OSXEventIDGenericDirectTouch        = NSEventTypeDirectTouch        ,
			OSXEventIDGenericChangeMode         = NSEventTypeChangeMode         ,
			OSXEventIDWindowDidExpose                  ,
			OSXEventIDWindowDidMove                    ,
			OSXEventIDWindowDidResize                  ,
			OSXEventIDWindowMiniaturize                ,
			OSXEventIDWindowDidDeminiaturize           ,
			OSXEventIDWindowDidBecomeKey               ,
			OSXEventIDWindowDidResignKey               ,
			OSXEventIDWindowDidChangeBackingProperties ,
			OSXEventIDWindowDidChangeScreenProfile     ,
			OSXEventIDWindowWillEnterFullScreen        ,
			OSXEventIDWindowDidEnterFullScreen         ,
			OSXEventIDWindowWillExitFullScreen         ,
			OSXEventIDWindowDidExitFullScreen          ,
			OSXEventIDWindowWillClose                  ,
		};

		static_assert( OSXEventIDX21 == 21 );
		static_assert( OSXEventIDX28 == 28 );
		static_assert( OSXEventIDX35 == 35 );
		static_assert( OSXEventIDX36 == 36 );

		enum EOSXMouseScrollEventFlags : uint32
		{
			E_OSX_MOUSE_SCROLL_EVENT_FLAG_SCROLLING_MODE_NORMAL_BIT = 0x01,
			E_OSX_MOUSE_SCROLL_EVENT_FLAG_SCROLLING_MODE_PRECISE_BIT = 0x02,
			E_OSX_MOUSE_SCROLL_EVENT_FLAG_SCROLLING_DIRECTION_NORMAL_BIT = 0x10,
			E_OSX_MOUSE_SCROLL_EVENT_FLAG_SCROLLING_DIRECTION_INVERTED_BIT = 0x20,
		};

		struct OSXEventSourceNativeData : public OSXNativeDataCommon
		{
			NSWindow * nsWindow = nil;
			NSView * nsView = nil;
			id nsEventListener = nil;
		};

		struct OSXEventControllerNativeData : public OSXNativeDataCommon
		{
			CGPoint lastCursorPosReal;
		};

		struct NativeEventType
		{
			OSXEventID nsAppEventID = OSXEventIDNull;

			NSEvent * nsEvent = nil;

			NSNotification * nsNotification = nil;

			NSWindow * nsSourceWindow = nil;

			explicit NativeEventType( NSEvent * pNSEvent )
			: nsAppEventID( static_cast<OSXEventID>( [pNSEvent type] ) )
			, nsEvent( pNSEvent )
			, nsSourceWindow( [pNSEvent window] )
			{}

			explicit NativeEventType( NSEvent * pNSEvent, NSEventType pOverrideEventID )
			: nsAppEventID( static_cast<OSXEventID>( pOverrideEventID ) )
			, nsEvent( pNSEvent )
			, nsSourceWindow( [pNSEvent window] )
			{}

			NativeEventType( OSXEventID pAppEventID, NSNotification * pNSNotification )
			: nsAppEventID( pAppEventID )
			, nsNotification( pNSNotification )
			, nsSourceWindow( static_cast<NSWindow *>( [pNSNotification object] ) )
			{}
		};

		EventSource * osxFindEventSourceByNSWindow( OSXEventController & pEventController, NSWindow * pNSWindow );

		void osxCreateEventListener( OSXEventSourceNativeData & pEventSourceNativeData );

		bool osxTranslateEvent( EventController & pEventController, EventSource & pEventSource, const NativeEventType & pNativeEvent, EventObject & pOutEvent );

		IC3_SYSTEM_API_NODISCARD bool osxIsEventTypeInputKeyboard( OSXEventID pEventID );

		IC3_SYSTEM_API_NODISCARD bool osxIsEventTypeInputMouse( OSXEventID pEventID );

		IC3_SYSTEM_API_NODISCARD bool osxIsEventTypeInputOther( OSXEventID pEventID );

		IC3_SYSTEM_API_NODISCARD bool osxIsEventTypeWindow( OSXEventID pEventID );

		IC3_SYSTEM_API_NODISCARD EMouseButtonID osxQueryMouseButtonID( NSEvent * pNSEvent );

		IC3_SYSTEM_API_NODISCARD Math::Vec2i32 osxQueryMouseRelativePosition( NSEvent * pNSEvent );

		IC3_SYSTEM_API_NODISCARD Bitmask<EOSXMouseScrollEventFlags> osxQueryMouseScrollEventFlags( const NativeEventType & pNativeEvent );

	}

	class OSXEventController : public OSXNativeObject<EventController, Platform::OSXEventControllerNativeData>
	{
	public:
		OSXEventController( SysContextHandle pSysContext );
		virtual ~OSXEventController() noexcept;

	private:
		/// @copybrief EventController::_nativeRegisterEventSource
		virtual void _nativeRegisterEventSource( EventSource & pEventSource ) override final;

		/// @copybrief EventController::_nativeUnregisterEventSource
		virtual void _nativeUnregisterEventSource( EventSource & pEventSource ) override final;

		/// @copybrief EventController::_nativeDispatchPendingEvents
		virtual bool _nativeDispatchPendingEvents() override final;

		/// @copybrief EventController::_nativeDispatchPendingEventsWait
		virtual bool _nativeDispatchPendingEventsWait() override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_EVENT_CORE_H__
