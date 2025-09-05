#!/bin/bash

# Diretório de build
BUILD_DIR="build"

# Se não existir, cria
if [ ! -d "$BUILD_DIR" ]; then
    mkdir $BUILD_DIR
fi

# Gera os arquivos de build com CMake
cmake -S . -B $BUILD_DIR

# Compila o projeto
cmake --build $BUILD_DIR

# Executa o binário
./$BUILD_DIR/SimpleEngine
