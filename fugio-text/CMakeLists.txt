cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "Text" )
set( PLUGIN_DESCRIPTION_SHORT "Text nodes" )
set( PLUGIN_DESCRIPTION_LONG "Text nodes" )
set( PLUGIN_FILENAME "fugio-text" )

set( PLUGIN_UUID "c939232f-8c22-401a-ab25-675298313b4a" )
set( PLUGIN_IID "com.bigfug.fugio.text" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Text_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Text_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Text_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

set( HEADER_FILES
	include/fugio/text/uuid.h

	include/fugio/text/syntax_error_interface.h
	include/fugio/text/syntax_error_signals.h
	include/fugio/text/syntax_highlighter_factory_interface.h
	include/fugio/text/syntax_highlighter_instance_interface.h
	include/fugio/text/syntax_highlighter_interface.h
)

set( SOURCE_FILES
	source/chopnode.cpp source/chopnode.h
	source/cmdtexteditorupdate.h
	source/codeeditor.cpp source/codeeditor.h
	source/comparestringnode.cpp source/comparestringnode.h
	source/containsnode.cpp source/containsnode.h
	source/leftnode.cpp source/leftnode.h
	source/linebuffernode.cpp source/linebuffernode.h
	source/linenumberarea.cpp source/linenumberarea.h
	source/midnode.cpp source/midnode.h
	source/numbertostringnode.cpp source/numbertostringnode.h
	source/regexpnode.cpp source/regexpnode.h
	source/rightnode.cpp source/rightnode.h
	source/stringjoinnode.cpp source/stringjoinnode.h
	source/stringsplitnode.cpp source/stringsplitnode.h
	source/stringtofloatnode.cpp source/stringtofloatnode.h
	source/stringtointegernode.cpp source/stringtointegernode.h
	source/syntaxerrorpin.cpp source/syntaxerrorpin.h
	source/texteditorform.cpp source/texteditorform.h
	source/texteditorform.ui
	source/texteditornode.cpp source/texteditornode.h
	source/texteditorremotenode.cpp source/texteditorremotenode.h
	source/textplugin.cpp source/textplugin.h
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

set( CPACK_ARCHIVE_FUGIO-TEXT_FILE_NAME "${PLUGIN_FILENAME}-${PLATFORM_NAME}" PARENT_SCOPE )

