find_package(Sphinx)

if(Sphinx_FOUND)
  add_sphinx_document(
      documentation
      CONF_FILE "${CMAKE_CURRENT_LIST_DIR}/src/conf.py"
      SKIP_PDF
      "${CMAKE_CURRENT_LIST_DIR}/src/index.rst"
      "${CMAKE_CURRENT_LIST_DIR}/src/instr_wb_slave.rst"
      "${CMAKE_CURRENT_LIST_DIR}/src/assets/logo.svg"
      "${CMAKE_CURRENT_LIST_DIR}/src/_static/css/custom.css"
    )
else()
  message(WARNING "No Sphinx found. Documentation target not available.")
endif()
