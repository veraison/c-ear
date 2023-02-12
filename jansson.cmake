# Copyright 2023 Contributors to the Veraison project.
# SPDX-License-Identifier: Apache-2.0

find_library(JANSSON_LIB NAMES jansson)

if(NOT JANSSON_LIB)
    message(FATAL_ERROR "jansson library not found")
endif()

find_path(JANSSON_INCLUDE_DIR NAMES jansson.h)

if(NOT JANSSON_INCLUDE_DIR)
    message(FATAL_ERROR "jansson headers not found")
endif()

include_directories(${JANSSON_INCLUDE_DIR})
