file(REMOVE_RECURSE
  "../lib/libSKETCH_FACTORY.a"
  "../lib/libSKETCH_FACTORY.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/SKETCH_FACTORY.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
