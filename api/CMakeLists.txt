cmake_minimum_required( VERSION 3.14 )

project( fugio-core-api LANGUAGES CXX )

install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-audio/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-colour/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-core/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-file/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-geometry/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-gui/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-image/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-json/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-math/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-midi/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-network/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-painter/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-serial/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-text/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )
install( DIRECTORY ${CMAKE_SOURCE_DIR}/fugio-time/include/fugio DESTINATION . COMPONENT ${PROJECT_NAME} )

set( CPACK_ARCHIVE_FUGIO-CORE-API_FILE_NAME "fugio-core-api" PARENT_SCOPE )

