#pragma once

#ifdef _WIN32
  // Evita arrastar cabeçalhos enormes e conflitos com std::min/max
  #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
  #endif
  #ifndef NOMINMAX
  #define NOMINMAX
  #endif
  #include <Windows.h>   // <— Windows.h vem ANTES de glad
#endif

// Diga ao GLFW para não incluir headers GL antigos
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

// glad depois do Windows.h (evita redef de APIENTRY)
#include <glad/glad.h>
#include <string>
#include <functional>
#include <cstdint>
#include <iostream>
#include <vector>
#include <mutex>
#include <memory>
#include "imgui.h"
