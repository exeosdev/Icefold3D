
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_GPU_CMD_CONTEXT_H__
#define __IC3_GRAPHICS_HW3D_DX12_GPU_CMD_CONTEXT_H__

#include "DX12Prerequisites.h"
#include <Ic3/Graphics/GCI/cmds/CommandContext.h>

#define ic3DX12CmdContextVerifyGraphics( pDX12CmdContext ) \
	ic3DebugAssert( pDX12CmdContext->mD3D12GraphicsCommandList )

namespace Ic3::Graphics::GCI
{

	struct DX12RenderTargetState
	{
		using RTVDescriptorArray = std::array<D3D12_CPU_DESCRIPTOR_HANDLE, gpm::RT_MAX_COLOR_ATTACHMENTS_NUM>;
		uint32 rtvDescriptorsNum = 0;
		RTVDescriptorArray rtvDescriptorArray;
		D3D12_CPU_DESCRIPTOR_HANDLE dsvDescriptor = cvD3D12CPUDescriptorEmpty;
	};

	/// @brief
	class ICFGX_DX12_CLASS DX12CommandContext final : public CommandContext
	{
		friend class DX12GPUCmdManager;
		friend class DX12ScreenPresentationLayer;

	public:
		ComPtr<ID3D12CommandAllocator> const mD3D12CommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> const mD3D12GraphicsCommandList;
		D3D12_COMMAND_LIST_TYPE const mD3D12CommandListType;

		DX12CommandContext( DX12GPUCmdManager & pDX12GPUCmdManager,
		                   ComPtr<ID3D12CommandAllocator> pD3D12CommandAllocator,
		                   ComPtr<ID3D12GraphicsCommandList> pD3D12GraphicsCommandList );

		virtual ~DX12CommandContext();

		virtual void beginCommandSequence( const CommandContextCommandSequenceBeginInfo & pInfo ) override;

		virtual void endCommandSequence( const CommandContextCommandSequenceEndInfo & pInfo ) override;

		virtual void executeSecondaryContext( CommandContext & pSecondaryCmdContext ) override;

		virtual void clearColorTarget( const Math::RGBAColorR32Norm & pColor ) override;

	private:
		void setRenderTargetState( const DX12RenderTargetState & pRenderTargetState );

	private:
		DX12RenderTargetState _renderTargetState;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_GPU_CMD_CONTEXT_H__
