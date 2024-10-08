
#ifndef __IC3_CPPX_STRING_EXT_H__
#define __IC3_CPPX_STRING_EXT_H__

#include "ArrayView.h"

namespace Ic3::Cppx
{

	template <typename T, size_t tMaxCapacity>
	class StaticVector
	{
	public:
		StaticVector() = default;

		StaticVector( StaticVector && pSrcObject )
		{}

		StaticVector( const StaticVector & pSrcObject )
		{}

		StaticVector & operator=( StaticVector && pRhs )
		{}

		StaticVector & operator=( const StaticVector & pRhs )
		{}

	private:
		using Storage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;
		Storage _data[tMaxCapacity];
		size_t _beginIndex = tMaxCapacity;
		size_t _endIndex = tMaxCapacity;
	};

}

#endif /* __IC3_CPPX_STRING_EXT_H__ */
