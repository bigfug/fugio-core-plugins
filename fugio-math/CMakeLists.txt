cmake_minimum_required( VERSION 3.14 )

#-------------------------------------------------------------------------------
# PLUGIN DEFINES

set( PLUGIN_NAME "Maths" )
set( PLUGIN_DESCRIPTION_SHORT "Maths nodes" )
set( PLUGIN_DESCRIPTION_LONG "Maths nodes" )
set( PLUGIN_FILENAME "fugio-math" )

set( PLUGIN_UUID "f4e1c7d7-ded2-4874-b1bb-2966b52be94e" )
set( PLUGIN_IID "com.bigfug.fugio.math" )

set( PLUGIN_RELEASE_VERSION "1.0.0" )
set( PLUGIN_COMPATIBILITY_VERSION "1.0.0" )
set( PLUGIN_API_COMPATIBILITY "4.0.0" )

set( PLUGIN_RELEASE_DATE "2019-10-25" )
set( PLUGIN_AUTHOR_NAME  "Alex May" )
set( PLUGIN_AUTHOR_CONTACT "https://github.com/bigfug" )

set( PLUGIN_WIKI_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Math_Plugin" )
set( PLUGIN_WIKI_NODE_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Math_Node_" )
set( PLUGIN_WIKI_PIN_LINK "https://github.com/bigfug/fugio-core-plugins/wiki/Math_Pin_" )

set( PLUGIN_REQUIREMENTS "\
	{ \"plugin\": \"com.bigfug.fugio.core\", \"version\": \"1.0.0\" }"
)

#-------------------------------------------------------------------------------

project( ${PLUGIN_FILENAME} VERSION ${PLUGIN_RELEASE_VERSION} LANGUAGES CXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

set( HEADER_FILES
	include/fugio/math/math_interface.h
	include/fugio/math/matrix_interface.h
	include/fugio/math/uuid.h
)

set( SOURCE_FILES
	source/absnode.cpp
	source/absnode.h
	source/addnode.cpp
	source/addnode.h
	source/andnode.cpp
	source/andnode.h
	source/arccosnode.cpp
	source/arccosnode.h
	source/booleanlatchnode.cpp
	source/booleanlatchnode.h
	source/ceilnode.cpp
	source/ceilnode.h
	source/comparenumbersnode.cpp
	source/comparenumbersnode.h
	source/crossproducenode.cpp
	source/crossproducenode.h
	source/dividenode.cpp
	source/dividenode.h
	source/dotproductnode.cpp
	source/dotproductnode.h
	source/floornode.cpp
	source/floornode.h
	source/joinvector3.cpp
	source/joinvector3.h
	source/joinvector4node.cpp
	source/joinvector4node.h
	source/mathplugin.cpp
	source/mathplugin.h
	source/matrixpin.cpp
	source/matrixpin.h
	source/matrix4node.cpp
	source/matrix4node.h
	source/matrix4pin.cpp
	source/matrix4pin.h
	source/matrixinversenode.cpp
	source/matrixinversenode.h
	source/matrixlookatnode.cpp
	source/matrixlookatnode.h
	source/matrixmultiplynode.cpp
	source/matrixmultiplynode.h
	source/matrixorthographicnode.cpp
	source/matrixorthographicnode.h
	source/matrixperspectivenode.cpp
	source/matrixperspectivenode.h
	source/matrixrotatenode.cpp
	source/matrixrotatenode.h
	source/matrixscalenode.cpp
	source/matrixscalenode.h
	source/matrixtranslatenode.cpp
	source/matrixtranslatenode.h
	source/minmaxnode.cpp
	source/minmaxnode.h
	source/modulusnode.cpp
	source/modulusnode.h
	source/multiplynode.cpp
	source/multiplynode.h
	source/nandnode.cpp
	source/nandnode.h
	source/normalisenode.cpp
	source/normalisenode.h
	source/nornode.cpp
	source/nornode.h
	source/notnode.cpp
	source/notnode.h
	source/ornode.cpp
	source/ornode.h
	source/pinode.cpp
	source/pinode.h
	source/quaternionpin.cpp
	source/quaternionpin.h
	source/radianstodegreesnode.cpp
	source/radianstodegreesnode.h
	source/rotationfromvectorsnode.cpp
	source/rotationfromvectorsnode.h
	source/roundnode.cpp
	source/roundnode.h
	source/sindegreenode.cpp
	source/sindegreenode.h
	source/sinradiannode.cpp
	source/sinradiannode.h
	source/splitvector3.cpp
	source/splitvector3.h
	source/splitvector4node.cpp
	source/splitvector4node.h
	source/subtractnode.cpp
	source/subtractnode.h
	source/vector3node.cpp
	source/vector3node.h
	source/vector3pin.cpp
	source/vector3pin.h
	source/vector4pin.cpp
	source/vector4pin.h
	source/xnornode.cpp
	source/xnornode.h
	source/xornode.cpp
	source/xornode.h
	source/mathexpressionnode.cpp
	source/mathexpressionnode.h

	source/orbitsnode.cpp
	source/orbitsnode.h
	source/andbitsnode.cpp
	source/andbitsnode.h
	source/notbitsnode.cpp
	source/notbitsnode.h
	source/xorbitsnode.cpp
	source/xorbitsnode.h
	source/flipflopnode.cpp
	source/flipflopnode.h
	source/nandbitsnode.cpp
	source/nandbitsnode.h
	source/inttobitsnode.cpp
	source/inttobitsnode.h
	source/transformnode.cpp
	source/transformnode.h
	source/bitstopinsnode.cpp
	source/bitstopinsnode.h
	source/multiplexornode.cpp
	source/multiplexornode.h
	source/numberarraynode.cpp
	source/numberarraynode.h
	source/cosinedegreesnode.cpp
	source/cosinedegreesnode.h
	source/pointtransformnode.cpp
	source/pointtransformnode.h
	source/maxnode.cpp
	source/maxnode.h
	source/minnode.cpp
	source/minnode.h
	source/pownode.cpp
	source/pownode.h
	source/sumnode.cpp
	source/sumnode.h
	source/bitstointnode.cpp
	source/bitstointnode.h
)

set( QRC_FILES
)

if( MSVC )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj" )
endif( MSVC )

add_library( ${PROJECT_NAME} MODULE ${SOURCE_FILES} ${HEADER_FILES} ${QRC_FILES} )

set_target_properties( ${PROJECT_NAME} PROPERTIES
  BUNDLE TRUE
)

#------------------------------------------------------------------------------
# Includes

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fugio-core/include )

#------------------------------------------------------------------------------
# Eigen

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/eigen )

#------------------------------------------------------------------------------
# ExprTk

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libs/exprtk )

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
	LIBRARY DESTINATION plugins
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

set( CPACK_ARCHIVE_FUGIO-MATH_FILE_NAME "${PLUGIN_FILENAME}-${PLATFORM_NAME}" PARENT_SCOPE )
