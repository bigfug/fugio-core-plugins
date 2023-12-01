cmake_minimum_required( VERSION 3.14 )

project( fugio-core-tests  LANGUAGES CXX )

include_directories( ../include )

set( SOURCE_FILES
	testcore.cpp
	testcore.h
	${FUGIO_API}/fugio/test_helper.h
)

add_definitions( -DFUGIOLIB_STATIC )

add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )

find_package( Qt5 REQUIRED COMPONENTS Core Network Test )

target_link_directories( ${PROJECT_NAME} PRIVATE ${FUGIO_LIBRARY_BUILD_DIR} )

target_link_libraries( ${PROJECT_NAME} libfugio Qt5::Core Qt5::Test Qt5::Network )

add_test( ${PROJECT_NAME} ${PROJECT_NAME} )

#------------------------------------------------------------------------------
# Fugio API

if( FUGIO_API AND EXISTS ${FUGIO_API} )
	target_include_directories( ${PROJECT_NAME} PRIVATE ${FUGIO_API} )
endif()

