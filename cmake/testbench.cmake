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

function(get_all_sources_recursive TARGET_NAME OUTPUT_LIST)
  set(CURRENT_FILES "")

  get_target_property(RAW_SOURCES ${TARGET_NAME} INTERFACE_SOURCES)

  if(RAW_SOURCES AND NOT "${RAW_SOURCES}" MATCHES "NOTFOUND")
    list(APPEND CURRENT_FILES ${RAW_SOURCES})
  endif()

  get_target_property(RAW_LIBS ${TARGET_NAME} INTERFACE_LINK_LIBRARIES)
  if(RAW_LIBS AND NOT "${RAW_LIBS}" MATCHES "NOTFOUND")
    foreach(DEP ${RAW_LIBS})
      if(TARGET ${DEP})
        get_all_sources_recursive(${DEP} SUB_FILES_LIST)
        list(APPEND CURRENT_FILES ${SUB_FILES_LIST})
      else()
        message(FATAL_ERROR "Recursive dependency ${DEP} not found")  
      endif()
    endforeach()
  endif()

  if(CURRENT_FILES)
    list(REMOVE_DUPLICATES CURRENT_FILES)
  endif()

  set(${OUTPUT_LIST} ${CURRENT_FILES} PARENT_SCOPE)
endfunction()

macro(add_testbench)
  cmake_parse_arguments(ARG ""
                            "MODULE;BENCH_DIR;BENCH;TESTDATA_DIR"
			                      "LIBS;DEPENDS;TEST_INCLUDE_DIRS;DEFINES"
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

  if(NOT ARG_LIBS)
    message(FATAL_ERROR "Need at least one source library")
  endif()

  foreach(LIB ${ARG_LIBS})
    if(NOT TARGET ${LIB})
      message(FATAL_ERROR "Library ${LIB} not defined")
    endif()
  endforeach()

  # Create folders for the waves and testdata
  file(MAKE_DIRECTORY ${TESTDATA_DIR}/waves)
  file(MAKE_DIRECTORY ${TESTDATA_DIR}/testdata)

  if("${ARG_BENCH}" STREQUAL "")
    set(ARG_BENCH ${ARG_MODULE})
  endif()
  set(TARGET tb_${ARG_BENCH})

  # Define a source collecting library
  add_library(lib${TARGET} INTERFACE)
  target_link_libraries(lib${TARGET} INTERFACE ${ARG_LIBS})

  get_all_sources_recursive(lib${TARGET} ${TARGET}_SOURCES)

  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

  # Create the test executable
  add_executable(${TARGET} ${ARG_BENCH_DIR}/${ARG_MODULE}/${TARGET}.cpp)
  if(ARG_DEPENDS)
    add_dependencies(${TARGET} ${ARG_DEPENDS})
  endif()
  target_include_directories(${TARGET} PUBLIC ${ARG_TEST_INCLUDE_DIRS})
  verilate(${TARGET}
    PREFIX     V${TARGET}
    TOP_MODULE ${TARGET}
    SOURCES    ${${TARGET}_SOURCES}
               ${ARG_BENCH_DIR}/${ARG_MODULE}/${TARGET}.sv
    TRACE
    DEFINES ${ARG_DEFINES})

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

