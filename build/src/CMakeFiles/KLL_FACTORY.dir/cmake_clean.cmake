file(REMOVE_RECURSE
  "../lib/libKLL_FACTORY.a"
  "../lib/libKLL_FACTORY.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/KLL_FACTORY.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
