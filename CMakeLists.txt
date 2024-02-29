cmake_minimum_required( VERSION 3.14 )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

project( fugio-core-plugins VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# Instruct CMake to run moc automatically when needed.
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTORCC ON )

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

set( FUGIO_API_ZIP_URL "https://github.com/bigfug/Fugio/releases/download/v3.1.0/fugio-api.zip" CACHE STRING "URL to fugio-api.zip" )

if( NOT EXISTS "${CMAKE_BINARY_DIR}/include/fugio" )
	message( "Initialising fugio-api" )

	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include )

	file(DOWNLOAD
                ${FUGIO_API_ZIP_URL}
		${CMAKE_BINARY_DIR}/fugio-api.zip
	)

	execute_process( COMMAND "${CMAKE_COMMAND}" -E tar -xzf ${CMAKE_BINARY_DIR}/fugio-api.zip
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/include" )
endif()

set( FUGIO_API "${CMAKE_BINARY_DIR}/include" )

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

if( UNIX )
	execute_process( COMMAND lsb_release -is
		OUTPUT_VARIABLE UNIX_ID
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	execute_process( COMMAND lsb_release -rs
		OUTPUT_VARIABLE UNIX_RELEASE
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	execute_process( COMMAND lsb_release -cs
		OUTPUT_VARIABLE UNIX_CODENAME
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	if( "${UNIX_ID}" STREQUAL "" )
		set( UNIX_ID "unknown" )
	else()
		message( "UNIX ID: ${UNIX_ID}" )
	endif()

	if( "${UNIX_RELEASE}" STREQUAL "" )
		set( UNIX_RELEASE "unknown" )
	else()
		message( "UNIX RELEASE: ${UNIX_RELEASE}" )
	endif()

	if( "${UNIX_CODENAME}" STREQUAL "" )
		set( UNIX_CODENAME "unknown" )
	else()
		message( "UNIX CODENAME: ${UNIX_CODENAME}" )
	endif()
endif( UNIX )

# Set the output package extension

if( WIN32 AND IS_64_BIT )
	set( PLATFORM_NAME "win64" )
elseif( WIN32 )
	set( PLATFORM_NAME "win32" )
elseif( APPLE )
	set( PLATFORM_NAME "macOS" )
elseif( UNIX AND IS_64_BIT )
	set( PLATFORM_NAME "${UNIX_CODENAME}-x64" )
elseif( UNIX )
	set( PLATFORM_NAME "${UNIX_CODENAME}-i386" )
endif()

#------------------------------------------------------------------------------

set( FUGIO_UNIX_DEPENDENCIES )

add_compile_definitions(QT_DISABLE_DEPRECATED_UP_TO=0x050F00)

add_subdirectory( api )
add_subdirectory( fugio-audio )
add_subdirectory( fugio-colour )
add_subdirectory( fugio-core )
add_subdirectory( fugio-file )
#add_subdirectory( Firmata )
#add_subdirectory( Gamepad )
#add_subdirectory( Geometry )
add_subdirectory( fugio-gui )
add_subdirectory( fugio-image )
#add_subdirectory( JSON )
add_subdirectory( fugio-math )
add_subdirectory( fugio-network )
#add_subdirectory( OpenGL )
#add_subdirectory( OSC )
#add_subdirectory( Painter )
#add_subdirectory( QML )
add_subdirectory( fugio-serial )
add_subdirectory( fugio-text )
add_subdirectory( fugio-time )

#------------------------------------------------------------------------------

set( FUGIO_PLUGIN_URL ${CMAKE_BINARY_DIR} CACHE STRING "" )

configure_file( fugio-manifest.json.in ${CMAKE_BINARY_DIR}/fugio-manifest.json @ONLY )

add_custom_target( fugio-plugin-manifest
	SOURCES ${CMAKE_CURRENT_BINARY_DIR}/fugio-manifest.json
)

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
