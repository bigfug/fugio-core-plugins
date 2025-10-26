cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "QML" )
set( PLUGIN_DESCRIPTION_SHORT "QML nodes" )
set( PLUGIN_DESCRIPTION_LONG "QML nodes" )
set( PLUGIN_FILENAME "fugio-qml" )

set( PLUGIN_UUID "76076cbf-7781-4efc-9f83-11df6f4ce8f1" )
set( PLUGIN_IID "com.bigfug.fugio.qml" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Qml_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Qml_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Qml_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.file\", \"version\": \"1.0.0\" },
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

set( HEADER_FILES
	include/fugio/qml/uuid.h
)

set( SOURCE_FILES
	source/qmlplugin.cpp
	source/qmlwidget.cpp
	source/qmlinterfacenode.cpp
	source/qmlnode.cpp
	source/qmlpin.cpp
	source/qmlplugin.h
	source/qmlwidget.h
	source/qmlinterfacenode.h
	source/qmlnode.h
	source/qmlpin.h
	source/syntaxhighlighterqml.cpp
	source/syntaxhighlighterqml.h
)

set( QRC_FILES
)

add_library( ${PROJECT_NAME} MODULE ${SOURCE_FILES} ${HEADER_FILES} ${QRC_FILES} )

set_target_properties( ${PROJECT_NAME} PROPERTIES
  BUNDLE TRUE
)

#------------------------------------------------------------------------------
# Includes

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source )

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../fugio-core/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../fugio-file/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../fugio-image/include )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../fugio-text/include )

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
	DESTINATION plugins/fugio
	COMPONENT ${PLUGIN_FILENAME}
)

install( DIRECTORY include DESTINATION . COMPONENT ${PLUGIN_FILENAME} PATTERN ".*" EXCLUDE )
install( DIRECTORY examples	DESTINATION . COMPONENT ${PLUGIN_FILENAME} PATTERN ".*" EXCLUDE )

#------------------------------------------------------------------------------
# Qt

find_package(Qt6 COMPONENTS Core Core5Compat Qml Quick QuickWidgets Gui Widgets LinguistTools OPTIONAL_COMPONENTS QuickControls2 QUIET )

qt_standard_project_setup( I18N_TRANSLATED_LANGUAGES de es fr it )

target_link_libraries( ${PLUGIN_FILENAME} PRIVATE Qt::Core Qt::Core5Compat Qt::Qml Qt::Quick Qt::QuickWidgets )

if( Qt5QuickControls2_DIR OR Qt6QuickControls2_DIR )
	target_link_libraries( ${PLUGIN_FILENAME} PRIVATE Qt::QuickControls2 )
endif()

qt_generate_deploy_app_script(
	TARGET ${PROJECT_NAME}
	OUTPUT_SCRIPT deploy_script
	NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

# qt_add_qml_module(
# 	asdsd
# 	STATIC
# 	URI Fugio
# 	VERSION 4.0
# 	SOURCES
# 		source/qmlpin.cpp source/qmlpin.h
# 		source/qmlnode.cpp source/qmlnode.h
# 	# PLUGIN_TARGET ${PLUGIN_FILENAME}
# )

# target_link_libraries( ${PLUGIN_FILENAME} PRIVATE asdsd )

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

set( CPACK_ARCHIVE_FUGIO-QML_FILE_NAME "${PLUGIN_FILENAME}-${PLATFORM_NAME}" PARENT_SCOPE )

