# A + B = {x in A OR x in B}
macro(SET_UNION A B RET)
  set(_SET_UNION_RET ${${A}} ${${B}})
  list(REMOVE_DUPLICATES _SET_UNION_RET)
  set(${RET} ${_SET_UNION_RET})
endmacro()

macro(populate_op ret)
  set(${${ret}})
  cmake_parse_arguments(POPULATE_OP "" "" "SPECIFIC_OP" ${ARGN})
  foreach(kernel ${POPULATE_OP_SPECIFIC_OP})
    SET_UNION(${ret} kernel_depend_${kernel} ${ret})
  endforeach()
endmacro()
