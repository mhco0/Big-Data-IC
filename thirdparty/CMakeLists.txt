add_library(csv_parser
    "csv_parser/csv.h"
)

add_library(nlohmann_json
    "nlohmann/json.hpp"
)

set_target_properties(nlohmann_json PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(csv_parser PROPERTIES LINKER_LANGUAGE CXX)