cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "Time" )
set( PLUGIN_DESCRIPTION_SHORT "Time nodes" )
set( PLUGIN_DESCRIPTION_LONG "Time nodes" )
set( PLUGIN_FILENAME "fugio-time" )

set( PLUGIN_UUID "843c26cc-a007-42e6-a8a6-140b8d649c9e" )
set( PLUGIN_IID "com.bigfug.fugio.time" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Time_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Time_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Time_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

set( HEADER_FILES
	include/fugio/time/uuid.h
)

set( SOURCE_FILES
	source/beattapnode.cpp source/beattapnode.h
	source/clocknode.cpp source/clocknode.h
	source/cronform.ui source/cronnode.cpp source/cronnode.h
	source/datenode.cpp source/datenode.h
	source/datepin.cpp source/datepin.h
	source/datetimepin.cpp source/datetimepin.h
	source/datetimetostringnode.cpp source/datetimetostringnode.h
	source/delaynode.cpp source/delaynode.h
	source/everynode.cpp source/everynode.h
	source/inertianode.cpp source/inertianode.h
	source/localtimenode.cpp source/localtimenode.h
	source/playheadcontrolnode.cpp source/playheadcontrolnode.h
	source/playheadframesnode.cpp source/playheadframesnode.h
	source/playheadnode.cpp source/playheadnode.h
	source/splitdatetimenode.cpp source/splitdatetimenode.h
	source/timenode.cpp source/timenode.h
	source/timepin.cpp source/timepin.h
	source/timeplugin.cpp source/timeplugin.h
	source/universetimenode.cpp source/universetimenode.h
	source/utctimenode.cpp source/utctimenode.h
)

set( QRC_FILES
)

add_library( ${PROJECT_NAME} MODULE ${SOURCE_FILES} ${HEADER_FILES} ${QRC_FILES} )

set_target_properties( ${PROJECT_NAME} PROPERTIES
  BUNDLE TRUE
)

#------------------------------------------------------------------------------
# Includes

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fugio-core/include )

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

target_link_libraries( ${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets )

#------------------------------------------------------------------------------
# Translations

set( TS_FILES
	translations/translations_de.ts
	)

qt_add_translations( ${PROJECT_NAME} TS_FILES ${TS_FILES} RESOURCE_PREFIX "/translations" )

#------------------------------------------------------------------------------
# Unit Tests

if( FUGIO_LIBRARY_BUILD_DIR )
#add_subdirectory( tests )
endif()

set( CPACK_ARCHIVE_FUGIO-TIME_FILE_NAME "${PLUGIN_FILENAME}-${PLATFORM_NAME}" PARENT_SCOPE )

