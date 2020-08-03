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

#ifndef REFSTRING_H
#define REFSTRING_H

#include <iostream>
#include <functional>
#include <string>


class RefString
{
public:
    struct RefStringHasher
    {
        size_t operator()( const RefString &k ) const
        {
            return std::hash<std::string>{}( *( k.ref ) );
        }
    };

    RefString() = default;
    RefString( std::string *ref );

    RefString& operator=( const std::string &value );

    bool operator==( const std::string &value ) const;
    operator std::string();
    operator const std::string() const;

    // friends ---
    friend std::istream &operator>>( std::istream &in, RefString &refString )
    {
        in >> *( refString.ref );
        return in;
    }
    // ---
private:
    std::string *ref;
};

#endif // REFSTRING_H