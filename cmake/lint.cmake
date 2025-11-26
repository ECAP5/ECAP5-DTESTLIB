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

set(DEFAULT_RULE_FILE ${CMAKE_CURRENT_LIST_DIR}/../config/default-verible-lint.rules)
set(DEFAULT_WAIVE_FILE ${CMAKE_CURRENT_LIST_DIR}/../config/default-verible-lint.waives)

function(add_lint_target)
  cmake_parse_arguments(ARG ""
                            "LIB;TARGET;CUSTOM_RULE_FILE;CUSTOM_WAIVE_FILE"
                            ""
                            ${ARGN})
  if (NOT ARG_TARGET)
    message(FATAL_ERROR "Need a target name")
  endif()

  if (NOT ARG_LIB)
    message(FATAL_ERROR "Need an interface library")
  endif()

  if (NOT TARGET ${ARG_LIB})
    message(FATAL_ERROR "Library ${ARG_LIB} not defined")
  endif()

  if (NOT CUSTOM_RULE_FILE)
    set(CUSTOM_RULE_FILE ${DEFAULT_RULE_FILE})
  endif()

  if (NOT CUSTOM_WAIVE_FILE)
    set(CUSTOM_WAIVE_FILE ${DEFAULT_WAIVE_FILE})
  endif()

  add_custom_target(${ARG_TARGET}
    COMMAND verible-verilog-lint $<TARGET_PROPERTY:${ARG_LIB},INTERFACE_SOURCES> --rules_config="${CUSTOM_RULE_FILE}" --waiver_files="${CUSTOM_WAIVE_FILE}"
    DEPENDS ${SRC_FILES} ${CUSTOM_RULE_FILE} ${CUSTOM_WAIVE_FILE}
    COMMAND_EXPAND_LISTS)
endfunction()

