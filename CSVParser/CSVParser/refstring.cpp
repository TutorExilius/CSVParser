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

#include "refstring.h"

RefString::RefString( const RefString &obj )
{
    *this = obj;
}

RefString::RefString( std::string *const ref )
: ref{ ref }
{
}

RefString& RefString::operator=( const std::string &value )
{
    *(this->ref) = value;
    return *this;
}

bool RefString::operator==( const std::string &value ) const
{
    return *(this->ref) == value;
}

RefString::operator std::string()
{
    return *ref;
}

RefString::operator const std::string() const
{
    return *ref;
}
