file(REMOVE_RECURSE
  "../lib/libGK.a"
  "../lib/libGK.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/GK.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
