# Copyright 2023 Contributors to the Veraison project.
# SPDX-License-Identifier: Apache-2.0

add_library(unity STATIC unity.c)

target_include_directories(unity PUBLIC .)
