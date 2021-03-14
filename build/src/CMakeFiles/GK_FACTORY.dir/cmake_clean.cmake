file(REMOVE_RECURSE
  "../lib/libGK_FACTORY.a"
  "../lib/libGK_FACTORY.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/GK_FACTORY.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
