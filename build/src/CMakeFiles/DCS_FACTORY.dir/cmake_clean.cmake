file(REMOVE_RECURSE
  "../lib/libDCS_FACTORY.a"
  "../lib/libDCS_FACTORY.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/DCS_FACTORY.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
