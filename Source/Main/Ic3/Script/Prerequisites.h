
#ifndef __IC3_SCRIPT_PREREQUISITES_H__
#define __IC3_SCRIPT_PREREQUISITES_H__

#include <Ic3/CoreLib/Prerequisites.h>

#if( IC3_BUILD_STATIC )
#  define IC3_SCRIPT_API
#  define IC3_SCRIPT_CLASS
#  define IC3_SCRIPT_OBJ    extern
#else
#  if( IC3_SCRIPT_BUILD )
#    define IC3_SCRIPT_API    IC3_PCL_ATTR_DLL_EXPORT
#    define IC3_SCRIPT_CLASS  IC3_PCL_ATTR_DLL_EXPORT
#    define IC3_SCRIPT_OBJ    IC3_PCL_ATTR_DLL_EXPORT
#  else
#    define IC3_SCRIPT_API    IC3_PCL_ATTR_DLL_IMPORT
#    define IC3_SCRIPT_CLASS  IC3_PCL_ATTR_DLL_IMPORT
#    define IC3_SCRIPT_OBJ    IC3_PCL_ATTR_DLL_IMPORT
#  endif
#endif

namespace Ic3::Script
{

	template <typename T>
	struct TypeMetaInfo;

#define ic3DeclareScriptType( pClass )                              \
	template <> struct TypeMetaInfo<pClass> {                       \
		static constexpr const char * sClassName = #pClass;         \
		static constexpr const char * sNamespace = "";              \
		static constexpr const char * sQualifiedTypeName = #pClass; \
	};

#define ic3DeclareScriptTypeExt( pClass, pNamespace )                              \
	template <> struct TypeMetaInfo<pClass> {                                      \
		static constexpr const char * sClassName = #pClass;                        \
		static constexpr const char * sNamespace = pNamespace;                     \
		static constexpr const char * sQualifiedTypeName = pNamespace "." #pClass; \
	};

	template <typename TClass>
	class ClassDefinition
	{
	};

	template <typename TFunction>
	class FunctionDef;

}

#endif //__IC3_SCRIPT_PREREQUISITES_H__
