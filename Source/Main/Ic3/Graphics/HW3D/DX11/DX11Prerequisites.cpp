
#include "DX11CommandList.h"

namespace Ic3::Graphics::GCI
{

	namespace smutil
	{

		ID3D11DeviceContext1 * getD3D11DeviceContextFromCommandList( void * pDX11CommandList )
		{
			return reinterpret_cast<DX11CommandList *>( pDX11CommandList )->mD3D11DeviceContext1.Get();
		}

	}

}
