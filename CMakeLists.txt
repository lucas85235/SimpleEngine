cmake_minimum_required(VERSION 3.16)
project(SimpleEngineProject LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ASSETS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_TARGET_DIR ${CMAKE_BINARY_DIR}/assets)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${ASSETS_SOURCE_DIR} ${ASSETS_TARGET_DIR}
    COMMENT "Copy assets to build directory"
)

add_subdirectory(engine)
add_subdirectory(apps/sandbox)
