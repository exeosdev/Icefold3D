
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_COMMAND_DEFS_H__
#define __IC3_GRAPHICS_GCI_COMMON_COMMAND_DEFS_H__

#include "State/CommonGPUStateDefs.h"
#include "State/RenderTargetCommon.h"
#include <atomic>

namespace Ic3::Graphics::GCI
{

	class CommandContext;
	class CommandContextDirect;
	class CommandContextDirectTransfer;
	class CommandContextDirectCompute;
	class CommandContextDirectGraphics;
	class CommandContextDeferred;
	class CommandContextDeferredGraphics;
	class CommandList;
	class CommandSystem;

	class ComputePipelineStateObject;
	class GraphicsPipelineStateObject;

	using gpu_cmd_device_queue_id_t = native_uint;

    enum : gpu_cmd_device_queue_id_t
    {
        E_DEVICE_COMMAND_QUEUE_ID_DEFAULT_GRAPHICS = 0u,
        E_DEVICE_COMMAND_QUEUE_ID_DEFAULT_COMPUTE = 1u,
        E_DEVICE_COMMAND_QUEUE_ID_DEFAULT_TRANSFER = 2u,
        E_DEVICE_COMMAND_QUEUE_ID_PRESENT = 3u,
        E_DEVICE_COMMAND_QUEUE_ID_UNKNOWN = QLimits<gpu_cmd_device_queue_id_t>::maxValue,
    };

	enum ECommandListActionFlags : uint32
	{
		E_COMMAND_LIST_ACTION_FLAG_RENDER_PASS_APPLY_PIPELINE_STATE_BIT = 0x01,
		E_COMMAND_LIST_ACTION_FLAG_RENDER_PASS_PRESERVE_DYNAMIC_STATE_BIT = 0x02,

		E_COMMAND_LIST_ACTION_FLAGS_DEFAULT =
		E_COMMAND_LIST_ACTION_FLAG_RENDER_PASS_APPLY_PIPELINE_STATE_BIT,
	};

	enum ECommandObjectPropertyFlags : uint32
	{
		E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_COMMON_BIT = 0x01,
		E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_TRANSFER_BIT = 0x02,
		E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_COMPUTE_BIT = 0x04,
		E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_GRAPHICS_BIT = 0x08,
		E_COMMAND_OBJECT_PROPERTY_MASK_COMMAND_CLASS_ALL = 0x0F,

		E_COMMAND_OBJECT_PROPERTY_FLAG_EXECUTION_MODE_DIRECT_BIT = 0x10,
		E_COMMAND_OBJECT_PROPERTY_FLAG_EXECUTION_MODE_DEFERRED_BIT = 0x20,
		E_COMMAND_OBJECT_PROPERTY_MASK_EXECUTION_MODE_ALL = 0x30,

		E_COMMAND_OBJECT_PROPERTY_MASK_CONTEXT_FAMILY_DIRECT =
			E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_COMMON_BIT |
			E_COMMAND_OBJECT_PROPERTY_FLAG_EXECUTION_MODE_DIRECT_BIT,

		E_COMMAND_OBJECT_PROPERTY_MASK_CONTEXT_FAMILY_DEFERRED =
			E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_COMMON_BIT |
			E_COMMAND_OBJECT_PROPERTY_FLAG_EXECUTION_MODE_DEFERRED_BIT,

		E_COMMAND_OBJECT_PROPERTY_MASK_CONTEXT_FAMILY_DIRECT_TRANSFER =
			E_COMMAND_OBJECT_PROPERTY_MASK_CONTEXT_FAMILY_DIRECT |
			E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_TRANSFER_BIT,

		E_COMMAND_OBJECT_PROPERTY_MASK_CONTEXT_FAMILY_DIRECT_COMPUTE =
			E_COMMAND_OBJECT_PROPERTY_MASK_CONTEXT_FAMILY_DIRECT_TRANSFER |
			E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_COMPUTE_BIT,

		E_COMMAND_OBJECT_PROPERTY_MASK_CONTEXT_FAMILY_DIRECT_GRAPHICS =
			E_COMMAND_OBJECT_PROPERTY_MASK_CONTEXT_FAMILY_DIRECT_COMPUTE |
			E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_GRAPHICS_BIT,

		E_COMMAND_OBJECT_PROPERTY_MASK_CONTEXT_FAMILY_DEFERRED_GRAPHICS =
			E_COMMAND_OBJECT_PROPERTY_MASK_CONTEXT_FAMILY_DEFERRED |
			E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_GRAPHICS_BIT,

		E_COMMAND_OBJECT_PROPERTY_MASK_ALL =
			E_COMMAND_OBJECT_PROPERTY_MASK_COMMAND_CLASS_ALL |
			E_COMMAND_OBJECT_PROPERTY_MASK_EXECUTION_MODE_ALL,
	};

