add_executable(sandbox
        src/AppLayer.cpp
        src/AppLayer.h
        src/main.cpp)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET ImGui PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set_property(TARGET sandbox PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_link_libraries(sandbox PUBLIC simple_engine)

target_compile_definitions(sandbox PUBLIC PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")

if(EXISTS "${ASSETS_SOURCE_DIR}")
  add_custom_command(
    TARGET sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying assets to binary dir..."
          COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:sandbox>/assets"
          COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_SOURCE_DIR}" "$<TARGET_FILE_DIR:sandbox>/assets"
    VERBATIM
  )
else()
  message(WARNING "Assets dir not found: ${ASSETS_SOURCE_DIR}")
endif()
