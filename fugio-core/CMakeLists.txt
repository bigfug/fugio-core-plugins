cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "Core" )
set( PLUGIN_DESCRIPTION_SHORT "Core nodes" )
set( PLUGIN_DESCRIPTION_LONG "Core nodes" )
set( PLUGIN_FILENAME "fugio-core" )

set( PLUGIN_UUID "ee6f7880-e762-42b4-891b-6bb58373e753" )
set( PLUGIN_IID "com.bigfug.fugio.core" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Core_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Core_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Core_Pin_" )

set( PLUGIN_REQUIREMENTS "" )

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

set( HEADER_FILES
	include/fugio/core/uuid.h
	include/fugio/core/array_interface.h
	include/fugio/core/array_list_interface.h
	include/fugio/core/list_interface.h
	include/fugio/core/size_interface.h
	include/fugio/core/struct_entry_interface.h
	include/fugio/core/variant_helper.h
	include/fugio/core/variant_interface.h
)

set( SOURCE_FILES
	source/coreplugin.cpp

	source/stringpin.cpp
	source/integerpin.cpp
	source/floatpin.cpp
	source/triggerpin.cpp
	source/boolpin.cpp
	source/bytearraypin.cpp
	source/bytearraypin.h
	source/numberrangenode.cpp
	source/randomnumbernode.cpp
	source/anytriggernode.cpp
	source/ratecontrolnode.cpp
	source/numberrangelimitnode.cpp
	source/boolrangenode.cpp
	source/togglenode.cpp
	source/triggerrangenumbernode.cpp
	source/outputrangenode.cpp
	source/sizepin.cpp
	source/splitsizenode.cpp
	source/joinsizenode.cpp
	source/arraypin.cpp
	source/pointpin.cpp
	source/size3dpin.cpp
	source/coreplugin.cpp
	source/splitlistnode.cpp
#    source/listpin.cpp
	source/indexnode.cpp
	source/booltotriggernode.cpp
	source/signalnumbernode.cpp
	source/envelopenode.cpp
	source/numberspreadnode.cpp
	source/choicepin.cpp
	source/splitpointnode.cpp
	source/variantpin.cpp
	source/duplicatepinsnode.cpp
	source/lastupdatedinputnode.cpp
	source/counternode.cpp
	source/listsizenode.cpp
	source/listindexnode.cpp
	source/framedelaynode.cpp
	source/typesizenode.cpp
	source/switchnode.cpp
	source/floatthresholdnode.cpp
	source/smoothnode.cpp
	source/autorangenode.cpp
	source/rectpin.cpp
	source/getsizenode.cpp
	source/triggerarraynode.cpp
	source/triggerbooleannode.cpp
	source/makearraynode.cpp
	source/flipflopnode.cpp
	source/loggernode.cpp
	source/arraylistentry.h
	source/bitstoboolnode.cpp
	source/bitstoboolnode.h
	source/booltobitsnode.cpp
	source/booltobitsnode.h
	source/bitarraypin.cpp
	source/bitarraypin.h
	source/linepin.cpp
	source/linepin.h
	source/joinpointnode.cpp
	source/joinpointnode.h
	source/joinrectnode.cpp
	source/joinrectnode.h
	source/transformpin.cpp
	source/transformpin.h
	source/polygonpin.cpp
	source/polygonpin.h
	source/splitrectnode.cpp
	source/splitrectnode.h
	source/integertofloatnode.cpp
	source/integertofloatnode.h
	source/blockupdatesnode.cpp
	source/blockupdatesnode.h
	source/processclicknode.cpp
	source/processclicknode.h
	source/decimatenode.cpp
	source/decimatenode.h
	source/updatenode.cpp
	source/updatenode.h
)

set( QRC_FILES
#	resources.qrc
)

add_library( ${PROJECT_NAME} MODULE ${SOURCE_FILES} ${HEADER_FILES} ${QRC_FILES} )

set_target_properties( ${PROJECT_NAME} PROPERTIES
  BUNDLE TRUE
)

#------------------------------------------------------------------------------
# Fugio API

if( FUGIO_API AND EXISTS ${FUGIO_API} )
	target_include_directories( ${PROJECT_NAME} PRIVATE ${FUGIO_API} )

	target_sources( ${PROJECT_NAME} PRIVATE
		${FUGIO_API}/fugio/nodecontrolbase.h
		${FUGIO_API}/fugio/pincontrolbase.h
		)
endif()

#------------------------------------------------------------------------------
# Additional includes

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fugio-math/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fugio-colour/include )

#------------------------------------------------------------------------------
# Plugin Config

configure_file(
	${CMAKE_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h
	@ONLY
)

target_sources( ${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/config.h
	)

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR} )

#------------------------------------------------------------------------------
# Plugin Manifest

configure_file(
	${CMAKE_SOURCE_DIR}/manifest.json.in
	${CMAKE_CURRENT_BINARY_DIR}/manifest.json
	@ONLY
)

target_sources( ${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/manifest.json
	)

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json DESTINATION . COMPONENT ${PROJECT_NAME} )

#------------------------------------------------------------------------------
# Install

install( TARGETS ${PROJECT_NAME}
	LIBRARY
	DESTINATION plugin
	COMPONENT ${PROJECT_NAME}
)

install( DIRECTORY include DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY examples	DESTINATION . COMPONENT ${PROJECT_NAME} )

#------------------------------------------------------------------------------
# Qt5

find_package( Qt5 COMPONENTS Core Widgets LinguistTools REQUIRED )

target_link_libraries( ${PROJECT_NAME} Qt5::Core Qt5::Widgets )

#------------------------------------------------------------------------------
# Translations

set( TS_FILES
	translations/translations_de.ts
	)

qt5_create_translation( QM_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source ${TS_FILES} OPTIONS "-silent" )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/translations/translations.qrc ${CMAKE_CURRENT_BINARY_DIR} COPYONLY )

qt5_add_resources( QRC_FILES ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc )

target_sources( ${PROJECT_NAME} PRIVATE
	${TS_FILES}
	${QM_FILES}
	${QRC_FILES}
	)

set_property( SOURCE ${CMAKE_CURRENT_BINARY_DIR}/qrc_translations.cpp PROPERTY SKIP_AUTOGEN ON )

#------------------------------------------------------------------------------
# Unit Tests

if( FUGIO_LIBRARY_BUILD_DIR )
	add_subdirectory( tests )
endif()

set( CPACK_ARCHIVE_fugio-core_FILE_NAME "fugio-core-${PLUGIN_RELEASE_VERSION}" )


