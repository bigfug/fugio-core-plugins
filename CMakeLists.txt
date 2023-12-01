cmake_minimum_required( VERSION 3.14 )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

project( fugio-core-plugins VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# Instruct CMake to run moc automatically when needed.
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTORCC OFF )

set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON )

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "*** Have SIZEOF void * = 8, so 64-bit")
    set( IS_64_BIT 1 )
else ()
    message(STATUS "*** SIZEOF void * != 8, so not 64-bit")
endif ()

# There must be a better way to do this...

if( EXISTS /opt/vc/include/bcm_host.h )
	set( Q_OS_RASPBERRY_PI TRUE )

	add_definitions( -DQ_OS_RASPBERRY_PI )

	message( "RASPBERRY PI BUILD" )
endif()

#------------------------------------------------------------------------------
# Fugio API

set( FUGIO_API "" CACHE PATH "" )

if( NOT FUGIO_API OR NOT EXISTS ${FUGIO_API} )
	message( "--- FUGIO_API NOT SET ---" )
endif()

#------------------------------------------------------------------------------
# Enable Unit Testing

set( FUGIO_LIBRARY_BUILD_DIR "" CACHE PATH "" )

include( CTest )

set( CTEST_OUTPUT_ON_FAILURE ON )

#------------------------------------------------------------------------------

set( PATH_APP . )
set( PATH_INCLUDE include )
set( PATH_SHARE share )
set( PATH_PLUGIN plugins )

if( WIN32 )
	set( PATH_APP bin )
	set( CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1 )
endif( WIN32 )

if( UNIX AND NOT APPLE )
	set( PATH_APP		bin )
	set( PATH_SHARE		share/fugio )
	set( PATH_PLUGIN	lib/fugio )
endif()

#------------------------------------------------------------------------------

set( FUGIO_UNIX_DEPENDENCIES )

#add_subdirectory( Audio )
add_subdirectory( fugio-colour )
add_subdirectory( fugio-core )
#add_subdirectory( File )
#add_subdirectory( Firmata )
#add_subdirectory( Gamepad )
#add_subdirectory( Geometry )
#add_subdirectory( GUI )
#add_subdirectory( Image )
#add_subdirectory( JSON )
add_subdirectory( fugio-math )
#add_subdirectory( Network )
#add_subdirectory( OpenGL )
#add_subdirectory( OSC )
#add_subdirectory( Painter )
#add_subdirectory( QML )
#add_subdirectory( Serial )
#add_subdirectory( Text )
#add_subdirectory( Time )

#------------------------------------------------------------------------------
# CPack

set( CPACK_PACKAGE_VENDOR "bigfug.com" )
set( CPACK_PACKAGE_CONTACT "fugio@bigfug.com" )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Core plugins for Fugio" )

set( CPACK_GENERATOR "ZIP" )
set( CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF )
set( CPACK_ARCHIVE_COMPONENT_INSTALL ON )
set( CPACK_COMPONENTS_GROUPING ONE_PER_GROUP )

include( CPack )