	enum class ECommandExecutionMode : uint32
	{
		Direct = E_COMMAND_OBJECT_PROPERTY_FLAG_EXECUTION_MODE_DIRECT_BIT,
		Deferred = E_COMMAND_OBJECT_PROPERTY_FLAG_EXECUTION_MODE_DEFERRED_BIT
	};

	enum class ECommandQueueClass : uint32
	{
		Transfer = E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_COMMON_BIT | E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_TRANSFER_BIT,
		Compute = static_cast<uint32>( Transfer ) | E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_COMPUTE_BIT,
		Graphics = static_cast<uint32>( Compute ) | E_COMMAND_OBJECT_PROPERTY_FLAG_COMMAND_CLASS_GRAPHICS_BIT,
		Default = 0,
	};

	enum class ECommandObjectType : uint32
	{
		DirectTransfer = static_cast<uint32>( ECommandQueueClass::Transfer ) | static_cast<uint32>( ECommandExecutionMode::Direct ),
		DirectCompute = static_cast<uint32>( ECommandQueueClass::Compute ) | static_cast<uint32>( ECommandExecutionMode::Direct ),
		DirectGraphics = static_cast<uint32>( ECommandQueueClass::Graphics ) | static_cast<uint32>( ECommandExecutionMode::Direct ),
		DeferredGraphics = static_cast<uint32>( ECommandQueueClass::Graphics ) | static_cast<uint32>( ECommandExecutionMode::Deferred ),
		Undefined = 0
	};

	using ECommandContextType = ECommandObjectType;
	using ECommandListType = ECommandObjectType;

	namespace CxDef
	{

		inline constexpr ECommandExecutionMode getCommandObjectExecutionMode( ECommandObjectType pType ) noexcept
		{
			return static_cast<ECommandExecutionMode>( static_cast<uint32>( pType ) & E_COMMAND_OBJECT_PROPERTY_MASK_EXECUTION_MODE_ALL );
		}

		inline constexpr Bitmask<ECommandObjectPropertyFlags> getCommandObjectPropertyFlags( ECommandObjectType pType ) noexcept
		{
			return makeBitmask<uint32>( static_cast<uint32>( pType ) & E_COMMAND_OBJECT_PROPERTY_MASK_ALL );
		}

	}

	enum class ECommandSubmitStateOp : enum_default_value_t
	{
		// Context is a temporary, "fire-and-forget" context. After its submission,
		// its internal state is invalidated, and it may no longer be used for command
		// submission. User must acquire a new context to do so.
		Discard,

		// Context is used multiple times or stored in a permanent manner. After the
		// submission, its internal state is updated with a new data and becomes ready
		// for further command recording/submission.
		// This flag is useful if it is required for a context object to stay alive
		// (and useful) between multiple submissions (e.g. captured within a lambda).
		Reset
	};

	enum class ECommandSubmitSyncMode : enum_default_value_t
	{
		// Enables the sync of CmdContext execution status. It causes two things to happen:
		// 1) Additional command is issued when submitting the command context, which will
		//    signal a sync object (fence or semaphore) used for the synchronization process.
		// 2) Returns a non-empty CommandSync object (its operator bool() evaluates to true),
		//    which can be used to wait for completion of all commands issued before.
		Default,

		// No sync option is provided. Commands are executed in a driver-specific manner.
		// Only commands executed on the same device queue are guaranteed to be executed
		// in the order of recording/submission.
		None
	};

	struct CommandContextSubmitInfo
	{
		ECommandQueueClass queuePreference = ECommandQueueClass::Default;
		ECommandSubmitStateOp stateOp = ECommandSubmitStateOp::Discard;
		ECommandSubmitSyncMode syncMode = ECommandSubmitSyncMode::None;
	};

	inline constexpr CommandContextSubmitInfo CX_COMMAND_CONTEXT_SUBMIT_DEFAULT {};

	struct CommandSync
	{
	public:
		using SyncDataReleaseFuncPtr = void ( * )( void * );

		void * syncData = nullptr;
		SyncDataReleaseFuncPtr syncDataReleaseFunc = nullptr;

	public:
		CommandSync() = default;

		CommandSync( CommandSync && pSrcObject )
		: syncData( pSrcObject.syncData )
		, syncDataReleaseFunc( pSrcObject.syncDataReleaseFunc )
		{
			pSrcObject.syncData = nullptr;
			pSrcObject.syncDataReleaseFunc = nullptr;
		}

		~CommandSync()
		{
			if( syncData && syncDataReleaseFunc )
			{
				syncDataReleaseFunc( syncData );
				syncData = nullptr;
			}
		}

		CommandSync & operator=( CommandSync && pRhs )
		{
			CommandSync( std::move( pRhs ) ).swap( *this );
			return *this;
		}

		explicit operator bool() const
		{
			return syncData != nullptr;
		}

		void swap( CommandSync & pOther )
		{
			std::swap( syncData, pOther.syncData );
			std::swap( syncDataReleaseFunc, pOther.syncDataReleaseFunc );
		}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_COMMAND_DEFS_H__
