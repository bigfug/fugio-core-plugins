cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "File" )
set( PLUGIN_DESCRIPTION_SHORT "File nodes" )
set( PLUGIN_DESCRIPTION_LONG "File nodes" )
set( PLUGIN_FILENAME "fugio-file" )

set( PLUGIN_UUID "8882583b-6398-46fb-a5e3-c3a8918cd3cf" )
set( PLUGIN_IID "com.bigfug.fugio.file" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/File_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/File_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/File_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

set( HEADER_FILES
	include/fugio/file/uuid.h
	include/fugio/file/filename_interface.h
)

set( SOURCE_FILES
	source/fileplugin.cpp source/fileplugin.h
	source/filenamenode.cpp source/filenamenode.h
	source/filenamepin.cpp source/filenamepin.h
	source/fileplugin.cpp source/fileplugin.h
	source/filewatchernode.cpp source/filewatchernode.h
	source/loadnode.cpp source/loadnode.h
)

set( QRC_FILES
	# translations/translations.qrc
)

add_library( ${PLUGIN_FILENAME} MODULE ${SOURCE_FILES} ${HEADER_FILES} ${QRC_FILES} )

set_target_properties( ${PLUGIN_FILENAME} PROPERTIES
  BUNDLE TRUE
)

#------------------------------------------------------------------------------
# Includes

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_include_directories( ${PLUGIN_FILENAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../fugio-core/include )

#------------------------------------------------------------------------------
# Fugio API

if( FUGIO_API AND EXISTS ${FUGIO_API} )
	target_include_directories( ${PLUGIN_FILENAME} PRIVATE ${FUGIO_API} )

	target_link_libraries( ${PROJECT_NAME} PUBLIC libfugio )
endif()

#------------------------------------------------------------------------------
# Plugin Config

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/../config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h
	@ONLY
)

target_sources( ${PLUGIN_FILENAME} PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/config.h
	)

target_include_directories( ${PLUGIN_FILENAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR} )

#------------------------------------------------------------------------------
# Plugin Manifest

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/../manifest.json.in
	${CMAKE_CURRENT_BINARY_DIR}/manifest.json
	@ONLY
)

target_sources( ${PLUGIN_FILENAME} PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/manifest.json
	)

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json DESTINATION . COMPONENT ${PLUGIN_FILENAME} )

#------------------------------------------------------------------------------
# Install

install( TARGETS ${PLUGIN_FILENAME}
	LIBRARY
	DESTINATION plugins
	COMPONENT ${PLUGIN_FILENAME}
)

install( DIRECTORY include DESTINATION . COMPONENT ${PLUGIN_FILENAME} PATTERN ".*" EXCLUDE )
#install( DIRECTORY examples	DESTINATION . COMPONENT ${PLUGIN_FILENAME} PATTERN ".*" EXCLUDE )

#------------------------------------------------------------------------------
# Qt

find_package(Qt6 COMPONENTS Core Widgets LinguistTools QUIET )

if (NOT Qt6_FOUND)
	find_package(Qt5 5.15 COMPONENTS Core Widgets LinguistTools QUIET)
endif()

qt_standard_project_setup( I18N_TRANSLATED_LANGUAGES de es fr it )

target_link_libraries( ${PLUGIN_FILENAME} PRIVATE Qt::Core Qt::Widgets )

qt_generate_deploy_app_script(
	TARGET ${PROJECT_NAME}
	OUTPUT_SCRIPT deploy_script
	NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

#------------------------------------------------------------------------------
# Translations

# set( TS_FILES
# 	translations/translations_de.ts
# 	)

# if( COMMAND qt_add_translations )
# 	qt_add_translations( ${PLUGIN_FILENAME} TS_FILES ${TS_FILES} RESOURCE_PREFIX "/translations" )
# else()
# 	qt5_create_translation( QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES} )

# 	configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/translations.qrc ${CMAKE_CURRENT_BINARY_DIR} COPYONLY )

# 	target_sources( ${PLUGIN_FILENAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc ${QM_FILES} )
# endif()

#------------------------------------------------------------------------------
# Unit Tests

if( FUGIO_LIBRARY_BUILD_DIR )
#add_subdirectory( tests )
endif()

set( CPACK_ARCHIVE_FUGIO-FILE_FILE_NAME "${PLUGIN_FILENAME}-${PLATFORM_NAME}" PARENT_SCOPE )

