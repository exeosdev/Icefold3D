
#include "VertexPipelineLayouts.h"

namespace Ic3
{

	void SGenericVertexAttributeInfo::initBaseAttributeFromDefinition( SVertexAttributeDefinition pDefinition )
	{
		baseFormat = pDefinition.baseFormat;
		baseAttributeIndex = pDefinition.baseAttributeIndex;
		instanceRate = pDefinition.instanceRate;
		componentsNum = pDefinition.componentsNum;
		dataStride = pDefinition.getDataStride();
		vertexStreamIndex = pDefinition.vertexStreamIndex;
		vertexStreamRelativeOffset = pDefinition.vertexStreamRelativeOffset;
		semantics = std::move( pDefinition.semantics );
		attributeClass = EVertexAttributeClass::BaseAttribute;
		componentIndex = 0;
	}

	void SGenericVertexAttributeInfo::initSubComponentFromBaseAttribute( const SGenericVertexAttributeInfo & pBaseAttribute, uint32 pSubComponentIndex )
	{
		baseFormat = pBaseAttribute.baseFormat;
		baseAttributeIndex = pBaseAttribute.baseAttributeIndex;
		instanceRate = pBaseAttribute.instanceRate;
		componentsNum = pBaseAttribute.componentsNum;
		dataStride = pBaseAttribute.dataStride;
		vertexStreamIndex = pBaseAttribute.vertexStreamIndex;
		vertexStreamRelativeOffset = 0;
		attributeClass = EVertexAttributeClass::SubComponent;
		componentIndex = pSubComponentIndex;
	}

	namespace GCIUtils
	{

		SGenericVertexAttributeInfo & updateVertexAttributeLayoutWithAttributeDefinition(
				SVertexAttributeDefinition pAttributeDefinition,
				SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				SVertexStreamArrayLayoutData * pStreamLayoutData )
		{
			const auto baseAttributeIndex = pAttributeDefinition.baseAttributeIndex;
			const auto lastComponentIndex = baseAttributeIndex + pAttributeDefinition.componentsNum - 1;

			auto & baseAttributeInfo = pAttributeLayoutData[baseAttributeIndex];
			baseAttributeInfo.initBaseAttributeFromDefinition( std::move( pAttributeDefinition ) );
			baseAttributeInfo.attributeClass = EVertexAttributeClass::BaseAttribute;
			baseAttributeInfo.componentIndex = 0;

			pAttributeLayoutData.activeBaseAttributesNum += 1;
			pAttributeLayoutData.activeGenericAttributesNum += 1;
			pAttributeLayoutData.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( baseAttributeIndex ) );
			pAttributeLayoutData.activeAttributeSemanticsMask.set( static_cast<uint32>( baseAttributeInfo.semantics.smtID ) );

			pAttributeLayoutData.semanticIDMap[baseAttributeInfo.semantics.smtID] = baseAttributeIndex;
			pAttributeLayoutData.semanticNameMap[baseAttributeInfo.semantics.smtName] = baseAttributeIndex;

			if( baseAttributeInfo.componentsNum == 1 )
			{
				baseAttributeInfo.baseAttribute = &baseAttributeInfo;
				baseAttributeInfo.nextComponent = nullptr;
			}
			else
			{
				auto * baseAttributePtr = &baseAttributeInfo;
				auto * previousAttributePtr = baseAttributePtr;

				for( uint32 nComponent = 1; nComponent < baseAttributeInfo.componentsNum; ++nComponent )
				{
					const auto subComponentIndex = baseAttributeInfo.baseAttributeIndex + nComponent;
					const auto subComponentOffset = nComponent * baseAttributeInfo.dataStride;

					auto & subComponentInfo = pAttributeLayoutData[subComponentIndex];
					subComponentInfo.initSubComponentFromBaseAttribute( baseAttributeInfo, nComponent );
					subComponentInfo.vertexStreamRelativeOffset = baseAttributeInfo.vertexStreamRelativeOffset + subComponentOffset;

					pAttributeLayoutData.activeGenericAttributesNum += 1;
					pAttributeLayoutData.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( subComponentIndex ) );

					previousAttributePtr->baseAttribute = baseAttributePtr;
					previousAttributePtr->nextComponent = &subComponentInfo;
					previousAttributePtr = previousAttributePtr->nextComponent;
				}

				previousAttributePtr->baseAttribute = baseAttributePtr;
				previousAttributePtr->nextComponent = nullptr;
			}

			if( baseAttributeIndex < pAttributeLayoutData.activeAttributesRange.begin )
			{
				pAttributeLayoutData.activeAttributesRange.begin = baseAttributeIndex;
			}

			if( lastComponentIndex > pAttributeLayoutData.activeAttributesRange.end )
			{
				pAttributeLayoutData.activeAttributesRange.end = lastComponentIndex;
			}

