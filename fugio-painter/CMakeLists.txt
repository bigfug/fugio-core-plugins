cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "Painter" )
set( PLUGIN_DESCRIPTION_SHORT "Painter nodes" )
set( PLUGIN_DESCRIPTION_LONG "Painter nodes" )
set( PLUGIN_FILENAME "fugio-painter" )

set( PLUGIN_UUID "b555e312-c198-44f9-8305-05400cd5b6ad" )
set( PLUGIN_IID "com.bigfug.fugio.painter" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Painter_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Painter_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Painter_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

set( HEADER_FILES
	include/fugio/painter/painter_helper.h
	include/fugio/painter/uuid.h
)

set( SOURCE_FILES
	source/painterplugin.cpp
	source/painterplugin.h

	source/brushnode.cpp
	source/brushnode.h
	source/circlenode.cpp
	source/circlenode.h
	source/clearnode.cpp
	source/clearnode.h
	source/drawimagenode.cpp
	source/drawimagenode.h
	source/fontmetricsnode.cpp
	source/fontmetricsnode.h
	source/fontnode.cpp
	source/fontnode.h
	source/fontpin.cpp
	source/fontpin.h
	source/painternode.cpp
	source/painternode.h
	source/pennode.cpp
	source/pennode.h
	source/rectnode.cpp
	source/rectnode.h
	source/switchnode.cpp
	source/switchnode.h
	source/textnode.cpp
	source/textnode.h
)

set( QRC_FILES
#	resources.qrc
)

add_library( ${PROJECT_NAME} MODULE ${SOURCE_FILES} ${HEADER_FILES} ${QRC_FILES} )

set_target_properties( ${PROJECT_NAME} PROPERTIES
  BUNDLE TRUE
)

#------------------------------------------------------------------------------
# Includes

target_include_directories( ${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/fugio-core/include
	${CMAKE_SOURCE_DIR}/fugio-colour/include
	${CMAKE_SOURCE_DIR}/fugio-image/include
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
# Qt

find_package(Qt6 COMPONENTS Core Widgets LinguistTools QUIET )

if (NOT Qt6_FOUND)
	find_package(Qt5 5.15 COMPONENTS Core Widgets LinguistTools QUIET)
endif()

target_link_libraries( ${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets )

#------------------------------------------------------------------------------
# Translations

set( TS_FILES
	translations/translations_de.ts
	)

if( COMMAND qt_add_translations )
	qt_add_translations( ${PROJECT_NAME} TS_FILES ${TS_FILES} RESOURCE_PREFIX "/translations" )
else()
	qt5_create_translation( QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES} )

	configure_file( ${CMAKE_SOURCE_DIR}/translations.qrc ${CMAKE_CURRENT_BINARY_DIR} COPYONLY )

	target_sources( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc ${QM_FILES} )
endif()

#------------------------------------------------------------------------------
# Unit Tests

if( FUGIO_LIBRARY_BUILD_DIR )
#add_subdirectory( tests )
endif()

set( CPACK_ARCHIVE_FUGIO-PAINTER_FILE_NAME "${PLUGIN_FILENAME}-${PLATFORM_NAME}" PARENT_SCOPE )

