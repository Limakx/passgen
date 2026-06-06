# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Limakx

cmake_minimum_required(VERSION 3.10)

project(passgen VERSION 1.0.0 LANGUAGES CXX)

# Required C++11 std::random_device and std::mt19937
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(passgen src/passgen.cpp)

if (MSVC)
    target_compile_options(passgen PRIVATE /W4)
else ()
    target_compile_options(passgen PRIVATE -Wall -Wextra -Wpedantic)
endif ()

set_target_properties(passgen PROPERTIES
    LINK_FLAGS "-static-libgcc -static-libstdc++"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)