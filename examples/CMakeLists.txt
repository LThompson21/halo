function(halo_example name)
  # Where to search for the source file
  set(_candidates
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/${name}.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp
  )

  set(_src "")
  foreach(p IN LISTS _candidates)
    if(EXISTS "${p}")
      set(_src "${p}")
      break()
    endif()
  endforeach()

  if(NOT _src)
    message(FATAL_ERROR "halo_example(${name}): couldn't find ${name}.cpp in: ${_candidates}")
  endif()

  # Sanitize target name and prefix
  set(_base "${name}")
  string(REGEX REPLACE "[^A-Za-z0-9_]" "_" _base "${_base}")
  string(REGEX REPLACE "_+" "_" _base "${_base}")
  if(_base MATCHES "^[0-9]")  # avoid leading digit
    set(_base "_${_base}")
  endif()

  set(_tgt "halo_example__${_base}")

  add_executable(${_tgt} "${_src}")
  target_link_libraries(${_tgt} PRIVATE halo::halo)

  # Nice predictable output dir
  set_target_properties(${_tgt} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  )

  # Optional: group in IDEs
  set_property(TARGET ${_tgt} PROPERTY FOLDER "examples")
endfunction()


# Define your examples here
halo_example(hello_world)
# Add more:
# halo_example(example_2 examples/example_2.cpp)
# halo_example(example_3 examples/example_3.cpp)
