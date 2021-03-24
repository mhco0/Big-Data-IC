file(REMOVE_RECURSE
  "../lib/libQSBD_DEBUG.a"
  "../lib/libQSBD_DEBUG.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/QSBD_DEBUG.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
