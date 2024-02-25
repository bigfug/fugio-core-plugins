cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "Network" )
set( PLUGIN_DESCRIPTION_SHORT "Network nodes" )
set( PLUGIN_DESCRIPTION_LONG "Network nodes" )
set( PLUGIN_FILENAME "fugio-network" )

set( PLUGIN_UUID "25e353e6-ef5d-42fd-b568-d2ff2b5d2843" )
set( PLUGIN_IID "com.bigfug.fugio.network" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Network_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Network_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Network_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.file\", \"version\": \"1.0.0\" },
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

set( HEADER_FILES
	include/fugio/network/uuid.h
)

set( SOURCE_FILES
	source/cobsdecodenode.cpp source/cobsdecodenode.h
	source/cobsencodenode.cpp source/cobsencodenode.h
	source/crc32.cpp source/crc32.h
	source/getnode.cpp source/getnode.h
	source/networkplugin.cpp source/networkplugin.h
	source/packetdecodenode.cpp source/packetdecodenode.h
	source/packetencodenode.cpp source/packetencodenode.h
	source/slipdecodenode.cpp source/slipdecodenode.h
	source/slipencodenode.cpp source/slipencodenode.h
	source/tcpreceivenode.cpp source/tcpreceivenode.h
	source/tcpreceiverawnode.cpp source/tcpreceiverawnode.h
	source/tcpsendnode.cpp source/tcpsendnode.h
	source/tcpsendrawnode.cpp source/tcpsendrawnode.h
	source/udpreceiverawnode.cpp source/udpreceiverawnode.h
	source/udpsendrawnode.cpp source/udpsendrawnode.h
	source/universereceivenode.cpp source/universereceivenode.h
	source/universesendnode.cpp source/universesendnode.h
	source/websocketclientnode.cpp source/websocketclientnode.h
	source/websocketdataservernode.cpp source/websocketdataservernode.h
	source/websocketservernode.cpp source/websocketservernode.h
)

set( QRC_FILES
	translations/translations.qrc
)

add_library( ${PROJECT_NAME} MODULE ${SOURCE_FILES} ${HEADER_FILES} ${QRC_FILES} )

set_target_properties( ${PROJECT_NAME} PROPERTIES
  BUNDLE TRUE
)

#------------------------------------------------------------------------------
# Includes

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fugio-core/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fugio-file/include )

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
	DESTINATION plugins
	COMPONENT ${PROJECT_NAME}
)

install( DIRECTORY include DESTINATION . COMPONENT ${PROJECT_NAME} PATTERN ".*" EXCLUDE )
install( DIRECTORY examples	DESTINATION . COMPONENT ${PROJECT_NAME} PATTERN ".*" EXCLUDE )

#------------------------------------------------------------------------------
# Qt5

find_package( Qt5 COMPONENTS Core Widgets Network LinguistTools REQUIRED )

target_link_libraries( ${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Network )

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
#add_subdirectory( tests )
endif()

set( CPACK_ARCHIVE_FUGIO-NETWORK_FILE_NAME "${PLUGIN_FILENAME}-${PLATFORM_NAME}" PARENT_SCOPE )

