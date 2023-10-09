
#ifndef __IC3_SYSTEM_FRAME_H__
#define __IC3_SYSTEM_FRAME_H__

#include "EventSource.h"
#include <Ic3/Math/Vector.h>

namespace Ic3::System
{

	using FramePos = Math::Pos2i;
	using FrameSize = Math::Size2u;

	/// @brief Predefined frame position: auto. When used, frame is spawned centered.
	inline constexpr FramePos CX_FRAME_POS_AUTO { -1, -1 };

	/// @brief Predefined frame position: origin. When used, frame is spawned at the origin point (0,0).
	inline constexpr FramePos CX_FRAME_POS_ORIGIN { 0, 0 };

	/// @brief Predefined frame size: auto. When used, frame size is selected by the implementation.
	inline constexpr FrameSize CX_FRAME_SIZE_AUTO { 0, 0 };

	/// @brief Predefined frame size: max. When used, frame size is the maximum size allowed by the system.
	/// Max size usually means the current size of the screen, but selected frame style may affect this (Win32).
	inline constexpr FrameSize CX_FRAME_SIZE_MAX { CX_UINT32_MAX, CX_UINT32_MAX };

	/// @brief Window styles supported by the system.
	enum class EFrameStyle : enum_default_value_t
	{
		//
		Unspecified,
		// Title-bar only window, no system menu, non-resizeable, no close or minimize button.
		Caption,
		// Fixed window, with system menu, non-resizeable.
		Fixed,
		//
		Overlay,
		// "Normal" window with title bar, minimize/maximize buttons (if applicable), resizeable.
		Resizeable,
		// Default style is the one which is most convenient for rendering - fixed frame.
		Default = static_cast< enum_default_value_t >( Fixed ),
	};

	enum EFrameGeometryUpdateFlags : uint32
	{
		E_FRAME_GEOMETRY_UPDATE_FLAG_POSITION_BIT = 0x01,

		E_FRAME_GEOMETRY_UPDATE_FLAG_SIZE_BIT = 0x02,

		E_FRAME_GEOMETRY_UPDATE_FLAG_STYLE_BIT = 0x08,

		E_FRAME_GEOMETRY_UPDATE_FLAG_SIZE_CLIENT_AREA_BIT = 0x10 | E_FRAME_GEOMETRY_UPDATE_FLAG_SIZE_BIT,

		E_FRAME_GEOMETRY_UPDATE_FLAG_SIZE_OUTER_RECT_BIT = 0x20 | E_FRAME_GEOMETRY_UPDATE_FLAG_SIZE_BIT,

		E_FRAME_GEOMETRY_UPDATE_FLAGS_ALL_DEFAULT = E_FRAME_GEOMETRY_UPDATE_FLAG_POSITION_BIT |
		                                            E_FRAME_GEOMETRY_UPDATE_FLAG_SIZE_OUTER_RECT_BIT |
		                                            E_FRAME_GEOMETRY_UPDATE_FLAG_STYLE_BIT
	};

	/// @brief
	enum class EFrameSizeMode : enum_default_value_t
	{
		ClientArea,
		OuterRect
	};

	/// @brief
	struct FrameGeometry
	{
		FramePos position;
		FrameSize size;
		EFrameStyle style;
	};

	inline constexpr FrameGeometry CX_FRAME_GEOMETRY_DEFAULT{ CX_FRAME_POS_AUTO, CX_FRAME_SIZE_AUTO, EFrameStyle::Default };

	class Frame : public EventSource
	{
	public:
		explicit Frame( SysContextHandle pSysContext );
		virtual ~Frame() noexcept;

		virtual void resizeClientArea( const FrameSize & pSize ) = 0;

		virtual void resizeFrame( const FrameSize & pSize ) = 0;

		virtual void setFullscreenMode( bool pEnable ) = 0;

		virtual void setTitle( const std::string & pTitleText ) = 0;

		virtual void updateGeometry( const FrameGeometry & pFrameGeometry,
		                             Bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) = 0;

		IC3_SYSTEM_API_NODISCARD virtual FrameSize getClientAreaSize() const = 0;

		IC3_SYSTEM_API_NODISCARD virtual FrameSize getFrameSize() const = 0;

		IC3_SYSTEM_API_NODISCARD virtual bool isFullscreen() const = 0;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_FRAME_H__
