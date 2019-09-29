#
# Copyright (c) 2019 Christian Mazakas (christian dot mazakas at gmail dot com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt
# or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/LeonineKing1199/quail
#

cmake_minimum_required(VERSION 3.10)

project(
  quail

  LANGUAGES
    CXX

  VERSION
    1.0.0

  DESCRIPTION
    "QUIC transport protocol written in Boost.Asio"
)

find_package(
  Boost 1.71
  REQUIRED
    headers
)

add_library(
  quail

  INTERFACE
)

target_compile_features(quail INTERFACE cxx_std_17)

if (MSVC)
  # Win10
  #
  set(QUAIL_WIN_ARCH "0x0A00" CACHE STRING "Asio-specific Win architecture value (default: Win10)")
  target_compile_definitions(quail INTERFACE "_WIN32_WINNT=${QUAIL_WIN_ARCH}")

  target_compile_options(quail INTERFACE "/bigobj")
endif()

target_include_directories(
  quail

  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

target_link_libraries(
  quail

  INTERFACE
    Boost::headers
)

if (BUILD_TESTING)
  include(CTest)

  find_package(Catch2 CONFIG REQUIRED)

  add_executable(
    quail_tests

    test/main.cpp

    test/long_header_packet_format_test.cpp
  )

  target_link_libraries(
    quail_tests

    PRIVATE
      quail
      Catch2::Catch2
  )

  include(Catch)
  catch_discover_tests(quail_tests)
endif()
