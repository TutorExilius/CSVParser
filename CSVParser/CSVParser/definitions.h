#include <unordered_map>
#include <string>
#include <vector>

#include "refstring.h"

struct Point
{
    size_t row;
    size_t col;
};

// CSVParser
using Matrix = std::vector<std::vector<std::string>>;

// TableView
using RefVec = std::vector<RefString>;
using RefMatrix = std::vector<RefVec>;
using Groups = std::unordered_map<const RefString, std::vector<RefVec>, RefString::RefStringHasher>;
using CountedGroups = std::unordered_map<const RefString, size_t, RefString::RefStringHasher>;
