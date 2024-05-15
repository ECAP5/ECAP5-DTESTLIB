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

cmake_minimum_required(VERSION 3.12)
include(FetchContent)

project(ECAP5_DTESTLIB VERSION 1.0.0)

# Create python virtual env
set(VENV_DIR ${CMAKE_BINARY_DIR}/venv)
if(NOT EXISTS "${VENV_DIR}")
  find_package(Python3 COMPONENTS Interpreter)
  execute_process(COMMAND "${Python3_EXECUTABLE}" -m venv ${VENV_DIR})
  set(Python3_FIND_VIRTUALENV ONLY)
  unset(Python3_EXECUTABLE)
endif()
set(ENV{VIRTUAL_ENV} "${VENV_DIR}")
find_package(Python3 COMPONENTS Interpreter Development)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(${CMAKE_CURRENT_LIST_DIR}/cmake/ecap5-treq.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/verilator-config.cmake)

add_subdirectory(docs)
add_subdirectory(tests)

