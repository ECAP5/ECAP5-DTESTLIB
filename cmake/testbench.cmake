#           __        _
#  ________/ /  ___ _(_)__  ___
# / __/ __/ _ \/ _ `/ / _ \/ -_)
# \__/\__/_//_/\_,_/_/_//_/\__/
# 
# Copyright (C) Clément Chain
# This file is part of ECAP5-DTESTLIB <https://github.com/ecap5/ECAP5-DTESTLIB>
# 
# ECAP5-DTESTLIB is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# ECAP5-DTESTLIB is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with ECAP5-DTESTLIB.  If not, see <http://www.gnu.org/licenses/>.

macro(add_testbench)
  cmake_parse_arguments(ARG ""
                            "MODULE;BENCH_DIR;LIBS_DIR;BENCH;TESTDATA_DIR"
                            "SRC_DIRS;INCLUDE_DIRS;LIBS;CUSTOM_DEPENDS"
                            ${ARGN})
  if(NOT ARG_BENCH_DIR)
    message(FATAL_ERROR "Need a bench directory")
  endif()

  if(NOT ARG_MODULE)
    message(FATAL_ERROR "Need a module name")
  endif()

  if(NOT ARG_TESTDATA_DIR)
    message(FATAL_ERROR "Need a testdata directory")
  endif()

  if(NOT ARG_SRC_DIRS)
    message(FATAL_ERROR "Need at least one source directory")
  endif()

  # Create folders for the waves and testdata
  file(MAKE_DIRECTORY ${TESTDATA_DIR}/waves)
  file(MAKE_DIRECTORY ${TESTDATA_DIR}/testdata)

  if("${ARG_BENCH}" STREQUAL "")
    set(ARG_BENCH ${ARG_MODULE})
  endif()
  set(TARGET tb_${ARG_BENCH})

  # Make a file list for testing libraries
  list(TRANSFORM ARG_LIBS APPEND ".sv")
  list(TRANSFORM ARG_LIBS PREPEND ${ARG_LIBS_DIR})

  # List include files
  file(GLOB INCLUDE_FILES ${ARG_INCLUDE_DIRS}/*.svh)

  # Create the test executable
  add_executable(${TARGET} ${ARG_BENCH_DIR}/${ARG_MODULE}/${TARGET}.cpp)
  target_include_directories(${TARGET} PUBLIC ${TEST_INCLUDE_DIR})
  verilate(${TARGET}
    PREFIX V${TARGET}
    SOURCES ${INCLUDE_FILES}
            ${ARG_BENCH_DIR}/${ARG_MODULE}/${TARGET}.sv
            ${ARG_LIBS}
    INCLUDE_DIRS ${ARG_SRC_DIRS}
    TRACE)

  set(TEST_TARGET simulate_${ARG_BENCH})
  set(TEST_OUTPUT ${ARG_TESTDATA_DIR}/${ARG_BENCH}.csv)
  # Create the ${TEST_TARGET} command to run the test
  get_target_property(BINARY_DIR "${TARGET}" BINARY_DIR)
  if(${DEBUGLOG})
    set(RUN_TARGET_ARGUMENT "-v")  
  endif()
  add_custom_command(
    COMMAND ${TARGET} ${RUN_TARGET_ARGUMENT}
    OUTPUT ${TEST_OUTPUT}
    DEPENDS ${TARGET}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests/)
  add_custom_target(${TEST_TARGET} DEPENDS ${TEST_OUTPUT})
  # Add the ${TEST_TARGET} target to the test targets
  list(APPEND TEST_BINARIES ${TARGET})
  list(APPEND TEST_TARGETS ${TEST_TARGET})
  list(APPEND TEST_OUTPUTS ${TEST_OUTPUT})
endmacro()

