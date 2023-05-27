macro(list_subdirectories retval curdir)
  file(
    GLOB_RECURSE sub-dir
    LIST_DIRECTORIES true
    ${curdir}/*)
  set(list_of_dirs "")
  foreach(dir ${sub-dir})
    if(IS_DIRECTORY ${dir})
      set(list_of_dirs ${list_of_dirs} ${dir})
    endif()
  endforeach()
  set(${retval} ${list_of_dirs})
endmacro()
