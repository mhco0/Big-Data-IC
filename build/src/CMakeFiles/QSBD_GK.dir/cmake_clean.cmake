file(REMOVE_RECURSE
  "../lib/libQSBD_GK.a"
  "../lib/libQSBD_GK.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/QSBD_GK.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
