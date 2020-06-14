/*

Author:			Tutor Exilius (http://www.exilius.de)

Created:		12.10.2018
Last update:	13.10.2018

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.

*/


#include "file_not_found_exception.h"

FileNotFoundException::FileNotFoundException( const std::string &errorMessage )
    : errorMessage{ errorMessage }
{
}

const char *FileNotFoundException::what() const noexcept
{
    return this->errorMessage.c_str();
}