# Copyright 2023 Contributors to the Veraison project.
# SPDX-License-Identifier: Apache-2.0

add_executable(ear-verify verify.c)

target_link_libraries(ear-verify ear)
target_link_libraries(ear-verify ${JWT_LIB})
target_link_libraries(ear-verify ${JANSSON_LIB})
target_link_libraries(ear-verify OpenSSL::Crypto)
