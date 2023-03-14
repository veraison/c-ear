# Copyright 2023 Contributors to the Veraison project.
# SPDX-License-Identifier: Apache-2.0

add_library(ear ear.c utils.c base64.c)

set_target_properties(ear PROPERTIES PUBLIC_HEADER "ear.h")

install(TARGETS ear
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

target_include_directories(ear PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ear ${JWT_LIB})