			return baseAttributeInfo;
		}

		bool createVertexAttributeArrayLayout(
				const ArrayView<SVertexAttributeDefinition> & pAttributeDefinitions,
				SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				SVertexStreamArrayLayoutData * pStreamLayoutData )
		{
		}

		bool createVertexStreamArrayLayout(
				SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				SVertexStreamArrayLayoutData & pStreamLayoutData )
		{
		}

		const char * getAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pGCIAttributeFormat )
		{
			switch( pGCIAttributeFormat )
			{
				ic3CaseReturn( GCI::EVertexAttribFormat::F16      , "1F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::F32      , "1F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I8       , "1I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::I16      , "1I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I32      , "1I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U8       , "1U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::U16      , "1U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U32      , "1U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I8N      , "1I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I16N     , "1I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::U8N      , "1U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U16N     , "1U16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2F16  , "2F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2F32  , "2F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I8   , "2I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I16  , "2I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I32  , "2I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U8   , "2U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U16  , "2U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U32  , "2U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I8N  , "2I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I16N , "2I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U8N  , "2U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U16N , "2U16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3F32  , "3F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3I32  , "3I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3U32  , "3U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4F16  , "4F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4F32  , "4F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I8   , "4I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I16  , "4I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I32  , "4I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U8   , "4U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U16  , "4U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U32  , "4U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I8N  , "4I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I16N , "4I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U8N  , "4U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U16N , "4U16N" );
				ic3CaseDefaultBreak();
			}

			return "#";
		}

		const char * getShaderSemanticShortName( const std::string_view & pSemanticName )
		{
			if( pSemanticName == "POSITION"  ) return "P";
			if( pSemanticName == "NORMAL"    ) return "N";
			if( pSemanticName == "TANGENT"   ) return "T";
			if( pSemanticName == "BITANGENT" ) return "B";
			if( pSemanticName == "COLOR"     ) return "C";
			if( pSemanticName == "TEXCOORD0" ) return "UV0";
			if( pSemanticName == "TEXCOORD1" ) return "UV1";
			if( pSemanticName == "TEXCOORD2" ) return "UV2";
			if( pSemanticName == "TEXCOORD3" ) return "UV3";
			if( pSemanticName == "TEXCOORD4" ) return "UV4";
			if( pSemanticName == "TEXCOORD5" ) return "UV5";
			if( pSemanticName == "TEXCOORD6" ) return "UV6";
			if( pSemanticName == "TEXCOORD7" ) return "UV7";
			if( pSemanticName == "INSTANCE0" ) return "I0";
			if( pSemanticName == "INSTANCE1" ) return "I1";

			return nullptr;
		}

		std::string generateVertexAttributeFormatString( const SGenericVertexAttributeInfo & pAttributeInfo )
		{
			std::string formatStr;
			formatStr.reserve( 16 );
			formatStr.append( 1, 'a' );
			formatStr.append( std::to_string( pAttributeInfo.baseAttributeIndex ) );

			if( const auto * semanticsIDCStr = getShaderSemanticShortName( pAttributeInfo.semantics.smtName ) )
			{
				formatStr.append( semanticsIDCStr );
			}
			else
			{
				formatStr.append( pAttributeInfo.semantics.smtName );
			}

			const auto baseFormatStr = getAttributeFormatStringIdentifier( pAttributeInfo.baseFormat );
			formatStr.append( 1, ':' );
			formatStr.append( baseFormatStr );

			if( pAttributeInfo.subComponentsNum > 1 )
			{
				formatStr.append( 1, '[' );
				formatStr.append( std::to_string( pAttributeInfo.subComponentsNum ) );
				formatStr.append( 1, ']' );
			}

			return formatStr;
		}

		std::string generateVertexAttributeLayoutString( const SVertexAttributeArrayLayoutData & pLayoutData )
		{
			std::string vertexStreamStrings[GCM::IA_MAX_VERTEX_STREAMS_NUM];

			for( uint32 iAttribute = 0; iAttribute < GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM; ++iAttribute )
			{
				const auto & attributeInfo = pLayoutData[iAttribute];
				if( attributeInfo.isBaseAttribute() )
				{
					const auto attributeFormatStr = generateVertexAttributeFormatString( attributeInfo );
					if( !vertexStreamStrings[attributeInfo.vertexStreamIndex].empty() )
					{
						vertexStreamStrings[attributeInfo.vertexStreamIndex].append( 1, '|' );
					}
					vertexStreamStrings[attributeInfo.vertexStreamIndex].append( attributeFormatStr );
				}
			}

			std::string resultStringID;

			for( uint32 iStream = 0; iStream < GCM::IA_MAX_VERTEX_STREAMS_NUM; ++iStream )
			{
				if( !vertexStreamStrings[iStream].empty() )
				{
					if( !resultStringID.empty() )
					{
						resultStringID.append( 1, '_' );
					}
					resultStringID.append( 1, 's' );
					resultStringID.append( std::to_string( iStream ) );
					resultStringID.append( 1, '<' );
					resultStringID.append( vertexStreamStrings[iStream] );
					resultStringID.append( 1, '>' );
				}
			}

			return resultStringID;
		}

	}

}
