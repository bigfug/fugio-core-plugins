cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "Image" )
set( PLUGIN_DESCRIPTION_SHORT "Image nodes" )
set( PLUGIN_DESCRIPTION_LONG "Image nodes" )
set( PLUGIN_FILENAME "fugio-image" )

set( PLUGIN_UUID "ae70e6be-c67a-466e-8d53-ca82aaf368cb" )
set( PLUGIN_IID "com.bigfug.fugio.image" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Image_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Image_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Image_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" },
	{ \"plugin\": \"com.bigfug.fugio.file\", \"version\": \"1.0.0\" },
	{ \"plugin\": \"com.bigfug.fugio.gui\", \"version\": \"1.0.0\" },
	{ \"plugin\": \"com.bigfug.fugio.colour\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

set( HEADER_FILES
	include/fugio/image/uuid.h
	include/fugio/image/image.h
	include/fugio/image/painter_interface.h
)

set( SOURCE_FILES	
	source/colourmasknode.cpp source/colourmasknode.h
	source/copynode.cpp source/copynode.h
	source/grabscreennode.cpp source/grabscreennode.h
	source/imagefilternode.cpp source/imagefilternode.h
	source/imageloadernode.cpp source/imageloadernode.h
	source/imagenode.cpp source/imagenode.h
	source/imagepin.cpp source/imagepin.h
	source/imageplugin.cpp source/imageplugin.h
	source/imagepreview.cpp source/imagepreview.h
	source/imagepreviewnode.cpp source/imagepreviewnode.h
	source/imagesavenode.cpp source/imagesavenode.h
	source/imagesizenode.cpp source/imagesizenode.h
	source/imagesplitnode.cpp source/imagesplitnode.h
	source/painterpin.cpp source/painterpin.h
	source/painterwindow.cpp source/painterwindow.h
	source/painterwindownode.cpp source/painterwindownode.h
	source/replacealphanode.cpp source/replacealphanode.h
	source/scaleimagenode.cpp source/scaleimagenode.h
	source/setalphanode.cpp source/setalphanode.h
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
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fugio-colour/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fugio-gui/include )

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
	DESTINATION plugin
	COMPONENT ${PROJECT_NAME}
)

install( DIRECTORY include DESTINATION . COMPONENT ${PROJECT_NAME} )

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
#add_subdirectory( tests )
endif()

set( CPACK_ARCHIVE_FUGIO-IMAGE_FILE_NAME "${PLUGIN_FILENAME}-${PLUGIN_RELEASE_VERSION}-${PLATFORM_NAME}" PARENT_SCOPE )

