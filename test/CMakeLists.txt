# Copyright 2023 Contributors to the Veraison project.
# SPDX-License-Identifier: Apache-2.0

add_executable(ear_test ear_test.c)

target_link_libraries(ear_test unity)
target_link_libraries(ear_test ear)
target_link_libraries(ear_test ${JWT_LIB})
target_link_libraries(ear_test ${JANSSON_LIB})
target_link_libraries(ear_test OpenSSL::Crypto)

add_test(ear_test ear_test)
