
set( CM_EBS_WORKSPACE__VARLIST
	CM_EBS_WORKSPACE_COMMON_DEFAULT_COMPILER_CCVERSION
	CM_EBS_WORKSPACE_COMMON_DEFAULT_COMPILER_CXXVERSION
	CM_EBS_WORKSPACE_COMMON_GROUP_LIST
	CM_EBS_WORKSPACE_COMMON_PROJECT_LIST
)

set( CM_EBS_WORKSPACE__PROPERTYLIST
	CM_EBS_WORKSPACE_GCONF_GID_PARENT_GROUP_ID
	CM_EBS_WORKSPACE_GCONF_GID_SUBGROUP_LIST
	CM_EBS_WORKSPACE_GCONF_GID_SUBPROJECT_LIST
	CM_EBS_WORKSPACE_PCONF_PID_PARENT_GROUP_ID
	CM_EBS_WORKSPACE_PROPERTY_IID_COMPILER_DEFINITIONS
	CM_EBS_WORKSPACE_PROPERTY_IID_COMPILER_FLAGS
	CM_EBS_WORKSPACE_PROPERTY_IID_INCLUDE_PATH
	CM_EBS_WORKSPACE_PROPERTY_IID_LINKER_FLAGS
	CM_EBS_WORKSPACE_PROPERTY_IID_LINKER_INPUT
	CM_EBS_WORKSPACE_PROPERTY_IID_LINKER_PATH
	CM_EBS_WORKSPACE_PROPERTY_IID_PCH_ACTIVE
	CM_EBS_WORKSPACE_PROPERTY_IID_PCH_FILE_HEADER
	CM_EBS_WORKSPACE_PROPERTY_IID_PCH_FILE_SOURCE
)

function( ebsWspAddGroup pGroupID pParentGroupID )
	ebsCoreAppendUniqueInternal( CM_EBS_WORKSPACE_COMMON_GROUP_LIST ${pGroupID} )
	if( pParentGroupID )
		set( CM_EBS_WORKSPACE_GCONF_${pGroupID}_PARENT_GROUP_ID ${pParentGroupID} )
		ebsCoreAppendUniqueInternal( CM_EBS_WORKSPACE_GCONF_${pParentGroupID}_SUBGROUP_LIST ${pGroupID} )
	endif()
endfunction()

function( ebsWspAddProject pProjectID pParentGroupID )
	ebsCoreAppendUniqueInternal( CM_EBS_WORKSPACE_COMMON_PROJECT_LIST ${pProjectID} )
	if( pParentGroupID )
		set( CM_EBS_WORKSPACE_PCONF_${pProjectID}_PARENT_GROUP_ID ${pParentGroupID} )
		ebsCoreAppendUniqueInternal( CM_EBS_WORKSPACE_GCONF_${pParentGroupID}_SUBPROJECT_LIST ${pProjectID} )
	endif()
endfunction()
