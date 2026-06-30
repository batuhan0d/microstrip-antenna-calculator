# Microstrip Patch Antenna Calculator

[![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B)
[![Framework](https://img.shields.io/badge/framework-Dear%20ImGui-purple.svg)](https://github.com/ocornut/imgui)
[![Build](https://img.shields.io/badge/build-CMake-red.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

A lightweight, high-performance desktop GUI application written in C++ using **Dear ImGui**, **GLFW**, and **OpenGL3**. This utility computes the precise physical dimensions of a rectangular microstrip patch antenna based on key engineering input parameters.

![App Preview](doc/ScreenshotOfApp.png.png)



## 📂 Repository Structure

```text
├── main.cpp           # Application entry point
├── CMakeLists.txt     # CMake build configuration
├── doc/               # Documentation and visual assets
├── fonts/             # Open-source TTF fonts (Roboto)
├── image/             # Antenna schematic images
├── imgui/             # Core ImGui library and backends
├── include/           # Header declarations (reqtangular.h, stb_image.h)
└── src/               # Additional source files

## 🏗️ Building and Running

This project uses **CMake** for generating build files. Ensure you have CMake and a C++ compiler installed. The CMake configuration is set up to automatically copy the required `image` and `fonts` directories to the build output folder

# 1. Create a build directory
mkdir build
cd build

# 2. Generate build files
cmake ..

# 3. Build the executable
cmake --build .