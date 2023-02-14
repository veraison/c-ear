# Copyright 2023 Contributors to the Veraison project.
# SPDX-License-Identifier: Apache-2.0

find_library(JWT_LIB NAMES jwt)

if(NOT JWT_LIB)
    message(FATAL_ERROR "jwt library not found")
endif()

find_path(JWT_INCLUDE_DIR NAMES jwt.h)

if(NOT JWT_INCLUDE_DIR)
    message(FATAL_ERROR "jwt headers not found")
endif()

include_directories(${JWT_INCLUDE_DIR})

