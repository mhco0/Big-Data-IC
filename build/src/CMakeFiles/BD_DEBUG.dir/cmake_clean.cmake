file(REMOVE_RECURSE
  "../lib/libBD_DEBUG.a"
  "../lib/libBD_DEBUG.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/BD_DEBUG.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
