file(REMOVE_RECURSE
  "../lib/libQUANTILE_SKETCH.a"
  "../lib/libQUANTILE_SKETCH.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/QUANTILE_SKETCH.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
