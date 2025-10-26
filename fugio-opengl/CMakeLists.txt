cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "OpenGL" )
set( PLUGIN_DESCRIPTION_SHORT "OpenGL nodes" )
set( PLUGIN_DESCRIPTION_LONG "OpenGL nodes" )
set( PLUGIN_FILENAME "fugio-opengl" )

set( PLUGIN_UUID "a111c714-22e2-46bf-9fc8-375d9973ae3e" )
set( PLUGIN_IID "com.bigfug.fugio.OpenGL" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/OpenGL_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/OpenGL_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/OpenGL_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

set( HEADER_FILES
	include/fugio/opengl/uuid.h
)

set( SOURCE_FILES
	source/arraytobuffernode.cpp
	source/arraytobuffernode.h
	source/arraytoindexnode.cpp
	source/arraytoindexnode.h
	source/bindtexturenode.cpp
	source/bindtexturenode.h
	source/bufferentrynode.cpp
	source/bufferentrynode.h
	source/bufferentrypin.cpp
	source/bufferentrypin.h
	source/bufferentryproxypin.cpp
	source/bufferentryproxypin.h
	source/buffernode.cpp
	source/buffernode.h
	source/bufferpin.cpp
	source/bufferpin.h
	source/buffertoarraynode.cpp
	source/buffertoarraynode.h
	source/clearnode.cpp
	source/clearnode.h
	source/contextnode.cpp
	source/contextnode.h
	source/cubemaprendernode.cpp
	source/cubemaprendernode.h
	source/deviceopengloutput.cpp
	source/deviceopengloutput.h
	source/drawnode.cpp
	source/drawnode.h
	source/imagetotexturenode.cpp
	source/imagetotexturenode.h
	source/instancebuffernode.cpp
	source/instancebuffernode.h
	source/opengl_includes.h
	source/openglplugin.cpp
	source/openglplugin.h
	source/preview.cpp
	source/preview.h
	source/previewnode.cpp
	source/previewnode.h
	source/rendernode.cpp
	source/rendernode.h
	source/renderpin.cpp
	source/renderpin.h
	source/rendertotexturenode.cpp
	source/rendertotexturenode.h
	source/shadercompilernode.cpp
	source/shadercompilernode.h
	source/shaderinstancenode.cpp
	source/shaderinstancenode.h
	source/shaderpin.cpp
	source/shaderpin.h
	source/stateform.cpp
	source/stateform.h
	source/statenode.cpp
	source/statenode.h
	source/statepin.cpp
	source/statepin.h
	source/syntaxhighlighterglsl.cpp
	source/syntaxhighlighterglsl.h
	source/textureclonenode.cpp
	source/textureclonenode.h
	source/texturecopynode.cpp
	source/texturecopynode.h
	source/texturecubenode.cpp
	source/texturecubenode.h
	source/texturemonitor.cpp
	source/texturemonitornode.cpp
	source/texturenode.cpp
	source/texturenode.h
	source/texturenodeform.cpp
	source/texturenodeform.h
	source/texturepin.cpp
	source/texturepin.h
	source/texturetoimagenode.cpp
	source/texturetoimagenode.h
	source/transformfeedbacknode.cpp
	source/transformfeedbacknode.h
	source/vertexarrayobjectnode.cpp
	source/vertexarrayobjectnode.h
	source/vertexarrayobjectpin.cpp
	source/vertexarrayobjectpin.h
	source/viewportmatrixnode.cpp
	source/viewportmatrixnode.h
	source/viewportnode.cpp
	source/viewportnode.h
	source/windownode.cpp
	source/windownode.h
    source/texturemonitor.h
    source/texturemonitornode.h

	source/easyshader2dnode.cpp
	source/easyshader2dnode.h
	source/shadercompilerdata.cpp
	source/shadercompilerdata.h
	source/arraytotexturebuffernode.cpp
	source/arraytotexturebuffernode.h

	source/openglstateform.ui
	source/texturenodeform.ui
)

set( QRC_FILES
)

add_library( ${PROJECT_NAME} MODULE ${SOURCE_FILES} ${HEADER_FILES} ${QRC_FILES} )

set_target_properties( ${PROJECT_NAME} PROPERTIES
  BUNDLE TRUE
)

#------------------------------------------------------------------------------
# Includes

target_include_directories( ${PROJECT_NAME} PRIVATE ../fugio-core/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ../fugio-file/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ../fugio-colour/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ../fugio-text/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ../fugio-image/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ../fugio-math/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ../fugio-gui/include )

#------------------------------------------------------------------------------
# Fugio API

if( FUGIO_API AND EXISTS ${FUGIO_API} )
	target_include_directories( ${PROJECT_NAME} PRIVATE ${FUGIO_API} )

	target_link_libraries( ${PROJECT_NAME} PUBLIC libfugio )
endif()

#------------------------------------------------------------------------------
# Plugin Config

configure_file(
	../config.h.in
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
	../manifest.json.in
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

find_package(Qt6 COMPONENTS Core Widgets OpenGL OpenGLWidgets Concurrent LinguistTools QUIET )

if (NOT Qt6_FOUND)
	find_package(Qt5 5.15 COMPONENTS Core Widgets OpenGL OpenGLWidgets Concurrent LinguistTools QUIET)
endif()

target_link_libraries( ${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets Qt::OpenGL Qt::OpenGLWidgets Qt::Concurrent )

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

set( CPACK_ARCHIVE_FUGIO-OPENGL_FILE_NAME "${PLUGIN_FILENAME}-${PLATFORM_NAME}" PARENT_SCOPE )

