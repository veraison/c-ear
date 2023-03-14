# Copyright 2023 Contributors to the Veraison project.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.4)
project(ear)

include(CTest)
include(libjwt.cmake)
include(jansson.cmake)
find_package(OpenSSL REQUIRED)

add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(unity)
add_subdirectory(test)

target_compile_options(${PROJECT_NAME}
    PUBLIC
    -Wall -Wextra -Werror
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED OFF
    C_EXTENSIONS OFF
)
