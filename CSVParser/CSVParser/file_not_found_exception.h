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


#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include <exception>
#include <string>

class FileNotFoundException final : public std::exception
{
public:
	explicit FileNotFoundException( const std::string &errorMessage );

	const char* what() const noexcept override;

private:
	std::string errorMessage;
};

#endif // FILENOTFOUNDEXCEPTION_H