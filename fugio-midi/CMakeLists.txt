cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "MIDI" )
set( PLUGIN_DESCRIPTION_SHORT "MIDI nodes" )
set( PLUGIN_DESCRIPTION_LONG "MIDI nodes" )
set( PLUGIN_FILENAME "fugio-midi" )

set( PLUGIN_UUID "d6979717-108a-43da-8c32-f9bd10a02bd3" )
set( PLUGIN_IID "com.bigfug.fugio.json" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Midi_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Midi_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Midi_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

set( HEADER_FILES
	include/fugio/midi/midi_input_interface.h
	include/fugio/midi/midi_interface.h
	include/fugio/midi/uuid.h
)

set( SOURCE_FILES
	source/midiplugin.cpp
	source/midiplugin.h

	source/channelinputnode.cpp
	source/channelinputnode.h
	source/channeloutputnode.cpp
	source/channeloutputnode.h
	source/frequencytonotenode.cpp
	source/frequencytonotenode.h
	source/intervalnode.cpp
	source/intervalnode.h
	source/mididecodernode.cpp
	source/mididecodernode.h
	source/midifileloadnode.cpp
	source/midifileloadnode.h
	source/midiinputhelpernode.cpp
	source/midiinputhelpernode.h
	source/midiinputpin.cpp
	source/midiinputpin.h
	source/midiinputsyncnode.cpp
	source/midiinputsyncnode.h
	source/midinotenode.cpp
	source/midinotenode.h
	source/midioutputhelpernode.cpp
	source/midioutputhelpernode.h
	source/midioutputnode.cpp
	source/midioutputnode.h
	source/midioutputpin.cpp
	source/midioutputpin.h
	source/midirotarynode.cpp
	source/midirotarynode.h
	source/notetofrequencynode.cpp
	source/notetofrequencynode.h
	source/scalenode.cpp
	source/scalenode.h

	# source/miditimelinenode.cpp
	# source/miditimelinenode.h
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
)

#------------------------------------------------------------------------------
# midifile

target_include_directories( ${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/libs
)

target_sources( ${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/libs/midifile/midifile.cpp
	${CMAKE_SOURCE_DIR}/libs/midifile/midifile.h
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

set( CPACK_ARCHIVE_FUGIO-MIDI_FILE_NAME "${PLUGIN_FILENAME}-${PLATFORM_NAME}" PARENT_SCOPE )

