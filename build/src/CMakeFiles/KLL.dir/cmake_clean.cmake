file(REMOVE_RECURSE
  "libKLL.a"
  "libKLL.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/KLL.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
