file(REMOVE_RECURSE
  "libCOMMUM_HEADER.a"
  "libCOMMUM_HEADER.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/COMMUM_HEADER.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
