add_executable(sandbox
    src/main.cpp
    src/App.cpp
)

# Liga o sandbox APENAS à engine. A engine trata do resto.
target_link_libraries(sandbox PRIVATE simple_engine)

# (Opcional: pode adicionar a macro de path aqui se precisar dela no main.cpp)
target_compile_definitions(sandbox PRIVATE PROJECT_SOURCE_DIR="${CMAKE_PROJECT_DIR}")