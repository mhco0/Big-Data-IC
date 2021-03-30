file(REMOVE_RECURSE
  "../lib/libnlohmann_json.a"
  "../lib/libnlohmann_json.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/nlohmann_json.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
