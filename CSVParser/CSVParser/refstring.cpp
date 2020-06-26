#include "refstring.h"

RefString::RefString( std::string *ref )
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
