cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "GUI" )
set( PLUGIN_DESCRIPTION_SHORT "GUI nodes" )
set( PLUGIN_DESCRIPTION_LONG "GUI nodes" )
set( PLUGIN_FILENAME "fugio-gui" )

set( PLUGIN_UUID "680cbb42-0807-411c-a2d1-dcc1446d3638" )
set( PLUGIN_IID "com.bigfug.fugio.gui" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Gui_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Gui_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Gui_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" },
	{ \"plugin\": \"com.bigfug.fugio.colour\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

set( HEADER_FILES
	include/fugio/gui/input_events_interface.h
	include/fugio/gui/keyboard_interface.h
	include/fugio/gui/uuid.h
)

set( SOURCE_FILES
	source/guiplugin.cpp
	source/guiplugin.h

	source/floatnode.cpp
	source/buttonnode.cpp
	source/integernode.cpp
	source/lcdnumbernode.cpp
	source/keyboardnode.cpp
	source/lednode.cpp
	source/keyboarddialog.cpp
	source/slidernode.cpp
	source/keyboardpin.cpp
	source/mainwindownode.cpp
	source/choicenode.cpp
	source/numbermonitorform.cpp
	source/numbermonitornode.cpp
	source/stringnode.cpp
	source/leditem.cpp
	source/dialnode.cpp
	source/dialwidget.cpp
	source/screennode.cpp
	source/inputeventspin.cpp
	source/mousenode.cpp
	source/floatnode.h
	source/buttonnode.h
	source/integernode.h
	source/lcdnumbernode.h
	source/numbermonitorform.h
	source/numbermonitornode.h
	source/keyboardnode.h
	source/lednode.h
	source/keyboarddialog.h
	source/choicenode.h
	source/keyboardpin.h
	source/slidernode.h
	source/mainwindownode.h
	source/stringnode.h
	source/leditem.h
	source/dialnode.h
	source/dialwidget.h
	source/screennode.h
	source/inputeventspin.h
	source/mousenode.h
	source/labelnode.cpp
	source/labelnode.h

	source/numbermonitorform.ui
	source/keyboarddialog.ui
)

set( QRC_FILES
	# translations/translations.qrc
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

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fugio-core/include )
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
#	add_subdirectory( tests )
endif()

set( CPACK_ARCHIVE_FUGIO-GUI_FILE_NAME "${PLUGIN_FILENAME}-${PLATFORM_NAME}" PARENT_SCOPE )


