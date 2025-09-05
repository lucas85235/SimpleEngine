add_executable(sandbox
    src/main.cpp
    src/App.cpp
)

target_include_directories(sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link only the engine; transitive deps come from the engine target
target_link_libraries(sandbox PRIVATE simple_engine)

# Warnings (optional)
if (MSVC)
  target_compile_options(sandbox PRIVATE /W4 /permissive-)
else()
  target_compile_options(sandbox PRIVATE -Wall -Wextra -Wpedantic)
endif()
