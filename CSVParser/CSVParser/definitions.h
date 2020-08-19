/*
CSVParser

GNU GENERAL PUBLIC LICENSE
https://github.com/TutorExilius/CSVParser

Author:			Tutor Exilius (http://www.exilius.de)

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.
*/

#include <unordered_map>
#include <string>
#include <vector>

#include "refstring.h"

struct Point
{
    size_t row;
    size_t col;

    friend std::ostream& operator<<( std::ostream &out, const Point &obj )
    { 
        out << obj.row << ", " << obj.col;
        return out;
    }
};

// CSVParser
using Matrix = std::vector<std::vector<std::string>>;

// TableView
using RefVec = std::vector<RefString>;
using RefMatrix = std::vector<RefVec>;
using Groups = std::unordered_map<const RefString, RefMatrix, RefString::RefStringHasher>;
using CountedGroups = std::unordered_map<const RefString, size_t, RefString::RefStringHasher>;
