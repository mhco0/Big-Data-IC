file(REMOVE_RECURSE
  "../lib/libQSBD_COMMUM_HEADER.a"
  "../lib/libQSBD_COMMUM_HEADER.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/QSBD_COMMUM_HEADER.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
